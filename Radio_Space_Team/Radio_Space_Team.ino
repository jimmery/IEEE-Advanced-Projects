#include <printf.h>
#include <RF24.h>
#include <nrf24L01.h>

#define led 13

// these are for the teensy. 
#define CEpin 23
#define CSpin 21

// these are for the arduino. 
//#define CEpin 14
//#define CSpin 15

typedef struct

void radio_init() {
  controller.setPALevel(RF24_PA_LOW);
  controller.setPayloadSize(32);
  controller.setChannel(7);
  controller.setCRCLength(RF24_CRC_16);
  controller.setDataRate(RF24_1MBPS);
  controller.openReadingPipe(1, 0xCC);
  controller.openWritingPipe(0xDC);
}

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
//  controller.setPALevel(RF24_PA_MIN);
//  controller.printDetails();
//  delay(1000);
//  
//  controller.setPALevel(RF24_PA_LOW);
//  controller.printDetails();
//  delay(1000);
//
//  controller.setPALevel(RF24_PA_HIGH);
//  controller.printDetails();
//  delay(1000);
//
//  controller.setPALevel(RF24_PA_MAX);
//  controller.printDetails();
//  delay(1000);
controller.write(
controller.startListening();
}
