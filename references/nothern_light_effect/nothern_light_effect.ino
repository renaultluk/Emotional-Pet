// A non-blocking everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 8

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 0;       // Current Pattern Number
int           patternInterval = 5000;   // Pattern Interval (ms)
int           pixelInterval = 50;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
uint16_t      pixelCurrent = 0;         // Pattern Current Pixel Number
uint16_t      pixelNumber = LED_COUNT;  // Total Number of Pixels

// setup() function -- runs once at startup --------------------------------
void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop(){
  colorWipe2(255,0,0);
}

// LED_emotion for wiping in 1 color then turn off in wipe motion, it's for emotion state only (Angry, Happy etc, not include idle)
void colorWipe2(int r, int g, int b) {
  unsigned long currentMillis = millis();                     //  Update current time
  if((currentMillis - patternPrevious) >= patternInterval) {  //  Check for expired time
    patternPrevious = currentMillis;
    patternCurrent++;                                         //  Advance to next pattern
    if(patternCurrent >= 2)
      patternCurrent = 0;
  }
  
  if(currentMillis - pixelPrevious >= pixelInterval) {        //  Check for expired time
    pixelPrevious = currentMillis;                            //  Run current frame
    switch (patternCurrent) {
      case 1:
        colorWipe(strip.Color(r, g, b), 50); // RGB value wipe
        break;        
      default:
        colorWipe(strip.Color(0, 0, 0), 50); // clear wipe the strip
        break;
    }
  }
}
void RedLEDfade() {
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  unsigned long currentMillis = millis();
  if(currentMillis - pixelPrevious >= pixelInterval) {        //  Check for expired time
    pixelPrevious = currentMillis;                            //  Run current frame 
    
    for(int i=0; i<strip.numPixels(); i++) {                     // For each pixel in strip...
      strip.setPixelColor(i, strip.Color(brightness, 0, 0));     //  Set RED
      strip.show();                                              //  Update strip to match
    }
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
    }
  }
}
void GreenLEDfade() {
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  unsigned long currentMillis = millis();
  if(currentMillis - pixelPrevious >= pixelInterval) {        //  Check for expired time
    pixelPrevious = currentMillis;                            //  Run current frame 
    
    for(int i=0; i<strip.numPixels(); i++) {                     // For each pixel in strip...
      strip.setPixelColor(i, strip.Color(0, brightness, 0));     //  Set RED
      strip.show();                                              //  Update strip to match
    }
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
    }
  }
}
void BlueLEDfade() {
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5;    // how many points to fade the LED by
  unsigned long currentMillis = millis();
  if(currentMillis - pixelPrevious >= pixelInterval) {        //  Check for expired time
    pixelPrevious = currentMillis;                            //  Run current frame 
    
    for(int i=0; i<strip.numPixels(); i++) {                     // For each pixel in strip...
      strip.setPixelColor(i, strip.Color(0, 0, brightness));     //  Set RED
      strip.show();                                              //  Update strip to match
    }
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
    }
  }
}


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  if(pixelInterval != wait)
    pixelInterval = wait;                   //  Update delay time
  strip.setPixelColor(pixelCurrent, color); //  Set pixel's color (in RAM)
  strip.show();                             //  Update strip to match
  pixelCurrent++;                           //  Advance current pixel
  if(pixelCurrent >= pixelNumber)           //  Loop the pattern from the first LED
    pixelCurrent = 0;
}
