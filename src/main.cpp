#include <Arduino.h>
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include <RTClib.h>
#include <sound/day/falcon1.h>

#define Bit_Clock_BCLK 27
#define Word_Select_WS 26
#define Serial_Data_SD 25
#define GAIN 0.700

RTC_DS3231 rtc;

AudioFileSourcePROGMEM *in;
AudioGeneratorWAV *wav;
AudioOutputI2S *out;

const unsigned long interval = 30000;  

unsigned char* daySounds[] = { falcon_1_wav };
const int numDaySounds = sizeof(daySounds) / sizeof(daySounds[0]);

void play();
unsigned char* getRandomSound();

void setup() {
  Serial.begin(115200);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
  audioLogger = &Serial;
  play();
  
}

void loop() {
  if (wav->isRunning()) {
    if(!wav->loop()){
      wav->stop();
    }
    return;
  } 
    
  DateTime now = rtc.now();
  int currentHour = now.hour();

  if (currentHour >= 6 && currentHour < 23) {
    delay(interval);
    play();
  }
}

unsigned char* getRandomSound() {
  esp_random(); 
  int randomIndex = esp_random() % numDaySounds; 
  return daySounds[randomIndex];
}

void play() {
  delete wav, in, out;
  
  in = new AudioFileSourcePROGMEM(falcon_1_wav, sizeof(falcon_1_wav));
  wav = new AudioGeneratorWAV();
  out = new AudioOutputI2S();
  out->SetPinout(Bit_Clock_BCLK,Word_Select_WS,Serial_Data_SD);
  out->SetGain(GAIN);
  wav->begin(in, out);
}