
***

## 📄 FICHIER 3: `WIRING_DIAGRAM.md`
```markdown
# 🔌 Schéma de Brochage

## ESP32 ↔ LCD Waveshare 1.28"

| ESP32 GPIO | Fonction | LCD Pin | Connecteur |
|---|---|---|---|
| GPIO 23 | SPI MOSI | DIN | PH2.0-3 |
| GPIO 18 | SPI SCLK | CLK | PH2.0-4 |
| GPIO 5 | SPI CS | CS | PH2.0-5 |
| GPIO 16 | LCD DC | DC | PH2.0-6 |
| GPIO 4 | LCD RST | RST | PH2.0-7 |
| GPIO 22 | LCD BL | BL | PH2.0-8 |
| 3.3V | Power | VCC | PH2.0-1 |
| GND | Ground | GND | PH2.0-2 |

## ESP32 ↔ KY-040 Rotary Encoder

| ESP32 GPIO | Signal | KY-040 |
|---|---|---|
| GPIO 21 | CLK | CLK |
| GPIO 25 | DT | DT |
| GPIO 32 | SW | SW |
| 3.3V | Power | + |
| GND | Ground | - |

## Schéma Physique

