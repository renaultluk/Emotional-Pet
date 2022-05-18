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

  anim_iterator = 0;
  anim_time = 0;

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
    if (curr_blink - prev_blink > 6000) {
      // face.changeFaceState(BLINK);
      if (testCounter == 0) face.changeFaceState(BLINK);
      else if (testCounter == 1) {
        face.changeFaceState(SAD);
        face.changeFaceState(NEUTRAL);
      }
      else if (testCounter == 2) {
        face.changeFaceState(HAPPY);
        face.changeFaceState(NEUTRAL);
      }
      else if (testCounter == 3) {
        face.changeFaceState(MENU);
      }
      else if (testCounter == 4) {
        face.menu.navigateTo("listen", "audio");
      }
      else if (testCounter == 5) {
        face.menu.navigateTo("listen", "choose");
      }
      else if (testCounter == 6) {
        face.menu.navigateTo("record", "main");
      }
      else if (testCounter == 7) {
        face.menu.navigateTo("record", "record");
      }
      else if (testCounter == 8) {
        face.menu.navigateTo("settings", "main");
      }
      else if (testCounter == 9) {
        face.menu.setVisible(false);
        face.menu.navigateTo("listen", "main");
        face.changeFaceState(NEUTRAL);
      }

      prev_blink = curr_blink;
      testCounter = (testCounter + 1) % 10;
    }
    face.draw(0);
    face.draw(1);
    prev_time = curr_time;
  }
}