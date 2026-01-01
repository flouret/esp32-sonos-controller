# 📘 Configuration Complète ESP32 + LCD + Encoder

## 📋 Table des Matières

1. [Spécifications Matériel](#spécifications)
2. [Installation Bibliothèques](#bibliothèques)
3. [Configuration TFT_eSPI](#tft_espi)
4. [Brochage Détaillé](#brochage)
5. [Test Hardware](#test-hardware)
6. [Troubleshooting](#troubleshooting)

---

## Spécifications

### ESP32 Dev Kit C V4
- **Processeur**: Tensilica Xtensa 32-bit LX6 dual-core
- **Mémoire RAM**: 520 KB SRAM
- **Mémoire Flash**: 4 MB (intégrée)
- **Fréquence**: 240 MHz (max)
- **GPIO**: 25+ broches utilisables
- **SPI**: 3 contrôleurs disponibles (HSPI, VSPI, SPI3)
- **Tension logique**: 3.3V
- **Alimentation**: USB (5V) ou batterie

### Waveshare 1.28" Round LCD
- **Résolution**: 240×240 pixels
- **Type**: GC9A01 driver
- **Interface**: SPI 4-wire (MOSI, SCLK, CS, DC) + RST + BL
- **Couleurs**: RGB 16-bit (65536 couleurs)
- **Tension**: 3.3V
- **Consommation**: ~80 mA (max)
- **Connecteur**: PH2.0 8-pins
- **Fréquence SPI**: jusqu'à 80 MHz

### KY-040 Rotary Encoder
- **Type**: Encodeur optique avec bouton intégré
- **Résolution**: 20 impulsions par rotation (peut varier)
- **Bouton**: Tactile (5V à GND)
- **Pins**: 5 (VCC, GND, CLK, DT, SW)
- **Voltage**: 3.3V ou 5V (nous utilisons 3.3V)

---

## Installation Bibliothèques

### Via Arduino IDE

#### 1. TFT_eSPI (pour l'écran)
