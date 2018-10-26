#include <FastLED.h>
#include <Button.h>

// How many leds in your strip?
#define NUM_LEDS 60

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
#define CLOCK_PIN 13
#define MAX_BRIGHTNESS 164      // Thats full on, watch the power!
#define MIN_BRIGHTNESS 1       // set to a minimum of 25%
#define BRIGHTNESS_IN_PIN 10    // The Analog input pin that the brightness control potentiometer is attached to.
#define BUTTON_MODE_IN_PIN 9     // The PWM input for button mode


#define COLOR_ORDER RGB
#define LED_TYPE WS2811        // i'm using WS2811s, FastLED supports lots of different types.

int currentMode = 0;
#define MODE_COUNTS 3

// Define the array of leds
CRGB leds[NUM_LEDS];
Button modeButton(BUTTON_MODE_IN_PIN, 100); // connect our button



// Pattern 2 pallette globals
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
//

void setup() {
  delay( 3000 ); // power-up safety delay

  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(MAX_BRIGHTNESS);
  modeButton.begin();

  // Pattern 2 initialization
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}


// Pattern 1 variables
static bool zoomForward = 1;
static uint8_t zoomHue = 0;
static uint8_t zoomIndex = 0;

void loop() {
  calibrateBrightness();


  int patternMode = getPatternMode();
  Serial.println("mode: ");
  Serial.println(patternMode);

  switch(patternMode) {
    case 0: nothingPattern(); break;
    case 1: zoomPattern(); break;
    case 2: basicPalettePattern(); break;
  }
}


int getPatternMode() {
  if (modeButton.pressed()) {
    Serial.println("Button 1 pressed");
    currentMode = (currentMode + 1) % MODE_COUNTS;
  }
  return currentMode;
}

// Nothing Pattern
///////////////////////////////////////////
void nothingPattern() {
  // EVERY_N_MILLISECONDS(10) {
    FastLED.clear();
    FastLED.show();
  // }
}


///////////////////////////////////////////


// Zoom back and forth pattern code
///////////////////////////////////////////

void zoomPattern() {
  EVERY_N_MILLISECONDS(10) {
    leds[zoomIndex] = CHSV(zoomHue++, 255, 255);

    if ( zoomForward) {zoomIndex++;} else {zoomIndex--;};
    FastLED.show();
    fadeall();

    if ( zoomIndex > NUM_LEDS - 1 || zoomIndex < 0 ) {
      zoomForward = !zoomForward;
    }
  }
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

///////////////////////////////////////////

// Basic Palette Code Pattern
///////////////////////////////////////////

void basicPalettePattern() {
  EVERY_N_MILLISECONDS(10) {
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
  }
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;

    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;

}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}


///////////////////////////////////////////
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
