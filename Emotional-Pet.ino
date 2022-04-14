#include "main.h"

void setup() {
  Serial.begin(115200);

  touchAttachInterrupt(TOUCH_PIN1, touch1Callback, TOUCH_THRESHOLD);
  touchAttachInterrupt(TOUCH_PIN2, touch2Callback, TOUCH_THRESHOLD);
}

void loop() {

}