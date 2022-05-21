#include "main.h"

void checkMPUSettings()
{
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
  delay(500);
  }

  // mpu.setGyroOffsetX(155);
  // mpu.setGyroOffsetY(15);
  // mpu.setGyroOffsetZ(15);

  // mpu.calibrateGyro();
  // mpu.setThreshold(3);
  
  Serial.println();

  Serial.print(" * Sleep Mode:        ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");

  Serial.print(" * Clock Source:      ");
  switch(mpu.getClockSource())
  {
  case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
  case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
  case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
  case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
  case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
  case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
  case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }

  Serial.print(" * Gyroscope:         ");
  switch(mpu.getScale())
  {
  case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
  case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
  case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
  case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  }

  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu.getGyroOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getGyroOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getGyroOffsetZ());

  Serial.println();
}

void getMPUData() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  
  // Vector rawGyro = mpu.readRawGyro();
  // Vector normGyro = mpu.readNormalizeGyro();

  // Serial.print(" Xraw = ");
  // Serial.print(rawGyro.XAxis);
  // Serial.print(" Yraw = ");
  // Serial.print(rawGyro.YAxis);
  // Serial.print(" Zraw = ");
  // Serial.println(rawGyro.ZAxis);
  // Serial.print(" Xnorm = ");
  // Serial.print(normGyro.XAxis);
  // Serial.print(" Ynorm = ");
  // Serial.print(normGyro.YAxis);
  // Serial.print(" Znorm = ");
  // Serial.println(normGyro.ZAxis);
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Serial.print("accel: (")
  Serial.print(ax);
  Serial.print(", ")
  Serial.print(ay);
  Serial.print(", ");
  Serial.print(az);
  Serial.print(") \t gyro: (");
  Serial.print(gx);
  Serial.print(", ")
  Serial.print(gy);
  Serial.print(", ");
  Serial.print(gz);
  Serial.println(")");

}

void IRAM_ATTR touch1Callback() {
  touched1 = true;
}

void IRAM_ATTR touch2Callback() {
  touched2 = true;
}

void checkPetting() {
  if (!touch1 && !touch2) {
    touchQueue[0] = 0;
    touchQueue[1] = 0;
  }

  if (touch2) {
    if (touchQueue[1] == 1) {
      // petted
    }
  }
}

void heartRateInit() {
  if (!heartRateSensor.begin()) {
    Serial.println("Heart Rate Sensor failed to init");
    while (1);
  }
  heartRateSensor.setup();
}

void getHeartRate() {
  const int ecgPin = 0;
  int upperThreshold = 1500;
  int lowerThreshold = 100;
  int ecgOffset = 8000;
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
  enum str_conditions { STRESSED , RELAXED };
  int menuChoice = 0;

  void setup() {
    Serial.begin(115200);

  }

  void(* resetFunc) (void) = 0;//declare reset function at address 0

  void phq_func (int rmsdd){
    if (rmsdd >= 0 && rmsdd <= 4) Serial.println("Depression severity: none");
    else if (rmsdd >= 5 && rmsdd <= 9) Serial.println("Depression severity: mild");
    else if (rmsdd >= 10 && rmsdd <= 14) Serial.println("Depression severity: moderate");
    else if (rmsdd >= 15 && rmsdd <= 19) Serial.println("Depression severity: moderately severe");
    else if (rmsdd >= 20 && rmsdd <= 27) Serial.println("Depression severity: severe");  
  }

  void gad_func (int rmsdd){
    if (rmsdd >= 0 && rmsdd <= 4) Serial.println("Anxiety level: minimal anxiety");
    else if (rmsdd >= 5 && rmsdd <= 9) Serial.println("Anxiety level: mild anxiety");
    else if (rmsdd >= 10 && rmsdd <= 14) Serial.println("Anxiety level: moderate anxiety");
    else if (rmsdd >= 15) Serial.println("Anxiety level: severe anxiety");
  }


  void loop() { 

    if(start == false) {
      Serial.println("Do you feel (1)positively STRESSED, (2)negatively STRESSED or (3)RELAXED? Please enter the number.");
      while (Serial.available() == 0) {
      }
      menuChoice = Serial.parseInt();
      delay(5000);
      start = true;
    }

    float weight = 0.1;                               
    LPF = (1.0 - weight) * LPF + weight * (analogRead(ecgPin)*500);
    int ecgReading = LPF + ecgOffset; 

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

    //Serial.println(ecgReading);
    //Serial.println(rrInterval);
    /*if (beatsPerMinute >= 60 && beatsPerMinute <= 140){
        float weight = 0.1;                               
        LPF_beat = (1.0 - weight) * LPF_beat + weight * beatsPerMinute;
        Serial.println(LPF_beat);    
    }*/
    if (hrvComplete == true && rmssd < 100) {
      Serial.print("Your HRV reading is: ");
      Serial.println(rmssd);
      float phq_posstr_hrv = -0.07761*rmssd + 14.40413;
      float phq_negstr_hrv = -0.08183*rmssd + 15.12931;
      float phq_rel_hrv = -0.2331*rmssd + 29.3654;
      float gad_posstr_hrv = -0.09276*rmssd + 14.14535;
      float gad_negstr_hrv = -0.09875*rmssd + 15.06781;
      float gad_rel_hrv = -0.1847*rmssd + 24.3324;

      switch (menuChoice) {

        case 1:
        phq_func (phq_posstr_hrv);
        gad_func (gad_posstr_hrv);
        break;

        case 2:
        phq_func (phq_negstr_hrv);
        gad_func (gad_negstr_hrv);
        break;

        case 3:
        phq_func (phq_rel_hrv);
        gad_func (gad_rel_hrv);
        break;
      }

    }
    else if (hrvComplete == true){
      Serial.println("please try not to move and relax");
      Serial.println("taking reading again");
      delay(5000);
      resetFunc();  //call reset
    }
    delay(10);
  }
}

