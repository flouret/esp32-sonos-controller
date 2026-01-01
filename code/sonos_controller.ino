// ========================================
// CONTRÔLEUR SONOS - ESP32 + LCD + Encoder
// Piloter une enceinte Sonos via interface LCD
// ========================================

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <TFT_eSPI.h>
#include <AiEsp32RotaryEncoder.h>

// ========== CONFIGURATION RÉSEAU ==========

const char* SSID = "YOUR_SSID";              // À configurer
const char* PASSWORD = "YOUR_PASSWORD";       // À configurer
const int SONOS_PORT = 1400;
const int SSDP_PORT = 1900;
const char* SSDP_MULTICAST = "239.255.255.250";

// ========== CONFIGURATION HARDWARE ==========

TFT_eSPI tft = TFT_eSPI();

#define ROTARY_ENCODER_A_PIN    21
#define ROTARY_ENCODER_B_PIN    25
#define ROTARY_ENCODER_BUTTON_PIN 32
#define ROTARY_ENCODER_VCC_PIN  -1

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(
    ROTARY_ENCODER_A_PIN,
    ROTARY_ENCODER_B_PIN,
    ROTARY_ENCODER_BUTTON_PIN,
    ROTARY_ENCODER_VCC_PIN,
    4
);

// ========== VARIABLES GLOBALES ==========

String sonosIP = "";
int currentVolume = 50;
bool isPlaying = false;
bool isMuted = false;
unsigned long lastCommandTime = 0;
const unsigned long COMMAND_DELAY = 200;

// ========== SETUP ==========

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n╔════════════════════════════════════════════╗");
    Serial.println("║     SONOS CONTROLLER v1.0                 ║");
    Serial.println("║  ESP32 + Waveshare LCD + KY-040 Encoder  ║");
    Serial.println("╚════════════════════════════════════════════╝\n");
    
    // Init TFT
    Serial.println("[TFT] Initializing...");
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    displaySplash("SONOS", "Starting...", TFT_CYAN);
    
    // Init Encoder
    Serial.println("[ENCODER] Initializing...");
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 100, false);
    rotaryEncoder.setAcceleration(75);
    
    delay(500);
    
    // Init WiFi
    displaySplash("WIFI", "Connecting", TFT_YELLOW);
    Serial.println("[WIFI] Connecting to: " + String(SSID));
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✓ WiFi Connected!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        displaySplash("WIFI", "Connected", TFT_GREEN);
        delay(1000);
    } else {
        Serial.println("\n✗ WiFi Connection Failed!");
        displaySplash("ERROR", "WiFi Failed", TFT_RED);
        delay(2000);
        return;
    }
    
    // Discover Sonos
    displaySplash("SONOS", "Discovering", TFT_YELLOW);
    Serial.println("[SONOS] Discovering speakers...");
    
    sonosIP = discoverSonos();
    
    if (sonosIP != "") {
        Serial.println("✓ Sonos found: " + sonosIP);
        displaySplash("SONOS", "Found: " + sonosIP, TFT_GREEN);
        delay(1500);
    } else {
        Serial.println("✗ No Sonos speakers found!");
        displaySplash("ERROR", "No Sonos Found", TFT_RED);
        delay(2000);
        return;
    }
    
    Serial.println("[SYSTEM] Setup complete!");
    displayMainControl();
}

// ========== MAIN LOOP ==========

void loop() {
    if (sonosIP == "") {
        displaySplash("ERROR", "No Sonos", TFT_RED);
        delay(1000);
        return;
    }
    
    if (rotaryEncoder.encoderChanged()) {
        handleEncoderChange();
    }
    
    if (rotaryEncoder.isEncoderButtonClicked()) {
        handleButtonPress();
    }
    
    updateDisplay();
    delay(50);
}

// ========== ENCODER HANDLING ==========

void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}

void handleEncoderChange() {
    int value = rotaryEncoder.readEncoder();
    
    if (millis() - lastCommandTime > COMMAND_DELAY) {
        currentVolume = value;
        setSonosVolume(sonosIP.c_str(), currentVolume);
        lastCommandTime = millis();
    }
}

void handleButtonPress() {
    Serial.println("[BUTTON] Pressed");
    
    if (isPlaying) {
        sonosPause(sonosIP.c_str());
        isPlaying = false;
    } else {
        sonosPlay(sonosIP.c_str());
        isPlaying = true;
    }
    updateDisplay();
}

// ========== SONOS DISCOVERY ==========

String discoverSonos() {
    WiFiUDP udp;
    
    const char* ssdpRequest =
        "M-SEARCH * HTTP/1.1\r\n"
        "HOST: 239.255.255.250:1900\r\n"
        "MAN: \"ssdp:discover\"\r\n"
        "MX: 1\r\n"
        "ST: urn:schemas-upnp-org:device:ZonePlayer:1\r\n"
        "\r\n";
    
    Serial.println("[SSDP] Sending discovery...");
    
    udp.beginMulticast(IPAddress(239, 255, 255, 250), 1900);
    udp.beginPacketMulticast(IPAddress(239, 255, 255, 250), 1900, WiFi.localIP());
    udp.write((const uint8_t*)ssdpRequest, strlen(ssdpRequest));
    udp.endPacket();
    
    unsigned long startTime = millis();
    String foundIP = "";
    
    while (millis() - startTime < 2000) {
        int packetSize = udp.parsePacket();
        
        if (packetSize) {
            char buffer[512];
            int len = udp.read(buffer, 511);
            buffer[len] = 0;
            
            String response = String(buffer);
            int locationIndex = response.indexOf("LOCATION: http://");
            if (locationIndex >= 0) {
                int ipStart = locationIndex + 17;
                int ipEnd = response.indexOf(":", ipStart);
                
                if (ipEnd > ipStart) {
                    foundIP = response.substring(ipStart, ipEnd);
                    Serial.println("[SSDP] Found: " + foundIP);
                    break;
                }
            }
        }
        delay(10);
    }
    
    udp.stop();
    return foundIP;
}

// ========== SONOS SOAP COMMANDS ==========

bool sendSoapCommand(const char* ip, const char* endpoint,
                     const char* soapAction, const String& soapBody) {
    HTTPClient http;
    
    String url = String("http://") + ip + ":" + SONOS_PORT + endpoint;
    
    http.begin(url);
    http.addHeader("Content-Type", "text/xml; charset=\"utf-8\"");
    http.addHeader("SOAPAction", soapAction);
    
    int httpCode = http.POST(soapBody);
    
    if (httpCode == 200) {
        Serial.println("✓ OK");
        http.end();
        return true;
    } else {
        Serial.print("✗ Error: ");
        Serial.println(httpCode);
        http.end();
        return false;
    }
}

bool setSonosVolume(const char* ip, int volume) {
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    
    String soapBody =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
        "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n"
        "<s:Body>\r\n"
        "<u:SetVolume xmlns:u=\"urn:schemas-upnp-org:service:RenderingControl:1\">\r\n"
        "<InstanceID>0</InstanceID>\r\n"
        "<Channel>Master</Channel>\r\n"
        "<DesiredVolume>" + String(volume) + "</DesiredVolume>\r\n"
        "</u:SetVolume>\r\n"
        "</s:Body>\r\n"
        "</s:Envelope>\r\n";
    
    return sendSoapCommand(ip,
        "/MediaRenderer/RenderingControl/Control",
        "urn:schemas-upnp-org:service:RenderingControl:1#SetVolume",
        soapBody);
}

bool sonosPlay(const char* ip) {
    String soapBody =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n"
        "<s:Body>\r\n"
        "<u:Play xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">\r\n"
        "<InstanceID>0</InstanceID>\r\n"
        "<Speed>1</Speed>\r\n"
        "</u:Play>\r\n"
        "</s:Body>\r\n"
        "</s:Envelope>\r\n";
    
    return sendSoapCommand(ip,
        "/MediaRenderer/AVTransport/Control",
        "urn:schemas-upnp-org:service:AVTransport:1#Play",
        soapBody);
}

bool sonosPause(const char* ip) {
    String soapBody =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n"
        "<s:Body>\r\n"
        "<u:Pause xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">\r\n"
        "<InstanceID>0</InstanceID>\r\n"
        "</u:Pause>\r\n"
        "</s:Body>\r\n"
        "</s:Envelope>\r\n";
    
    return sendSoapCommand(ip,
        "/MediaRenderer/AVTransport/Control",
        "urn:schemas-upnp-org:service:AVTransport:1#Pause",
        soapBody);
}

bool sonosNext(const char* ip) {
    String soapBody =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n"
        "<s:Body>\r\n"
        "<u:Next xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">\r\n"
        "<InstanceID>0</InstanceID>\r\n"
        "</u:Next>\r\n"
        "</s:Body>\r\n"
        "</s:Envelope>\r\n";
    
    return sendSoapCommand(ip,
        "/MediaRenderer/AVTransport/Control",
        "urn:schemas-upnp-org:service:AVTransport:1#Next",
        soapBody);
}

bool sonosPrevious(const char* ip) {
    String soapBody =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\">\r\n"
        "<s:Body>\r\n"
        "<u:Previous xmlns:u=\"urn:schemas-upnp-org:service:AVTransport:1\">\r\n"
        "<InstanceID>0</InstanceID>\r\n"
        "</u:Previous>\r\n"
        "</s:Body>\r\n"
        "</s:Envelope>\r\n";
    
    return sendSoapCommand(ip,
        "/MediaRenderer/AVTransport/Control",
        "urn:schemas-upnp-org:service:AVTransport:1#Previous",
        soapBody);
}

// ========== DISPLAY FUNCTIONS ==========

void displaySplash(const char* title, const String& message, uint16_t color) {
    tft.fillScreen(TFT_BLACK);
    
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.drawString(title, 120, 80);
    
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.drawString(message, 120, 130);
}

void displayMainControl() {
    tft.fillScreen(TFT_BLACK);
    
    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(2);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("SONOS CONTROL", 120, 15);
    
    tft.drawLine(20, 35, 220, 35, TFT_CYAN);
    
    updateDisplay();
}

void updateDisplay() {
    tft.fillRect(0, 50, 240, 70, TFT_BLACK);
    
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("Volume", 120, 55);
    
    tft.setTextColor(TFT_GREEN);
    tft.setTextSize(4);
    char volStr[10];
    sprintf(volStr, "%d", currentVolume);
    tft.drawString(volStr, 120, 85);
    
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.drawString("%", 155, 105);
    
    int barWidth = 160;
    int barX = 40;
    int barY = 120;
    int fillWidth = (currentVolume * barWidth) / 100;
    
    tft.drawRect(barX, barY, barWidth, 15, TFT_WHITE);
    tft.fillRect(barX + 1, barY + 1, fillWidth - 2, 13, TFT_GREEN);
    
    tft.fillRect(0, 145, 240, 80, TFT_BLACK);
    
    tft.setTextColor(isPlaying ? TFT_GREEN : TFT_GRAY);
    tft.setTextSize(2);
    tft.setTextDatum(ML_DATUM);
    tft.drawString(isPlaying ? "▶" : "⏸", 40, 160);
    
    tft.setTextColor(isPlaying ? TFT_GREEN : TFT_WHITE);
    tft.setTextSize(1);
    tft.drawString(isPlaying ? "Playing" : "Paused", 70, 160);
    
    tft.setTextColor(isMuted ? TFT_YELLOW : TFT_GRAY);
    tft.setTextSize(2);
    tft.drawString(isMuted ? "🔇" : "🔊", 40, 190);
    
    tft.setTextColor(isMuted ? TFT_YELLOW : TFT_WHITE);
    tft.setTextSize(1);
    tft.drawString(isMuted ? "Muted" : "Normal", 70, 190);
    
    tft.setTextColor(TFT_GRAY);
    tft.setTextSize(1);
    tft.setTextDatum(BC_DATUM);
    tft.drawString("Rotate: Volume | Press: Play/Pause", 120, 235);
}
