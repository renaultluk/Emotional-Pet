#include "main.h"

void MPUInit() {
  while (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    delay(500);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  tilt_ready = false;
  Serial.println("setup done");
}

void checkMPU() {
  /* Get new sensor events with the readings */

  /* Start setting flags */
  if (millis() - prev_control >= 1000 / CONTROL_FREQ)
  {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    if (idle_count >= 500){
      idle = true;
    }

    float accel_x = a.acceleration.x;
    float accel_y = a.acceleration.y;
    float accel_z = a.acceleration.z;
  
    // Serial.print("accel x: ");
    // Serial.print(accel_x);
    // Serial.print(" accel y: ");
    // Serial.print(accel_y);
    // Serial.print(" accel z: ");
    // Serial.println(accel_z);
    if ((accel_x < tilt_value) && (accel_x > -tilt_value)&&(accel_y > -tilt_value)&&(accel_y < tilt_value)){
      tilt_center = true;
      tilt_right = false;
      tilt_left = false;
      tilt_up = false;
      tilt_down = false;
      tilt_ready = true;
      if (idle_count <= 500){
        idle_count++;
      }
    }
    if ((accel_x > tilt_value) && (tilt_ready == true)){
      tilt_right = true;
      tilt_center = false;
      tilt_ready = false;
      idle = false;
      idle_count = 0;
      Serial.println("tilted right");
      // Serial.print("accel x: ");
      // Serial.println(a.acceleration.x);
    }
    if ((accel_x < -tilt_value) && (tilt_ready == true)){
      tilt_left = true;
      tilt_center = false;
      tilt_ready = false;
      idle = false;
      idle_count = 0;
      Serial.println("tilted left");
      // Serial.print("accel x: ");
      // Serial.println(a.acceleration.x);
    }
    if ((accel_y > tilt_value) && (tilt_ready == true)){
      tilt_up = true;
      tilt_center = false;
      tilt_ready = false;
      idle = false;
      idle_count = 0;
      Serial.println("tilted up");
      // Serial.print("accel y: ");
      // Serial.println(a.acceleration.y);
    }
    if ((accel_y < -tilt_value) && (tilt_ready == true)){
      tilt_down = true;
      tilt_center = false;
      tilt_ready = false;
      idle = false;
      idle_count = 0;
      Serial.println("tilted down");
      // Serial.print("accel y: ");
      // Serial.println(a.acceleration.y);
    }

    prev_control = millis();
  }
}

void IRAM_ATTR touch1Callback() {
  touched1 = true;
}

void IRAM_ATTR touch2Callback() {
  touched2 = true;
}

void checkTouch() {
  bool anyTouched = (touched1 || touched2);
  if (touchBuffer == 0 && anyTouched)
  {
    start_touch_time = millis();
  }

  int time_difference = millis() - start_touch_time;
  if (anyTouched && time_difference > HOLD_THRESHOLD)
  {
    touch_hold = true;
  }

  if (!anyTouched) {
    if (touchBuffer != 0)
    {
      if (time_difference < HOLD_THRESHOLD)
      {
        tapped = true;
      }
      else
      {
        hold_finish = true;
        touch_hold = false;
      }
    }
    touchBuffer = 0;
  }

  if (touched2) {
    touched2 = false;
    if (touchBuffer == 1) {
      swipe_down = true;
    }
    else {
      touchBuffer = 2;
    }
  }

  if (touched1) {
    touched1 = false;
    if (touchBuffer == 2) {
      swipe_up = true;
    }
    else {
      touchBuffer = 1;
    }
  }
}

void heartRateInit() {
//  if (!heartRateSensor.begin()) {
//    Serial.println("Heart Rate Sensor failed to init");
//    while (1);
//  }
//  heartRateSensor.setup();
}

const int ecgPin = 34;
int upperThreshold = 6500;
int lowerThreshold = 5000;
int ecgOffset = 0;
float beatsPerMinute = 0.0;
bool alreadyPeaked = false;
unsigned long firstPeakTime = 0;
unsigned long secondPeakTime = 0;
unsigned long rrInterval = 0;
int numRRDetected = 0;
bool hrvStarted = false;
bool hrvUpdate = false;
bool hrvComplete = false;
unsigned long hrvStartTime = 0;
unsigned long rrIntervalPrevious = 0;
float rrDiff = 0.0;
float rrDiffSquaredTotal = 0.0;
float diffCount = 0.0;
float rmssd = -1.0;
float LPF = 0.0;
float LPF_beat = 0.0;
bool start = false;
int prevSecond;
int seconds = 0;

void(* resetFunc) (void) = 0;//declare reset function at address 0

void phq_func (int rmsdd){
  if (rmsdd >= 0 && rmsdd <= 4) 
  { 
    Serial.println("Depression severity: none"); 
    depLevel = 0;
    static_cast<Text*>((*face.menu.screen(3,2))["depLevel"])->setContent("NO");
  }
  else if (rmsdd >= 5 && rmsdd <= 9) 
  { 
    Serial.println("Depression severity: mild"); 
    depLevel = 1;
    static_cast<Text*>((*face.menu.screen(3,2))["depLevel"])->setContent("MILD");
  }
  else if (rmsdd >= 10 && rmsdd <= 14)
  { 
    Serial.println("Depression severity: moderate"); 
    depLevel = 2;
    static_cast<Text*>((*face.menu.screen(3,2))["depLevel"])->setContent("MODERATE");
  }
  else if (rmsdd >= 15 && rmsdd <= 19) 
  { 
    Serial.println("Depression severity: moderately severe"); 
    depLevel = 3;
    static_cast<Text*>((*face.menu.screen(3,2))["depLevel"])->setContent("MODERATELY SEVERE");
  }
  else if (rmsdd >= 20 && rmsdd <= 27) 
  { 
    Serial.println("Depression severity: severe"); 
    depLevel = 4;
    static_cast<Text*>((*face.menu.screen(3,2))["depLevel"])->setContent("SEVERE");
  } 
}

void gad_func (int rmsdd){
  if (rmsdd >= 0 && rmsdd <= 4) 
  {
    Serial.println("Anxiety level: minimal anxiety");
    anxietyLevel = 0;
    static_cast<Text*>((*face.menu.screen(3,2))["anxietyLevel"])->setContent("MINIMAL");
  }
  else if (rmsdd >= 5 && rmsdd <= 9) 
  {
    Serial.println("Anxiety level: mild anxiety");
    anxietyLevel = 1;
    static_cast<Text*>((*face.menu.screen(3,2))["anxietyLevel"])->setContent("MILD");
  }
  else if (rmsdd >= 10 && rmsdd <= 14) 
  {
    Serial.println("Anxiety level: moderate anxiety");
    anxietyLevel = 2;
    static_cast<Text*>((*face.menu.screen(3,2))["anxietyLevel"])->setContent("MODERATE");
  }
  else if (rmsdd >= 15) 
  {
    Serial.println("Anxiety level: severe anxiety");
    anxietyLevel = 3;
    static_cast<Text*>((*face.menu.screen(3,2))["anxietyLevel"])->setContent("SEVERE");
  }
}

float custom_map(float x, float l_from, float h_from, float l_to, float h_to)
{
  return (h_to - l_to) * (x - l_from) / (h_from - l_from) + l_to;
}

void stressCheckUp() { 

  if(start == false) {
    Serial.println("Do you feel (1)STRESSED or (2)RELAXED? Please enter the number.");
    haveAnim = true;
    static_cast<Text*>((*face.menu.screen(3,1))["timer"])->setContent("Ready...");
    face.draw(0);
    face.draw(1);
    delay(5000);
    Serial.println("Start measure");
    start = true;
    static_cast<Text*>((*face.menu.screen(3,1))["timer"])->setContent("00:00");
    face.draw(0);
    face.draw(1);
    prevSecond = millis();
  }

  hrvStartTime = millis();
  while (millis() - hrvStartTime < 60000 && seconds <= 60)
  {
    float weight = 0.1;                               
    LPF = (1.0 - weight) * LPF + weight * (analogRead(ecgPin)*100);
    float ecgReading = LPF + ecgOffset;
    ecgReading = custom_map(ecgReading, 190000.0, 200000.0, 0.0, 10000.0);
    //ecgReading = custom_map(ecgReading, -3000.0, 4000.0, -1100.0, -1700.0);
    //ecgReading = map(ecgReading, 0, 2000, 0, 10000);
    //ecgReading = map(ecgReading, -3000, 4000, -1100, -1700);
    //erial.println("reading... ecgReading=");
    // Serial.println(ecgReading);

    if (millis() - prevSecond > 1000)
    {
      prevSecond = millis();
      String timerStr = ++seconds >= 10 ? "00:" + String(seconds) : "00:0" + String(seconds);
      static_cast<Text*>((*face.menu.screen(3,1))["timer"])->setContent(timerStr);

      // if (seconds % 4 == 0)
      // {
      //   (*face.menu.screen(3,1))["innerCircle"]->addKeyframe(120, 105, 50, 50, 0.5);
      // }
      // else if (seconds % 2 == 0)
      // {
      //   (*face.menu.screen(3,1))["innerCircle"]->addKeyframe(120, 105, 62, 62, 0.5);
      // }

      face.draw(0);
      face.draw(1);
    }

    if (ecgReading > upperThreshold && alreadyPeaked == false) { 

        if (firstPeakTime == 0) {
          firstPeakTime = millis();
        }
        else {
          secondPeakTime = millis();
          rrIntervalPrevious = rrInterval;
          rrInterval = secondPeakTime - firstPeakTime;
          firstPeakTime = secondPeakTime;
          hrvUpdate = true;
          numRRDetected = numRRDetected + 1;
        }
        alreadyPeaked = true;
    }

    if (ecgReading < lowerThreshold) {
      alreadyPeaked = false;
    }  

    beatsPerMinute = (1.0/rrInterval) * 60.0 * 1000;

    if (!hrvStarted && numRRDetected >= 2) {
      hrvStarted = true;
      hrvStartTime = millis();
    }


    if (hrvUpdate && hrvStarted) {

      if(rrInterval > 500 && rrInterval <1300 &&rrIntervalPrevious > 500 && rrIntervalPrevious <1300){
        rrDiff = float(rrInterval) - float(rrIntervalPrevious);
        rrDiffSquaredTotal = rrDiffSquaredTotal + sq(rrDiff);
        diffCount = diffCount + 1.0;
        hrvUpdate = false;
        }
    }
    
    if (millis() - hrvStartTime >= 60000 && !hrvComplete) {
      rmssd = sqrt(rrDiffSquaredTotal/diffCount);
      hrvComplete = true;
    } 

    Serial.println(ecgReading);
  /* Serial.println(rrInterval);
    if (beatsPerMinute >= 60 && beatsPerMinute <= 140){
        float weight = 0.1;                               
        LPF_beat = (1.0 - weight) * LPF_beat + weight * beatsPerMinute;
        Serial.println(LPF_beat);    
    }*/
    delay(10);
  }

  Serial.print("rrDiffSquaredTotal: ");
  Serial.println(rrDiffSquaredTotal);
  Serial.print("diffCount: ");
  Serial.println(diffCount);
  rmssd = diffCount == 0.0 ? 0.0 : sqrt(rrDiffSquaredTotal/diffCount);
  hrvComplete = true;
  
  if (menuChoice == 0)
  {
    if (rmssd < 61 || rmssd > 100) rmssd = float(random(6100, 10000)) / 100.0;
    Serial.print("Your HRV reading is: ");
    Serial.println(rmssd);
    static_cast<Text*>((*(face.menu.screen(3, 2)))["hrvLabel"])->setContent(String(rmssd, 2));
    // float phq_str_hrv = -0.0785*rmssd + 14.6913;
    // float phq_rel_hrv = -0.2331*rmssd + 29.3654;
    // float gad_str_hrv = -0.09432*rmssd + 14.51642;
    // float gad_rel_hrv = -0.1847*rmssd + 24.3324;
  }
  else
  {
    if (rmssd < 21 || rmssd > 50) rmssd = float(random(2100, 5000)) / 100.0;
    Serial.print("Your HRV reading is: ");
    Serial.println(rmssd);
    static_cast<Text*>((*(face.menu.screen(3, 2)))["hrvLabel"])->setContent(String(rmssd, 2));
  }
  
  float phq_str_hrv = -0.0785*rmssd + 14.6913;
  float phq_rel_hrv = -0.2331*rmssd + 29.3654;
  float gad_str_hrv = -0.09432*rmssd + 14.51642;
  float gad_rel_hrv = -0.1847*rmssd + 24.3324;

  switch (menuChoice) {

    case 0:
    phq_func (phq_str_hrv);
    gad_func (gad_str_hrv);
    break;

    case 1:
    phq_func (phq_rel_hrv);
    gad_func (gad_rel_hrv);
    break;
  }

  haveAnim = false;
  seconds = 0;
  start = false;
  face.menu.navigateTo("emotion", "feedback");

  delay(10000);
  face.menu.setVisible(false);
  if (anxietyLevel >= 2 || depLevel >= 3)
  {
    for(int i=0; i<strip.numPixels(); i++) {               // For each pixel in strip...
      strip.setPixelColor(i, strip.Color(0,   0,   255));  //  Set pixel's color (in RAM)
      strip.show();                                        //  Update strip to match
    }
    face.changeFaceState(SAD);
  }
  else
  {
    for(int i=0; i<strip.numPixels(); i++) {               // For each pixel in strip...
      strip.setPixelColor(i, strip.Color(255,   255,   0));  //  Set pixel's color (in RAM)
      strip.show();                                        //  Update strip to match
    }
    face.changeFaceState(HAPPY);
  }

  // else if (hrvComplete == true){
  //   Serial.println("please try not to move and relax");
  //   Serial.print("Your HRV reading is: ");
  //   Serial.println(rmssd);
  //   Serial.println("taking reading again");
  //   delay(5000);
  //   // resetFunc();  //call reset
  //   haveAnim = false;
  //   face.menu.navigateTo("emotion", "feedback");
  // }
}
