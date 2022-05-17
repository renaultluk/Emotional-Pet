// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define tilt_value 5   // accelerometer tilt threshold (in ms^-2)

bool tilt_ready; //boolean to prevent keep creating other input unless it's change stated
bool tilt_center;
bool shake;
bool tilt_left;
bool tilt_right;
bool tilt_up;
bool tilt_down;

Adafruit_MPU6050 mpu;

void setup(void) {
  int run_counter = 0;
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Try to initialize!
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

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Start setting flags */
  
  if ((a.acceleration.x < tilt_value) && (a.acceleration.x > -tilt_value)&&(a.acceleration.y > -tilt_value)&&(a.acceleration.y < tilt_value)){
    tilt_center = true;
    tilt_right = false;
    tilt_left = false;
    tilt_up = false;
    tilt_down = false;
    tilt_ready = true;
  }
  if ((a.acceleration.x > tilt_value) && (tilt_ready == true)){
    tilt_right = true;
    tilt_center = false;
    tilt_ready = false;
    
  }
  if ((a.acceleration.x < -tilt_value) && (tilt_ready == true)){
    tilt_left = true;
    tilt_center = false;
    tilt_ready = false;
  }
  if ((a.acceleration.y > tilt_value) && (tilt_ready == true)){
    tilt_up = true;
    tilt_center = false;
    tilt_ready = false;
  }
  if ((a.acceleration.y < -tilt_value) && (tilt_ready == true)){
    tilt_down = true;
    tilt_center = false;
    tilt_ready = false;
  }

}
