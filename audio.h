
#ifndef _AUDIO_H
#define _AUDIO_H

#include "main.h"

// library: esp8266 audio
#include <AudioFileSourceID3.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

//#include "AudioFileSourceSD.h"
//#include "AudioFileSourceID3.h"
//#include "AudioGeneratorMP3.h"
//#include "AudioOutputI2S.h"

#define I2S_MODE I2S_MODE_ADC_BUILT_IN
#define FORMAT_SPIFFS_IF_FAILED true

#include "Wav.h"
#include "I2S.h"

void playAudio(const char* path);
//path of file for playing
void record(const char filename[], const int record_time);
//enter path and record length
void listDir(const char* path);

#endif
