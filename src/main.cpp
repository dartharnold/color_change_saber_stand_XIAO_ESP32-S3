#include <Arduino.h>
#include <Adafruit_NeoPixel.h>  

//Setup the NeoPixel
#define LED_PIN 2
#define NUM_LEDS 24
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Crystal Colors
#define WHITE         0x00FFFFFF    // #FFFFFF
#define RED           0x00FF0000    // #ff0000  
#define PURPLE        0x00FF00FF    // #FF00FF  
#define BLUE          0x0000008B    // #00008B
#define GREEN         0x0000FF00    // #00FF00
#define YELLOW        0x00FFBF00    // #FFBF00

void setup() {
  Serial.begin(11500); // Initialize serial communications with the PC
  for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
    strip.setPixelColor(pixel, WHITE); // Set all pixels to white
  }
  strip.show();  // Turn OFF all pixels ASAP
}

void loop() {
  delay(1000);
  for (int color = 0 ; color < 6; color++) {
    switch (color) {
      case 0:
         Serial.println("Red, Change to Red");
        for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
          strip.setPixelColor(pixel, RED); // Set all pixels to red
        }
        break;
      case 1:
         Serial.println("Purple, Change to Purple");
        for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
          strip.setPixelColor(pixel, PURPLE); // Set all pixels to purple
        }
        break;
      case 2:
         Serial.println("Blue, Change to Blue");
        for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
          strip.setPixelColor(pixel, BLUE); // Set all pixels to blue
        }
        break;
      case 3:
         Serial.println("Greeen, Change to Greeen");
        for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
          strip.setPixelColor(pixel, GREEN); // Set all pixels to Green
        }
        break;
      case 4:
         Serial.println("Yellow, Change to Yellow");
        for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
          strip.setPixelColor(pixel, YELLOW); // Set all pixels to yellow
        }
        break;
      case 5:
         Serial.println("White, Change to White");
        for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
          strip.setPixelColor(pixel, WHITE); // Set all pixels to white
        }
        break;
    }
    strip.show();  // Update the strip with new colors
    delay(1000);   // Wait for a second before changing color again
  }
}