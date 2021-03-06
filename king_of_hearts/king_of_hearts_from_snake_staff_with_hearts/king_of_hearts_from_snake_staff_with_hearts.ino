#include <FastLED.h>
#include <Button.h>

// TODO:
// Pick random one each time we start
// Do the slowly rising heart one
// Make the heartbeat more 


// How many leds in your strip?
#define NUM_LEDS 64

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
#define CLOCK_PIN 13
#define MAX_BRIGHTNESS 120      // Thats full on, watch the power!
#define MIN_BRIGHTNESS 30       // set to a minimum of 25%
#define BRIGHTNESS_IN_PIN 10    // The Analog input pin that the brightness control potentiometer is attached to.
#define BUTTON_MODE_IN_PIN 9     // The PWM input for button mode


#define COLOR_ORDER RGB
#define LED_TYPE WS2811        // i'm using WS2811s, FastLED supports lots of different types.

int currentMode = 0;
#define MODE_COUNTS 5;

// Define the array of leds
CRGB leds[NUM_LEDS];
Button modeButton(BUTTON_MODE_IN_PIN, 100); // connect our button



// Pattern 2 pallette globals
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

CRGBPalette16 whitePalette;
CRGBPalette16 firePalette; 
// LavaColors_p already defined

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


  fill_solid(whitePalette, 16, CRGB::White);
//  firePalette = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);

  // Don't ask me why green turns out as red, but it does  
  firePalette = CRGBPalette16( CRGB::White, CRGB::Green, CRGB::Green, CRGB::Green);
  FastLED.clear();
  currentMode = random8() % MODE_COUNTS;

}


// Pattern 1 variables
static bool zoomForward = 1;
static uint8_t zoomHue = 0;
static uint8_t zoomIndex = 0;

void loop() {
  calibrateBrightness();


  int patternMode = getPatternMode();
  // Serial.println("mode: ");
  // Serial.println(patternMode)

    switch(patternMode) {
    //
    case 0: rainbowShimmering(50); break;
    case 1: colorBeams(); break;
    case 2: heartBeat(); break;
    case 3: risingHeart(); break;
    case 4: whiteBeams(); break;
  }

}


int getPatternMode() {
  static uint64_t startTime = millis();
  static uint64_t timeElapsed;
  
  EVERY_N_MILLISECONDS(2005) {
    timeElapsed = millis() - startTime;
    
    if (timeElapsed > getModeCycleTime(currentMode)) {
      currentMode = random8() % MODE_COUNTS;
      FastLED.clear();
      FastLED.show();
      startTime = millis();
      
    }
  }

  return currentMode;

}

uint32_t getModeCycleTime(int mode) {
//  return 1000;
  switch(mode) {
    case 0: return 30000; break;
    case 3: return 30000; break;
    default: return 20000;
   }
}



void risingHeart() {
    static uint8_t maxBrightness = 255;
    static int colorIndex = 100;
    static int currentEndIndex = 0;
    static int HEART_START_OFFSET = 3;
    static int brightness = 0;
    
//    static int brightness = 200;

      
  EVERY_N_MILLISECONDS(200) {
//    brightness = 250;
    for( int i = 0; i < currentEndIndex ; i++) {
        brightness = maxBrightness - 10 * (currentEndIndex - i);
        if (currentEndIndex - i > 35) {
          brightness = 0;
        }

        if (currentEndIndex - i == 1) {
           leds[i + HEART_START_OFFSET] = CRGB::White;
           leds[NUM_LEDS - i - HEART_START_OFFSET -2] = CRGB::White;
        } else {
        leds[i + HEART_START_OFFSET] = ColorFromPalette( currentPalette, colorIndex  , brightness, currentBlending);
        leds[NUM_LEDS - i - HEART_START_OFFSET - 2] = ColorFromPalette( currentPalette, colorIndex , brightness, currentBlending);
        }

//        colorIndex += 1;
    }
    currentEndIndex +=1 ;

    if ( currentEndIndex == NUM_LEDS - HEART_START_OFFSET -3) {
      currentEndIndex =0;
      colorIndex = random8() % 255;
    }
    

//        FastLED.clear();
    FastLED.show();
  }
}


// Nothing Pattern
///////////////////////////////////////////
void nothingPattern() {
  EVERY_N_MILLISECONDS(10) {
    FastLED.clear();
    FastLED.show();
  }
}
///////////////////////////////////////////
void heartBeat() {
  static bool isBeating = true;
  static int colorIndex = 8;
  static int count = 0;
  static int startTime = 0;
  static int timeElapsed = 0;
  static int brightness_low = 180;
  static int brightness_high = 255;
  static int brightness = 0;
  static int hue = 0;

  
  EVERY_N_MILLISECONDS(1800) {
    startTime = millis();
    count = 0;
    hue += 7;
  }


   timeElapsed = millis() - startTime;

   if (timeElapsed > 550 ) {
      if (brightness != 0) {
        brightness--;
      }
   } else if (timeElapsed < 200 || timeElapsed > 450) {
     brightness = brightness_high;
   } else {
     brightness = brightness_low;
   }

  for( int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CHSV(hue, 100, brightness);
  }   
  FastLED.show();
}


// multiColorSingleSparkle Pattern

class Sparkle
{
  public:
    int position;
    int colorIndex;
    int brightness;
    bool forward;
    bool active;
};

void rainbowSingleSparklePattern() {
  singleSparklePattern(RainbowColors_p, LINEARBLEND);
}

void rainbowMultipleSparklePattern() {
  multipleSparklePattern(RainbowColors_p, LINEARBLEND);
}

///////////////////////////////////////////
void singleSparklePattern(CRGBPalette16 colorPalette, TBlendType blending) {
  static bool firstRun = true;
  EVERY_N_MILLISECONDS(5) {
    static Sparkle sparkle;
    if (firstRun) {
      sparkle.active = false;
      firstRun = false;
    }

    EVERY_N_MILLISECONDS(1000) {
      if (not sparkle.active) {
        sparkle.position = random8() % NUM_LEDS;
        sparkle.colorIndex = random8() % 256;
        sparkle.brightness = 0;
        sparkle.forward = true;
        sparkle.active = true;
      }
    }
    // for that pixel, fade in then fade out:
    // If we have a current sparkle, update it
    if (sparkle.active) {
      if (sparkle.brightness >= 123) {
        sparkle.forward = not sparkle.forward;
      }

      if (sparkle.forward) {
        sparkle.brightness++;
      } else {
        sparkle.brightness--;
      }

      leds[sparkle.position] = ColorFromPalette(colorPalette, sparkle.colorIndex, sparkle.brightness, blending);
      FastLED.show();

      // If we've faded out, then deactivate the sparkle.
      if (sparkle.brightness == 0 and not sparkle.forward) {
        sparkle.active = false;
        return;
      }
    }
  }
}

void multipleSparklePattern(CRGBPalette16 colorPalette, TBlendType blending) {
  static bool firstRun = true;
  static int lastColor = random8() % 256;
  const int numSparkles = 23;
  static Sparkle sparkles[numSparkles];
  // On first run make sure that all sparkles are inactive
  if (firstRun) {
    for (int i=0; i < numSparkles; i++) {
      sparkles[i].active = false;
    }

    firstRun = false;
  }

  // Every 500ms, if there is an inactive sparkle, light a new one!
  EVERY_N_MILLISECONDS(500) {
    for (int i=0; i < numSparkles; i++) {
      Serial.print("Looping through sparkles");

      // Sparkle sparkle = sparkles[i];
      if (not sparkles[i].active) {

        Serial.print("initializing sparkles[i]\n");
        sparkles[i].position = random8() % NUM_LEDS;
        lastColor = lastColor + 3;
        sparkles[i].colorIndex = lastColor;
        sparkles[i].brightness = 0;
        sparkles[i].forward = true;
        sparkles[i].active = true;
        break;
      }
    }
  }

  EVERY_N_MILLISECONDS(10) {
    // for any active pixel, fade it in then fade out:
    for (int i=0; i < numSparkles; i++) {
      if (sparkles[i].active) {
        if (sparkles[i].brightness >= 123) {
          sparkles[i].forward = not sparkles[i].forward;
        }

        if (sparkles[i].forward) {
          sparkles[i].brightness++;
        } else {
          sparkles[i].brightness--;
        }

        leds[sparkles[i].position] = ColorFromPalette(colorPalette, sparkles[i].colorIndex, sparkles[i].brightness, blending);
        FastLED.show();

        // If we've faded out, then deactivate the sparkles[i].
        if (sparkles[i].brightness == 0 and not sparkles[i].forward) {
          sparkles[i].active = false;
          return;
        }
      }
    }
  }
}


///////////////////////////////////////////



// Zoom patterns back and forth pattern code
///////////////////////////////////////////

// Zooms back and forth for a copule seconds and then turns off.
void zoomIntervalPattern() {

  EVERY_N_MILLISECONDS(10) {

    uint8_t secondHand = (millis() / 1000) % 60;
    // Only light up if we are in the 2 second interval (or are finishing up)
    if ((secondHand % 10 < 2) or ( zoomIndex < NUM_LEDS - 1 and zoomIndex > 0 )) {
      leds[zoomIndex] = CHSV(zoomHue++, 255, 255);
      if ( zoomForward) {zoomIndex++;} else {zoomIndex--;};

      if ( zoomIndex > NUM_LEDS - 1 || zoomIndex < 0 ) {
        zoomForward = !zoomForward;
      }
    }

    FastLED.show();
    fadeall();

  }
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

//////////////////////////////////////
// Beamns


//////////////////////////////////////

///////////////////////////////////////////

// Basic Palette Code Pattern


void beams(bool useColor) {
  static bool fire_beam = false;
  static int startBlockIndex = 0;
  const int blockLength = 25;
  static int colorIndex = 0;
  const int brightness = 255;
  static const CRGBPalette16 colorPalette = whitePalette;
  static const TBlendType blending = LINEARBLEND;

  EVERY_N_MILLISECONDS(4000) {
    fire_beam = true;
    startBlockIndex = -blockLength;
    colorIndex = random8() % 256;
  }

  EVERY_N_MILLISECONDS(25) {
    // Only light up if we have fire_beam on
    if (fire_beam) {
      for( int i = 0; i < NUM_LEDS + blockLength; i++) {
        if (i < 0 || i > NUM_LEDS - 1) {
          continue;
        }
        if (i >= startBlockIndex and i < startBlockIndex + blockLength) {
          if (useColor) {
            leds[i] = ColorFromPalette( currentPalette, colorIndex + i, brightness, currentBlending);
            


          } else {
            leds[i] = CRGB::White;//ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);

          }
        } else {
          leds[i] = CRGB::Black;
        }
      }
      FastLED.show();
      startBlockIndex++;

      if (startBlockIndex > NUM_LEDS) {
        fire_beam = false;
      }
    }
  }
}

void colorBeams() {
  beams(true);
}

void whiteBeams() {
  beams(false);

}


///////////////////////////////////////////


void rainbowShimmering(int speed) {
  EVERY_N_MILLISECONDS(speed) {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
  }
}


void paletteShimmering(int speed, CRGBPalette16 palette) {
  EVERY_N_MILLISECONDS(speed) {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    currentPalette = palette;
    currentBlending = LINEARBLEND;
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
  }
}

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
        colorIndex += 1;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand == 0)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
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
  // Serial.print("brightness");
  // Serial.print(brightness);
  // Serial.print("\n");
  FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));

}

int getBrightnessValue() {
  // read the analog brightness value:
  // map it to the range of the FastLED brightness:
  return map(analogRead(BRIGHTNESS_IN_PIN), 0, 1023, 0, 255);
}

