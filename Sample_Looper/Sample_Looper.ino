// Sample looper mangler - manual //

/*

 Pot0 = clock/time
 Pot1 = current sample time
 Pot2 = single/next/random mode
 Pot3 = rewind
 
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

  bool flip = false;

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

  uint16_t mode = analogRead(2);

  if(mode == 0) player.setPlayMode(player.SINGLECYCLE);
  else if(mode > 0 && mode < 512){
    player.setPlayMode(player.SINGLECYCLE); player.next();
  } else player.setPlayMode(player.RANDOM);

  if (flip == true && analogRead(3) > 0) player.fastReverse(0);

  player.setPlayTime(map(analogRead(1), MINADC, MAXADC, 0, player.getTotalTime()));
  
  LED_PORT ^= 1 << LED_BIT;
  
  delay(map(analogRead(0), MINADC, MAXADC, 1, 100 * (player.getTotalTime()/6)));

  flip = !flip;
  
}
