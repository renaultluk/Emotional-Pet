#include "main.h"

Face face;
int curr_time;
int prev_time;
int curr_blink;
int prev_blink;

int test_emotion = 0;

bool btnPress = false;

void btnCallback() {
  btnPress = true;
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.initDMA();
  tft.startWrite();

  // checkMPUSettings();
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

  tft.fillScreen(TFT_WHITE);
  // face.changeFaceState(SAD);
  // face.draw();
}

void loop() {
  curr_time = millis();
  curr_blink = millis();

  // face.update();

  // if (btnPr)

  if (curr_time - prev_time > (1000 / FRAME_RATE)) {
    if (curr_blink - prev_blink > 5000) {
      face.changeFaceState(BLINK);
      prev_blink = curr_blink;
    }
    face.draw(0);
    face.draw(1);
    prev_time = curr_time;
  }
}