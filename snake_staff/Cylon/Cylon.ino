#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 30 

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
#define CLOCK_PIN 13

bool up_or_down = false;
// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
	Serial.begin(57600);
	Serial.println("resetting");
	LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
	LEDS.setBrightness(84);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void fadeall2() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() { 
	static uint8_t hue = 0;
	Serial.print("x");
	
	EVERY_N_MILLISECONDS(2000) {
  	// First slide the led in one direction
  	for(int i = 0; i < NUM_LEDS; i++) {
  		// Set the i'th led to red 
  		leds[i] = CHSV(hue++, 255, 255);
  		// Show the leds
  		FastLED.show(); 
  		// now that we've shown the leds, reset the i'th led to black
  //		 leds[i] = CRGB::Black;
  		fadeall();
  		// Wait a little bit before we loop around and do it again
  		FastLED.delay(10);
  	}
  	Serial.print("x");
  
  //   delay(3000);
  
  	// Now go in the other direction.  
  	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
  		// Set the i'th led to red 
  		leds[i] = CHSV(hue++, 255, 255);
  		// Show the leds
  		FastLED.show();
  		// now that we've shown the leds, reset the i'th led to black
  //		 leds[i] = CRGB::Black;
  		fadeall();
  		// Wait a little bit before we loop around and do it again
  		FastLED.delay(10);
  //       delay(2000);
  
  	}
	}
  fadeall2();
    FastLED.show();

//  EVERY_N_MILLISECONDS(100) {
////    fadeToBlackBy(leds, NUM_LEDS, 1);
//  fadeall();
//    FastLED.show();
//  }
////   delay(2000);5

 up_or_down = not up_or_down;
// if (up_or_down) {
//   delay(2000);
//
// }
}
