#include "main.h"

#include "audio_model_new.h"




double spectrum[110250];
double processed_audio_data[216];
int predicted_emotion;

void calculateMFCC()
{
  for (int i = 0; i < 216; i++)
  {
    double mean = 0.0;
    for (int coef = 0; coef < 13; coef++)
    {
      mean += GetCoefficient(spectrum, 44100, 48, 512, coef);
    }
    processed_audio_data[i] = mean / 13.0;
  }
}

void MLInit()
{
  ml.begin(audio_model);

  if (!ml.isOk()) {
    Serial.print("ERROR: ");
    Serial.println(ml.getErrorMessage());

    while (true) delay(1000);
  }
}

void predictEmotion()
{
  calculateMFCC();
  ml.predict(processed_audio_data, predicted_emotion);
  // TODO: handle outputted emotion
}