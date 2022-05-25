#ifndef MAIN_H
#define MAIN_H

// ******* Includes ******* //

#include <TFT_eSPI.h>
#include <SPI.h>
#include <MPU6050.h>
#include <FS.h>
#include <SD.h>
#include <JPEGDecoder.h>
#include <MAX30105.h>

#include <EloquentTinyML.h>
#include "libmfcc.h"

#include "audio.h"
#include "SDcard.h"

// ******* Finite State Machine ******* //

typedef struct {
  // STATE_INIT,
  // STATE_FACE,
  // STATE_MENU,
  // STATE_VOICE_MESSAGE,
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

// ******* Constants ******* //

#define TOUCH_THRESHOLD   40

#define NUMBER_OF_INPUTS 216
#define NUMBER_OF_OUTPUTS 1
#define TENSOR_ARENA_SIZE 8*1024

// ******* Global Variables ******* //

bool touched1 = false;
bool touched2 = false;

int touchQueue[2];

// ******* Component Objects ******* //

TFT_eSPI tft = TFT_eSPI();
SPIClass spiSD(HSPI);
MPU6050 mpu;
MAX30105 heartRateSensor;
Eloquent::TinyML::TfLite<NUMBER_OF_INPUTS, NUMBER_OF_OUTPUTS, TENSOR_ARENA_SIZE> ml;

// ******* Functions ******* //

// ******* Sensors ******* //

void checkMPUSettings();
void getMPUData();

void touch1Callback();
void touch2Callback();

void heartRateInit();
void getHeartRate();


#endif