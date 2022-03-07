#ifndef MAIN_H
#define MAIN_H

// ******* Includes ******* //

#include <TFT_eSPI.h>
#include <MPU6050.h>
#include <FS.h>
#include <SD.h>

#include "face.h"
#include "voice-message.h"

// ******* Finite State Machine ******* //

typedef struct {
  STATE_INIT,
  STATE_FACE,
  STATE_VOICE_MESSAGE,
} mainState_t;

#endif