#include <printf.h>
#include <RF24.h>
#include <nrf24L01.h>

// these are for the teensy. 
//#define CEpin 23
//#define CSpin 21

// these are for the arduino. 
#define CEpin 14
#define CSpin 15

RF24* controller;

typedef struct {
  int counter;
} Data;

void radio_init() {
  controller = new RF24(CEpin, CSpin);
  controller->begin();
  
  controller->setPALevel(RF24_PA_LOW);
  controller->setPayloadSize(32);
  controller->setChannel(7);
  controller->setCRCLength(RF24_CRC_16);
  controller->setDataRate(RF24_1MBPS);
  controller->openReadingPipe(1, 0xCC);
  controller->openWritingPipe(0xDC);
}

void setup() {
  radio_init();
  Serial.begin(9600);
  printf_begin();
  //put your setup code here, to run once:
//  controller.write_register(RF_SETUP, 0b00000110);
  delay(1000);
  Serial.println("The delay has finished.");
}

void loop() {
  // put your main code here, to run repeatedly:
  controller->printDetails();
  delay(1000);
}
