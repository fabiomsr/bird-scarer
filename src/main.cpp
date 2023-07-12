#include <Arduino.h>
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceSD.h"
#include <esp32-hal.h>
#include <ESP32Time.h>

#define Bit_Clock_BCLK 27
#define Word_Select_WS 26
#define Serial_Data_SD 25
#define GAIN 0.900

ESP32Time rtc(3600);

const unsigned long interval = 5000;  

void play();
int getRandomIndex();

void setup() {
  Serial.begin(115200);
  SD.begin();
  rtc.setTime(00, 58, 19, 12, 7, 2023); 

  audioLogger = &Serial;
}

void loop() {
  int currentHour = rtc.getHour(true);

  if (currentHour >= 6 && currentHour < 21) {
    delay(interval);
    play();
  }
}

int getRandomIndex() {
  return random(1, 15);
}

void play() {

  int soundIndex = getRandomIndex();
  String filename = "/bird"+ String(soundIndex) +".wav";

  AudioFileSourceSD *in = new AudioFileSourceSD();
  in->open(filename.c_str());

  AudioGeneratorWAV *wav = new AudioGeneratorWAV();
  AudioOutputI2S *out = new AudioOutputI2S();

  out->SetPinout(Bit_Clock_BCLK,Word_Select_WS,Serial_Data_SD);
  out->SetGain(GAIN);
  wav->begin(in, out);

  while (wav->isRunning()) {
    if (!wav->loop()) {
      wav->stop();
    }
  }

  in->close();

  delete in, wav, out;
}