// Drum looper - logistic equation //

/*

 Pot0 = clock/time
 Pot1 = chaos chain
 Pot2 =
 Pot3 =
 
 Created by JLS 2021

 */
 
#include "DFRobot_PLAY.h"
#include <SoftwareSerial.h>

#define LED_PIN   13
#define LED_PORT  PORTB
#define LED_BIT   5
#define MAXADC    1023 // max ADC value
#define MINADC    0    // min ADC value
#define VOLUME    30   // master volume

SoftwareSerial playerSerial(2, 3); // RX, TX

DFRobot_PLAY player;

  float r;
  float x = 0.5f;

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
  
  r = map(analogRead(1), MINADC, MAXADC, 3499, 3999);
  r /= 1000.0f;
  
  float nx = x;
  x = r * nx * (1.0f - nx);

  uint8_t xout = 10 * x;
  
  player.playFileNum(xout);
 
  LED_PORT ^= 1 << LED_BIT;
  
  delay(map(analogRead(0), MINADC, MAXADC, 100, 500));
  
}
