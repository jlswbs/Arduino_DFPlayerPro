// DF Player Pro template //

#include "DFRobot_PLAY.h"
#include <SoftwareSerial.h>

SoftwareSerial DF1201SSerial(2, 3);  //RX  TX

DFRobot_PLAY DF1201S;

void setup(void){

  DF1201SSerial.begin(115200);
  while(!DF1201S.begin(DF1201SSerial)){}

  DF1201S.setVol(30); // 0-30

  DF1201S.switchFunction(DF1201S.MUSIC);

  DF1201S.start();
  
  DF1201S.setPlayMode(DF1201S.RANDOM);
  
}

void loop(){

  DF1201S.next();
      
  DF1201S.setPlayTime(rand()%10);
  
  DF1201S.fastForward(rand()%10);

  DF1201S.fastReverse(rand()%10);

  delay(2000);
  
}
