#include "audio.h"

AudioGeneratorWAV *wav;
AudioFileSourceSD *file;
AudioOutputI2S *out;

void playAudio(const char* path) {
  File sd = SD.open(path);
  file = new AudioFileSourceSD(path);
//  id3 = new AudioFileSourceID3(file);
  out = new AudioOutputI2S(0, 1);
  wav = new AudioGeneratorWAV();
  wav->begin(file, out);
  sd.close();
  
  while(1){
    if (wav->isRunning()) {
      if (!wav->loop()) {
        wav->stop(); 
        out->stop();
      }
    } else {
      Serial.printf("WAV done\n");
      delay(100);
      break;
    }
  }
}
