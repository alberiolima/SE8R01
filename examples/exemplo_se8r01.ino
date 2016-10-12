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

byte mode = RX_MODE;
//byte mode = TX_MODE;

int counter_for_tx = 0;
byte start_ch_for_RPD = 0, width_for_RPD2 = 45;
int tryes_for_test_link = 100;

byte txData[] = { "123456789012" }; //must be min 4 byte (wtf??), max - 32 bytes
const int PAYLOAD_WIDTH = 12;
byte rxData[PAYLOAD_WIDTH]; //size of rx must be the same as tx if not DNPL

void init_radio() {
  if (!init_rf(10, 7, 8, PAYLOAD_WIDTH)) {
    Serial.println("Chip not found!");
    while (1);

  }
  setPower(POWER_5dbm);
  selectTxPipe(0);
  setRtr(15);
  changeMode(mode);
  Serial.println("********************** Radio starting *******************");
}

void setup() {

  Serial.begin(9600);

  init_radio();

}

void loop() {
    switch (mode) {

    case RX_MODE: {
      byte pipe = 7;
      pipe = getRxData(rxData);
      if (pipe < 7) {
        while ((getStatusReg() & 0xE) != 0xE) //while fifo not empty
          getRxPayload(rxData, PAYLOAD_WIDTH); //reading payload from fifo to buffer
        //String strVal((char*) rxData);
        //Serial.print("Received on pipe:");
        //Serial.println(pipe);
        for (int i = 0;i<16;i+=2){
          int numero = rxData[i]*256;
          numero += rxData[i+1];
          Serial.print(numero);
          Serial.print( " " );          
        }
        Serial.println();
        if (pipe == 1) {
          //byte newCh = (byte) strVal.toInt();
          Serial.print("Change channel to:");
          //Serial.println(newCh);
          //setChannel(newCh);
        }
      }
      break;
    }

    case TX_MODE: {
      String(counter_for_tx).getBytes(txData, PAYLOAD_WIDTH);
      char rtr = sendWithAck(txData);
      if (rtr >= 0)
        Serial.println("Success! Data packet delivered!");
      else {
        Serial.println(
            "************************Fail! Data packet may be lost!********************************");
      }

      Serial.print("Rtr: ");
      Serial.print(rtr, DEC);
      Serial.print(" - ");
      Serial.println((readReg(REG_OBSERVE_TX) & 0xF0) >> 4);

      counter_for_tx++;
      delay(500);
      break;
    }
  
  }

}


