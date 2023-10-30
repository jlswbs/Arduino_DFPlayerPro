// Drum looper - logistic equation //

/*

 Pot0 = clock/time
 Pot1 = chaos chain
 Pot2 = sample range
 Pot3 = none
 
 Created by JLS 2023

 */
 
#include "DF1201S.h"
#include <SoftwareSerial.h>

#define LED_PIN   13
#define LED_PORT  PORTB
#define LED_BIT   5
#define MAXTEMPO  250   // 250 BPM 16th note
#define MINTEMPO  60    // 60 BPM 16th note
#define MAXADC    1023  // max ADC value
#define MINADC    0     // min ADC value
#define VOLUME    30    // master volume
#define SAMP      10    // number of samples

SoftwareSerial playerSerial(2, 3); // RX, TX
DF1201S player;

  float r;
  float x = 0.1f;

void setup(void){

  pinMode(LED_PIN, OUTPUT);

  playerSerial.begin(115200);
  while(!player.begin(playerSerial)){}
  player.setVol(VOLUME); // 0-30
  player.switchFunction(player.MUSIC);
  player.start();
  player.setPlayMode(player.SINGLE);
  
}

void loop(){

  LED_PORT ^= 1 << LED_BIT;
  
  r = map(analogRead(1), MINADC, MAXADC, 34999, 39999);
  r /= 10000.0f;
  
  float nx = x;
  x = r * nx * (1.0f - nx);

  uint8_t range = map(analogRead(2), MINADC, MAXADC, 1, SAMP);
  uint8_t xout = ceil(range * x);

  LED_PORT ^= 1 << LED_BIT;
  
  player.playFileNum(xout);
  
  uint16_t tempo = map(analogRead(0), MINADC, MAXADC, MINTEMPO, MAXTEMPO);
  uint16_t delay_ms = 60000 / tempo;
    
  delay(delay_ms / 2);
  
}
