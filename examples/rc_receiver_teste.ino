/*
 * pinagem modulo wireless
 * 
 * Nome  se8r01   UNO
 * 3.3V   1       3.3V
 * GND    2       GND
 * CE     3       7
 * CSN    4       10
 * SCK    5       13
 * MOSI   6       11
 * MISO   7       12
 * IRQ    8       8
 */

#include "se8r01.h"
const int PAYLOAD_WIDTH = 12;
byte rxData[PAYLOAD_WIDTH];

int radioCanal[6]= {512,512,512,512,0,0};

void init_radio() {

  
  if (!init_rf(10, 7, 8, PAYLOAD_WIDTH)) {
    Serial.println("Chip not found!");
    while (1);

  }
  setPower(POWER_5dbm);
  selectTxPipe(0);
  setRtr(15);
  changeMode(RX_MODE);
}

void setup() {
  Serial.begin(9600);
  
  init_radio();
}

void loop() {
  
  recebeRadio();
  delay(40);
}



void recebeRadio(){
  if(digitalRead(8)==LOW) {
    delayMicroseconds(240);  
    byte pipe = 7;
    pipe = getRxData(rxData);
    if (pipe < 7) {
      while ((getStatusReg() & 0xE) != 0xE){
        getRxPayload(rxData, PAYLOAD_WIDTH);  
      }
      radioCanal[0] = (rxData[0] * 256) + rxData[1];
      radioCanal[1] = (rxData[2] * 256) + rxData[3];
      radioCanal[2] = (rxData[4] * 256) + rxData[5];
      radioCanal[3] = (rxData[6] * 256) + rxData[7];
      radioCanal[4] = (rxData[8] * 256) + rxData[9];
      radioCanal[5] = (rxData[10] * 256) + rxData[11];
        
      for (int i = 0;i<6;i++){
        Serial.print( radioCanal[i] );
        Serial.print( " " );
      }
      Serial.println();
    }
  }
}
