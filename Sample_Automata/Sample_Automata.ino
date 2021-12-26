// Sample looper mangler - cellular automata //

/*

 Pot0 = clock/time
 Pot1 = chain sample time
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
#define WIDTH     180

SoftwareSerial playerSerial(2, 3); // RX, TX

DFRobot_PLAY player;

  bool state[WIDTH];
  bool newst[WIDTH];
  bool rules[8] = {0, 1, 1, 1, 1, 0, 0, 0};
  bool flip = false;

void setup(void){

  pinMode(LED_PIN, OUTPUT);

  playerSerial.begin(115200);
  while(!player.begin(playerSerial)){}
  player.setVol(VOLUME); // 0-30
  player.switchFunction(player.MUSIC);
  player.start();

  for (int x=0; x<WIDTH; x++) state[x] = rand()%2;
  
}

void loop(){

  memset (newst, 0, sizeof(newst));

  for (int x=0; x<WIDTH; x++) {     
    uint8_t k = 4*state[(x-1+WIDTH)%WIDTH] + 2*state[x] + state[(x+1)%WIDTH];
    newst[x] = rules[k];
  }

  memcpy (state, newst, sizeof(state));

  uint8_t value = 0;
  uint8_t va = player.getTotalTime();
  
  for (int x = 0; x < va; x++) value = value + state[x];

  LED_PORT ^= 1 << LED_BIT;

  uint16_t mode = analogRead(2);

  if(mode == 0) player.setPlayMode(player.SINGLECYCLE);
  else if(mode > 0 && mode < 512){
    player.setPlayMode(player.SINGLECYCLE); player.next();
  } else player.setPlayMode(player.RANDOM);

  if (flip == true && analogRead(3) > 0) player.fastReverse(0);

  player.setPlayTime(map(value*analogRead(1), MINADC, value*MAXADC, 0, player.getTotalTime()));
  
  LED_PORT ^= 1 << LED_BIT;
  
  delay(map(analogRead(0), MINADC, MAXADC, 1, 100 * (player.getTotalTime()/6)));

  flip = !flip;
  
}
