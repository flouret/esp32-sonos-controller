# ESP32 Sonos Controller

Pilotez votre enceinte Sonos avec ESP32 Dev Kit C V4, écran LCD Waveshare 1.28" et encodeur rotatif KY-040.

## 🚀 Démarrage Rapide

1. Configurer SSID/Password dans `sonos_controller.ino` (lignes 20-21)
2. Charger le code sur ESP32
3. Moniteur Série (115200 baud) : vérifier "✓ WiFi Connected!" et "✓ Sonos found"
4. Tourner encodeur → Volume Sonos change ✅

## 📋 Fichiers

- `sonos_controller.ino` - Code principal
- `User_Setup.h` - Configuration TFT_eSPI
- `QUICK_START.md` - Guide 5 étapes
- `SONOS_CONTROL_GUIDE.md` - Documentation technique
- `SONOS_SETUP_CONFIG.md` - Configuration détaillée + dépannage
- `WIRING_DIAGRAM.md` - Schéma de brochage

## 🔧 Prérequis

- ESP32 Dev Kit C V4
- Waveshare 1.28" Round LCD
- KY-040 Rotary Encoder
- Enceinte Sonos (Play:1, One, Beam, Arc, etc.)
- Arduino IDE 1.8.19+
- Bibliothèques: TFT_eSPI, AiEsp32RotaryEncoder

## 📊 Fonctionnalités

✅ Contrôle volume (0-100%)
✅ Play/Pause toggle
✅ Next/Previous track
✅ Mute/Unmute
✅ Découverte automatique Sonos (SSDP)
✅ Affichage LCD temps réel
✅ WiFi auto-reconnect

## 📖 Documentation

Voir `QUICK_START.md` pour démarrer (5 min)
Voir `SONOS_SETUP_CONFIG.md` pour troubleshooting

## 📄 Licence

Open source - utilisation libre, modification autorisée
