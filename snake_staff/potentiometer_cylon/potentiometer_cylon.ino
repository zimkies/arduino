#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 30

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
#define CLOCK_PIN 13
#define MAX_BRIGHTNESS 164      // Thats full on, watch the power!
#define MIN_BRIGHTNESS 5       // set to a minimum of 25%
#define BRIGHTNESS_IN_PIN A0    // The Analog input pin that the brightness control potentiometer is attached to.


#define COLOR_ORDER RGB
#define LED_TYPE WS2811        // i'm using WS2811s, FastLED supports lots of different types.



// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(MAX_BRIGHTNESS);
}


// Pattern 1 variables
static bool forward = 1;
static uint8_t hue = 0;
static uint8_t i = 0;

void loop() {
  int brightness = getBrightnessValue();
  Serial.print("brightness");
  Serial.print(brightness);
  Serial.print("\n");
  FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));


  EVERY_N_MILLISECONDS(10) {
    leds[i] = CHSV(hue++, 255, 255);
    if ( forward) {i++;} else {i--;};
    FastLED.show();
    fadeall();

    if ( i > NUM_LEDS - 1 || i < 0 ) {
      forward = !forward;
    }
  }
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

int getBrightnessValue() {
  // read the analog brightness value:
  // map it to the range of the FastLED brightness:
  return map(analogRead(BRIGHTNESS_IN_PIN), 0, 1023, 0, 255);
}
