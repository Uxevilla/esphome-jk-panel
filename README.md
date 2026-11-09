# esphome-jk-panel

Panel web (`/panel`) para ESPHome pensado para BMS JK (LiFePO4) con
[`syssi/esphome-jk-bms`](https://github.com/syssi/esphome-jk-bms), más un paquete (`bms-unit.yaml`)
con la definición de entidades por BMS.

- `components/jk_panel`: componente que sirve un HTML incrustado (gzip) en `/panel`, respeta la
  autenticación de `web_server` y **veta** (403/500) escrituras REST sobre entidades cuyo nombre
  contiene ` ro ` (ESPHome no filtra las entidades internas en su API REST).
- `bms-unit.yaml`: paquete parametrizado (`id`, `label`, `mac`, `rated_amps`).

```yaml
external_components:
  - source: github://syssi/esphome-jk-bms@main
  - source: github://Uxevilla/esphome-jk-panel@main
    components: [jk_panel]

jk_panel:

packages:
  bms1:
    url: https://github.com/Uxevilla/esphome-jk-panel
    ref: main
    files:
      - path: bms-unit.yaml
        vars: {id: s15p, label: bateria1, mac: "AA:BB:CC:DD:EE:FF", rated_amps: 150}
```
