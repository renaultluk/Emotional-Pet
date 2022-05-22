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
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>

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

// ******* Credentials ******* //

#define WIFI_SSID ""
#define WIFI_PW ""

#define FB_API_KEY ""
#define FB_DATABASE_URL ""
#define FB_STORAGE_BUCKET_ID ""

// ******* Global Variables ******* //

bool touched1 = false;
bool touched2 = false;

int touchQueue[2];

volatile bool dataChanged = false;

// ******* Component Objects ******* //

TFT_eSPI tft = TFT_eSPI();
SPIClass spiSD(HSPI);
MPU6050 mpu;
MAX30105 heartRateSensor;

FirebaseData fbdo;
FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;

// ******* Functions ******* //

// ******* Sensors ******* //

void checkMPUSettings();
void getMPUData();

void touch1Callback();
void touch2Callback();

void heartRateInit();
void getHeartRate();

// ******* Firebase ******* //

void firebaseInit();
void uploadFile(String path);
void downloadFile(String remote_path, String local_path);
void checkFirebaseDataChanged();
void addFriend(int friendID);
void getFriends();
void getAudio(String path);
void postAudio();
void getEmotion();

#endif