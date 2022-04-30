
#ifndef _AUDIO_H
#define _AUDIO_H

// library: esp8266 audio
#include <AudioFileSourceID3.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

//#include "AudioFileSourceSD.h"
//#include "AudioFileSourceID3.h"
//#include "AudioGeneratorMP3.h"
//#include "AudioOutputI2S.h"

void playAudio(const char* path);

#endif
