#include "main.h"

Face face;
int curr_time;
int prev_time;
int curr_blink;
int prev_blink;

int test_emotion = 0;

bool btnPress = false;

int testCounter = 0;

void btnCallback() {
  btnPress = true;
}

void checkNavigation(){
  if (tilt_up){
    face.menu.navigateTo('u'); //(up down left right = u d l r)
    //playAudio(const char* path);
    PlayHaptic(1); //PlayHaptic(effect number)
    }
  if (tilt_down){
    face.menu.navigateTo('d'); //(up down left right = u d l r)
    //playAudio(const char* path);
    PlayHaptic(4); //PlayHaptic(effect number)
    }
  if (tilt_left){
    face.menu.navigateTo('l'); //(up down left right = u d l r)
    //playAudio(const char* path);
    PlayHaptic(27); //PlayHaptic(effect number)
    }
  if (tilt_right){
    face.menu.navigateTo('r'); //(up down left right = u d l r)
    //playAudio(const char* path);
    PlayHaptic(27); //PlayHaptic(effect number)
    }
}

void mainControlFlow()
{
  
}


void setup() {
  Serial.begin(115200);

  MPUInit();
  hapticsInit();
  
  tft.init();
  tft.initDMA();
  tft.startWrite();

  // heartRateInit();

  // touchAttachInterrupt(TOUCH_PIN1, touch1Callback, TOUCH_THRESHOLD);
  // touchAttachInterrupt(TOUCH_PIN2, touch2Callback, TOUCH_THRESHOLD);

  sprPtr[0] = (uint16_t*)spr[0].createSprite(TFT_WIDTH, TFT_HEIGHT / 2);
  sprPtr[1] = (uint16_t*)spr[1].createSprite(TFT_WIDTH, TFT_HEIGHT / 2);

  spr[1].setViewport(0, -TFT_HEIGHT / 2, TFT_WIDTH, TFT_HEIGHT);

  spr[0].setTextDatum(MC_DATUM);
  spr[1].setTextDatum(MC_DATUM);

  prev_time = millis();
  curr_time = millis();
  prev_blink = millis();
  curr_blink = millis();

  anim_iterator = 0;
  anim_time = 0;

  tft.fillScreen(TFT_WHITE);
  // face.changeFaceState(SAD);
  // face.draw();
}

void loop() {
  
}