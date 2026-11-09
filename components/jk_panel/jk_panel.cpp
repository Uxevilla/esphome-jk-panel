#include "jk_panel.h"
#include "esphome/core/log.h"
#include "esphome/core/string_ref.h"

namespace esphome::jk_panel {

static const char *const TAG = "jk_panel";

void JkPanel::setup() { this->base_->add_handler(this); }

void JkPanel::dump_config() { ESP_LOGCONFIG(TAG, "JK panel: /panel (%u bytes gzip)", (unsigned) this->size_); }

// ¿Es una escritura (number: /set; switch: /turn_on,/turn_off,/toggle) sobre una entidad
// marcada como solo lectura (nombre con " ro ")?
static bool is_readonly_write(const std::string &u) {
  const bool num = u.compare(0, 8, "/number/") == 0;
  const bool sw = u.compare(0, 8, "/switch/") == 0;
  if (!num && !sw)
    return false;
  if (u.find("/set") == std::string::npos && u.find("/turn_") == std::string::npos &&
      u.find("/toggle") == std::string::npos)
    return false;
  return u.find("%20ro%20") != std::string::npos || u.find(" ro ") != std::string::npos ||
         u.find("+ro+") != std::string::npos || u.find("_ro_") != std::string::npos;
}

bool JkPanel::canHandle(AsyncWebServerRequest *request) const {
  char url_buf[AsyncWebServerRequest::URL_BUF_SIZE];
  StringRef url = request->url_to(url_buf);
  if (is_readonly_write(std::string(url.c_str(), url.size())))
    return true;  // cualquier método: ESPHome acepta GET y POST en /set
  if (request->method() != HTTP_GET)
    return false;
  return url == "/panel" || url == "/panel/";
}

void JkPanel::handleRequest(AsyncWebServerRequest *request) {
  char url_buf[AsyncWebServerRequest::URL_BUF_SIZE];
  StringRef url = request->url_to(url_buf);
  if (is_readonly_write(std::string(url.c_str(), url.size()))) {
    ESP_LOGW(TAG, "Escritura vetada en parámetro de solo lectura");
    request->send(403, "text/plain", "Parametro de solo lectura");
    return;
  }
  auto *response = request->beginResponse(200, "text/html", this->data_, this->size_);
  response->addHeader("Content-Encoding", "gzip");
  response->addHeader("Cache-Control", "no-cache");
  request->send(response);
}

}  // namespace esphome::jk_panel
