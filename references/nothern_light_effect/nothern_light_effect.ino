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
