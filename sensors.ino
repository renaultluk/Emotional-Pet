#include "main.h"

void checkMPUSettings()
{
  // while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  // {    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
  // delay(500);
  // }

  // // mpu.setGyroOffsetX(155);
  // // mpu.setGyroOffsetY(15);
  // // mpu.setGyroOffsetZ(15);

  // // mpu.calibrateGyro();
  // // mpu.setThreshold(3);
  
  // Serial.println();

  // Serial.print(" * Sleep Mode:        ");
  // Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");

  // Serial.print(" * Clock Source:      ");
  // switch(mpu.getClockSource())
  // {
  // case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
  // case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
  // case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
  // case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
  // case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
  // case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
  // case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  // }

  // Serial.print(" * Gyroscope:         ");
  // switch(mpu.getScale())
  // {
  // case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
  // case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
  // case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
  // case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  // }

  // Serial.print(" * Gyroscope offsets: ");
  // Serial.print(mpu.getGyroOffsetX());
  // Serial.print(" / ");
  // Serial.print(mpu.getGyroOffsetY());
  // Serial.print(" / ");
  // Serial.println(mpu.getGyroOffsetZ());

  // Serial.println();
}

void getMPUData() {
  // int16_t ax, ay, az;
  // int16_t gx, gy, gz;
  
  // // Vector rawGyro = mpu.readRawGyro();
  // // Vector normGyro = mpu.readNormalizeGyro();

  // // Serial.print(" Xraw = ");
  // // Serial.print(rawGyro.XAxis);
  // // Serial.print(" Yraw = ");
  // // Serial.print(rawGyro.YAxis);
  // // Serial.print(" Zraw = ");
  // // Serial.println(rawGyro.ZAxis);
  // // Serial.print(" Xnorm = ");
  // // Serial.print(normGyro.XAxis);
  // // Serial.print(" Ynorm = ");
  // // Serial.print(normGyro.YAxis);
  // // Serial.print(" Znorm = ");
  // // Serial.println(normGyro.ZAxis);
  // mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Serial.print("accel: (")
  // Serial.print(ax);
  // Serial.print(", ")
  // Serial.print(ay);
  // Serial.print(", ");
  // Serial.print(az);
  // Serial.print(") \t gyro: (");
  // Serial.print(gx);
  // Serial.print(", ")
  // Serial.print(gy);
  // Serial.print(", ");
  // Serial.print(gz);
  // Serial.println(")");

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

