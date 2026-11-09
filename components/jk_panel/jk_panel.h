#pragma once
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/web_server_base/web_server_base.h"

namespace esphome::jk_panel {

// Sirve una página HTML (gzip, incrustada en el firmware) en /panel.
// Se registra con add_handler(), así que respeta el usuario/contraseña de web_server.
// Además VETA (403) cualquier escritura (/number/<nombre>/set) sobre números cuyo nombre
// contiene " ro ": ESPHome no filtra los números internos en su API REST.
class JkPanel : public AsyncWebHandler, public Component {
 public:
  explicit JkPanel(web_server_base::WebServerBase *base) : base_(base) {}
  void set_data(const uint8_t *data, size_t size) {
    this->data_ = data;
    this->size_ = size;
  }
  void setup() override;
  void dump_config() override;
  // Antes que web_server (WIFI-1) para poder vetar peticiones antes de que él las atienda.
  float get_setup_priority() const override { return setup_priority::WIFI - 0.5f; }

  bool canHandle(AsyncWebServerRequest *request) const override;
  void handleRequest(AsyncWebServerRequest *request) override;

 protected:
  web_server_base::WebServerBase *base_;
  const uint8_t *data_{nullptr};
  size_t size_{0};
};

}  // namespace esphome::jk_panel
