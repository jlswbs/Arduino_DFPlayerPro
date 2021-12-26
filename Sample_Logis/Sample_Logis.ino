// Sample looper mangler - logistic equation //

/*

 Pot0 = clock/time
 Pot1 = chain sample time
 Pot2 = single/next/random mode
 Pot3 = chain logis
 
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
  
}

void loop(){

  LED_PORT ^= 1 << LED_BIT;
  
  r = map(analogRead(3), MINADC, MAXADC, 35699, 38284);
  r /= 10000.0f;
  
  float nx = x;
  x = r * nx * (1.0f - nx);

  uint16_t mode = analogRead(2);

  if(mode == 0) player.setPlayMode(player.SINGLECYCLE);
  else if(mode > 0 && mode < 512){
    player.setPlayMode(player.SINGLECYCLE); player.next();
  } else player.setPlayMode(player.RANDOM);

  player.setPlayTime(map(analogRead(1)*x, MINADC, MAXADC, 0, player.getTotalTime()));
  
  LED_PORT ^= 1 << LED_BIT;
  
  delay(map(analogRead(0), MINADC, MAXADC, 1, 100 * (player.getTotalTime()/6)));
  
}
