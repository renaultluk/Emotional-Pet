#include "audio.h"

AudioGeneratorWAV *wav;
AudioFileSourceSD *fileplay;
AudioOutputI2S *out;
AudioFileSourceID3* id3;

//for recording
const int headerSize = 44;
const int numCommunicationData = 8000;
const int numPartWavData = numCommunicationData/4;
byte header[headerSize];
char communicationData[numCommunicationData];
char partWavData[numPartWavData];

void playAudio(const char* path) {
  File sd = SPIFFS.open(path);
  fileplay = new AudioFileSourceSPIFFS(path);
 id3 = new AudioFileSourceID3(fileplay);
  out = new AudioOutputI2S(0, 1);
  wav = new AudioGeneratorWAV();
  wav->begin(fileplay, out);
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
  // tft.startWrite();
}

void record(const char filename[], const int record_time){
  const int waveDataSize = record_time * 176000;

  
  CreateWavHeader(header, waveDataSize);
  SPIFFS.remove(filename);
  File file = SPIFFS.open(filename, "a");
  if (!file) return;
  file.write(header, headerSize);
  I2S_Init(I2S_MODE, I2S_BITS_PER_SAMPLE_32BIT);
  for (int j = 0; j < waveDataSize/numPartWavData; ++j) {
    I2S_Read(communicationData, numCommunicationData);
    for (int i = 0; i < numCommunicationData/8; ++i) {
      partWavData[2*i] = communicationData[8*i + 2];
      partWavData[2*i + 1] = communicationData[8*i + 3];
    }
    file.write((const byte*)partWavData, numPartWavData);
  }
  file.close();
  Serial.println("finish");
}
