# 🔊 Guide Contrôle Sonos

## Protocoles

### SSDP (Découverte)
- Port: UDP 1900
- Multicast: 239.255.255.250:1900
- Rôle: Trouver IP enceinte Sonos

### SOAP (Contrôle)
- Port: TCP 1400
- Format: XML + HTTP POST
- Rôle: Envoyer commandes

## Commandes Disponibles

### Volume
```cpp
setSonosVolume(ip, 0-100);  // 0-100%
