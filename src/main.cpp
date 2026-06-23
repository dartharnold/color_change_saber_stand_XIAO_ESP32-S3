#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ================== PIN DEFINITIONS ==================
#define RX_PIN      43      
#define LED_PIN     2
#define NUM_LEDS    24

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
HardwareSerial rfidSerial(1);

// Crystal Colors
#define WHITE   0x00FFFFFF
#define RED     0x00FF0000
#define PURPLE  0x00FF00FF
#define BLUE    0x0000008B
#define GREEN   0x0000FF00
#define YELLOW  0x00FFBF00

// State tracking
String lastColor = "";
unsigned long lastPrintTime = 0;
unsigned long lastChangeTime = 0;

// Black crystal flicker state
unsigned long flickerPreviousMillis = 0;
const long flickerInterval = 22;      // Fast but not frantic

// ====================== FUNCTIONS ======================
String getCrystalColor(String id) {
  if (id == "30303030303030433333") return "BLACK";
  if (id == "30303030303030433041") return "RED";
  if (id == "30303030303030433046") return "PURPLE";
  if (id == "30303030303030433036") return "BLUE";
  if (id == "30303030303030433043") return "GREEN";
  if (id == "30303030303030433033") return "YELLOW";
  if (id == "30303030303030433030") return "WHITE";
  return "UNKNOWN";
}

enum CrystalType {
    CRYSTAL_UNKNOWN, CRYSTAL_BLACK, CRYSTAL_RED, CRYSTAL_PURPLE,
    CRYSTAL_BLUE, CRYSTAL_GREEN, CRYSTAL_YELLOW, CRYSTAL_WHITE
};

CrystalType getCrystalType(String color) {
    if (color == "BLACK")  return CRYSTAL_BLACK;
    if (color == "RED")    return CRYSTAL_RED;
    if (color == "PURPLE") return CRYSTAL_PURPLE;
    if (color == "BLUE")   return CRYSTAL_BLUE;
    if (color == "GREEN")  return CRYSTAL_GREEN;
    if (color == "YELLOW") return CRYSTAL_YELLOW;
    if (color == "WHITE")  return CRYSTAL_WHITE;
    return CRYSTAL_UNKNOWN;
}

// ====================== SETUP ======================
void setup() {
  Serial.begin(115200);
  delay(2000);
  
  rfidSerial.begin(9600, SERIAL_8N1, RX_PIN, -1);
  
  Serial.println("=== SWGE Kyber Crystal Scanner Ready ===");
  
  strip.setBrightness(220);
  strip.fill(WHITE);
  strip.show();
}

// ====================== LOOP ======================
void loop() {
  // Handle RFID reading
  if (rfidSerial.available() >= 14) {
    if (rfidSerial.read() == 0x02) {
      String tagID = "";
      for (int i = 0; i < 10; i++) {
        byte b = rfidSerial.read();
        tagID += String(b, HEX);
      }
      rfidSerial.read(); // checksum
      rfidSerial.read(); // end

      tagID.toUpperCase();
      String color = getCrystalColor(tagID);

      if ((color != lastColor || (millis() - lastPrintTime > 3000)) && 
          (millis() - lastChangeTime > 800)) {

        Serial.print("Kyber Crystal Detected: ");
        Serial.println(color);

        CrystalType crystal = getCrystalType(color);

        switch (crystal) {
          case CRYSTAL_BLACK:
            Serial.println("Black, Change to Black - Unstable Flicker");
            strip.fill(RED);
            strip.show();
            break;

          case CRYSTAL_RED:    strip.fill(RED); break;
          case CRYSTAL_PURPLE: strip.fill(PURPLE); break;
          case CRYSTAL_BLUE:   strip.fill(BLUE); break;
          case CRYSTAL_GREEN:  strip.fill(GREEN); break;
          case CRYSTAL_YELLOW: strip.fill(YELLOW); break;
          case CRYSTAL_WHITE:  strip.fill(WHITE); break;

          case CRYSTAL_UNKNOWN:
          default:
            Serial.println("Unknown Crystal Detected");
            strip.fill(0);
            break;
        }
        
        strip.show();
        lastColor = color;
        lastPrintTime = millis();
        lastChangeTime = millis();
      }
    }
  }

  // ================== Improved Intense Black Flicker ==================
  if (lastColor == "BLACK") {
    unsigned long currentMillis = millis();
    
    if (currentMillis - flickerPreviousMillis >= flickerInterval) {
      flickerPreviousMillis = currentMillis;
      
      for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
        // Minimum brightness floor to prevent full dark moments
        uint8_t brightness = random(90, 255);
        
        uint32_t flickerRed = strip.Color(brightness, random(0, 45), 0);
        
        // Stronger, more frequent flashes
        if (random(0, 10) == 0) {
          brightness = random(230, 255);
          flickerRed = strip.Color(brightness, random(40, 90), 0);   // Hot orange flashes
        }
        
        strip.setPixelColor(pixel, flickerRed);
      }
      strip.show();
    }
  }

  delay(15);
}