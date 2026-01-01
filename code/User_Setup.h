// ============================================
// USER_SETUP.h - Configuration TFT_eSPI
// Pour: Waveshare 1.28" Round LCD (GC9A01)
// ============================================

#define GC9A01_DRIVER

// Pins pour ESP32 DevKit C V4
#define TFT_MOSI  23   // GPIO 23 (VSPI MOSI)
#define TFT_SCLK  18   // GPIO 18 (VSPI SCLK)
#define TFT_CS    5    // GPIO 5  (Chip Select)
#define TFT_DC    16   // GPIO 16 (Data/Command)
#define TFT_RST   4    // GPIO 4  (Reset)
#define TFT_BL    22   // GPIO 22 (Backlight control)

// Configuration SPI
#define SPI_FREQUENCY   27000000  // 27 MHz
#define SPI_READ_FREQUENCY 5000000  // 5 MHz

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_GFXFF
#define SMOOTH_FONT

// Paramètres d'affichage
#define TFT_WIDTH  240
#define TFT_HEIGHT 240
#define TFT_INVERT_1 0
