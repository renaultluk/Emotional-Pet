#include "main.h"

Face face;
int curr_time;
int prev_time;
int curr_blink;
int prev_blink;

void setup() {
  Serial.begin(115200);
  tft.init();

  // checkMPUSettings();
  // heartRateInit();

  // touchAttachInterrupt(TOUCH_PIN1, touch1Callback, TOUCH_THRESHOLD);
  // touchAttachInterrupt(TOUCH_PIN2, touch2Callback, TOUCH_THRESHOLD);

  prev_time = millis();
  curr_time = millis();
  prev_blink = millis();
  curr_blink = millis();

  tft.fillScreen(TFT_WHITE);
  // face.draw();
}

void loop() {
  curr_time = millis();
  curr_blink = millis();
  if (curr_time - prev_time > 1000 / FRAME_RATE) {
    if (curr_blink - prev_blink > 3000 / FRAME_RATE) {
      face.changeFaceState(BLINK);
      prev_blink = curr_blink;
    }
    face.update();
    face.draw();
    prev_time = curr_time;
  }
}