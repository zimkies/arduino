#include <FastLED.h>
#include <Button.h>

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
#define BUTTON_MODE_IN_PIN 3     // The PWM input for button mode


#define COLOR_ORDER RGB
#define LED_TYPE WS2811        // i'm using WS2811s, FastLED supports lots of different types.

int currentMode = 0;
#define MODE_COUNTS 2

// Define the array of leds
CRGB leds[NUM_LEDS];
Button modeButton(BUTTON_MODE_IN_PIN, 100); // connect our button

void setup() {
  delay( 3000 ); // power-up safety delay

  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(MAX_BRIGHTNESS);
  modeButton.begin();
}


// Pattern 1 variables
static bool forward = 1;
static uint8_t hue = 0;
static uint8_t i = 0;

void loop() {
  calibrateBrightness();

  int patternMode = getPatternMode();

  // Pattern 0 (zoom)
  if (patternMode == 0) {
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

  // Pattern 1 (Nothing)
  if (patternMode == 1) {
    FastLED.clear();
    FastLED.show();

  }

  // Pattern 2
}


int getPatternMode() {
  if (modeButton.pressed()) {
    Serial.println("Button 1 pressed");
    currentMode = (currentMode + 1) % MODE_COUNTS;
  }
  return currentMode;
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void calibrateBrightness() {
  int brightness = getBrightnessValue();
  Serial.print("brightness");
  Serial.print(brightness);
  Serial.print("\n");
  FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

}
int getBrightnessValue() {
  // read the analog brightness value:
  // map it to the range of the FastLED brightness:
  return map(analogRead(BRIGHTNESS_IN_PIN), 0, 1023, 0, 255);
}
