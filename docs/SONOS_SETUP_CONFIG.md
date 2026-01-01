
***

## 📄 FICHIER 7: `SONOS_SETUP_CONFIG.md` (résumé)

```markdown
# 🔧 Configuration & Dépannage

## Prérequis

- Enceinte Sonos alimentée
- Sonos + ESP32 sur même WiFi 2.4 GHz
- UPnP activé dans Sonos app
- Multicast non filtré

## Configuration Sonos App

1. Ouvrir app Sonos
2. Settings → System → Network
3. Vérifier: Multicast Filtering = OFF
4. Vérifier: UPnP = ON

## Configuration ESP32

Modifier dans `sonos_controller.ino`:

```cpp
const char* SSID = "VOTRE_SSID";
const char* PASSWORD = "VOTRE_PASSWORD";
