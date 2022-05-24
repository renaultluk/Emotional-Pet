#ifndef MAIN_H
#define MAIN_H

// ******* Includes ******* //

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <MPU6050.h>
#include <FS.h>
#include "SD.h"
#include <JPEGDecoder.h>
#include <MAX30105.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>

#include "face.h"
#include "userFile.h"
#include "audio.h"

using namespace std::__cxx11;

// ******* Finite State Machine ******* //

typedef enum {
  STATE_INIT,
  STATE_FACE,
  STATE_MENU,
  STATE_VOICE_MESSAGE,
} screenState_t;

// ******* Pins ******* //

#define SCL_PIN           0

#define DISPLAY_MISO      0
#define DISPLAY_MOSI      0
#define DISPLAY_CS        0

#define SD_MISO           0
#define SD_MOSI           0
#define SD_CS             0

#define LED_R_PIN         0
#define LED_G_PIN         0
#define LED_B_PIN         0

#define HEART_RATE_SDA    0
#define SPEAKER_PIN       0

#define TOUCH_PIN1        0
#define TOUCH_PIN2        0

#define tilt_value 5   // accelerometer tilt threshold (in ms^-2)

// ******* Constants ******* //

#define TOUCH_THRESHOLD   40
#define HOLD_THRESHOLD    500

#define FRAME_RATE      24

// ******* Global Variables ******* //

// Touch Pads
bool touched1 = false;
bool touched2 = false;

int touchBuffer = 0;

bool swipe_up = false;
bool swipe_down = false;
bool tapped = false;
bool touch_hold = false;
bool hold_finish = false;

int start_touch_time;

// IMU
bool tilt_ready; //boolean to prevent keep creating other input unless it's change stated
bool tilt_center;
bool shake;
bool tilt_left;
bool tilt_right;
bool tilt_up;
bool tilt_down;
bool idle;
int idle_count;

UIElGroup* currScreen = nullptr;
UIElGroup* screenStack[3];
int stackNum = 0;

// Heart Rate Sensor

enum str_conditions { STRESSED , RELAXED };
int menuChoice = 0;

// Audio
String replyTarget;
String audioTarget;


// ******* Component Objects ******* //

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr[2] = {TFT_eSprite(&tft), TFT_eSprite(&tft)};
uint16_t* sprPtr[2];
SPIClass spiSD(HSPI);
MAX30105 heartRateSensor;
Adafruit_MPU6050 mpu;
Adafruit_DRV2605 drv;
Face face;

// ******* Functions ******* //

void mainControlFlow();


// ******* Sensors ******* //

void MPUInit();
void checkMPU();

void touch1Callback();
void touch2Callback();
void checkTouch();

void heartRateInit();
void stressCheckUp();

// ******* Actuators ******* //

void hapticInit();
void PlayHaptic(int effect);

void playAudio(const char* path);

#endif