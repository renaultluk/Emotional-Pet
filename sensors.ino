#include "main.h"

void MPUInit() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  tilt_ready = false;
  Serial.println("setup done");
}

void checkMPU() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Start setting flags */
  
  if (idle_count >= 500){
    idle = true;
  }
  if ((a.acceleration.x < tilt_value) && (a.acceleration.x > -tilt_value)&&(a.acceleration.y > -tilt_value)&&(a.acceleration.y < tilt_value)){
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
  if ((a.acceleration.x > tilt_value) && (tilt_ready == true)){
    tilt_right = true;
    tilt_center = false;
    tilt_ready = false;
    idle = false;
    idle_count = 0;
    
  }
  if ((a.acceleration.x < -tilt_value) && (tilt_ready == true)){
    tilt_left = true;
    tilt_center = false;
    tilt_ready = false;
    idle = false;
    idle_count = 0;
  }
  if ((a.acceleration.y > tilt_value) && (tilt_ready == true)){
    tilt_up = true;
    tilt_center = false;
    tilt_ready = false;
    idle = false;
    idle_count = 0;
  }
  if ((a.acceleration.y < -tilt_value) && (tilt_ready == true)){
    tilt_down = true;
    tilt_center = false;
    tilt_ready = false;
    idle = false;
    idle_count = 0;
  }
}

void IRAM_ATTR touch1Callback() {
  touched1 = true;
}

void IRAM_ATTR touch2Callback() {
  touched2 = true;
}

void checkPetting() {
  if (!touched1 && !touched2) {
    touchQueue[0] = 0;
    touchQueue[1] = 0;
  }

  if (touched2) {
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
  Serial.print("R: ");
  Serial.print(heartRateSensor.getRed());
  Serial.print(" IR: ");
  Serial.println(heartRateSensor.getIR());
}

