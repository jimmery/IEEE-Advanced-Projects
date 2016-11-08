#include <printf.h>
#include <RF24.h>
#include <nrf24L01.h>

#define led 13

// these are for the teensy. 
//#define CEpin 23
//#define CSpin 21

// these are for the arduino. 
#define CEpin 14
#define CSpin 15


//
//void radio_init() {
//  setPALevel(RF24_PA_LOW);
//  setPayloadSize(32);
//  setChannel(7); //team number
//  setCRCLength(RF24_CRC_16);
//  setDataRate(RF24_1MPS);
//  openReadingPipe(1, 0xDC); //Writing pipe of the Teensy
//  openWritingPipe(0xCC); //Reading pipe of the Teensy
//}

RF24 controller(CEpin, CSpin);
  
void setup() {
  Serial.begin(9600);
  controller.begin();
  printf_begin();
  delay(1000);
  Serial.println("The delay has finished.");
}

void loop() {
  // put your main code here, to run repeatedly:
  controller.setPALevel(RF24_PA_MIN);
  controller.printDetails();
  delay(1000);
  
  controller.setPALevel(RF24_PA_LOW);
  controller.printDetails();
  delay(1000);

  controller.setPALevel(RF24_PA_HIGH);
  controller.printDetails();
  delay(1000);

  controller.setPALevel(RF24_PA_MAX);
  controller.printDetails();
  delay(1000);
}
