
/* mover

By: Andrew Tuline

Date: February 2015

This is a simple pixel moving routine for those of you that just like to count pixels on a strand.
I prefer to use sine waves as they provide a LOT of flexibility with less code for moving pixels around.

You should be able to add more variables, such as hues, hue rotation, direction and so forth.

*/


#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif 

// Fixed definitions cannot change on the fly.
#define LED_DT D8                                             // Data pin to connect to the strip.
#define LED_CK D4
#define COLOR_ORDER RGB                                       // Are they RGB, GRB or what??
#define LED_TYPE WS2801                                       // Don't forget to change LEDS.addLeds
#define NUM_LEDS 480                                          // Number of LED's.
#define FRAMES_PER_SECOND  60

// Initialize changeable global variables.
uint8_t max_bright = 150;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.


// Define variables used by the sequence.

uint8_t thisdelay = 0;                                      // A delay value for the sequence(s).
uint8_t  thisfade = 20;                                      // How quickly does it fade? Lower = slower fade rate.


void setup() {
  delay(1000);                                                // Power-up safety delay or something like that.
  Serial.begin(57600);

//  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);        // Use this for WS2812B
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 9000);               // FastLED power management set at 5V, 500mA.
} // setup()


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
SimplePatternList gPatterns = {rainbowWithGlitter};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  //EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_solid( leds, NUM_LEDS, CRGB(255,0,0)); 
  //fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(480);
  
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}


