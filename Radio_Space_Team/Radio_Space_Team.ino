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

//void radio_init() {
//  setPALevel(RF24_PA_LOW);
//  setPayloadSize(32);
//  setChannel(7);
//  setCRCLength(RF24_CRC_16);
//  setDataRate(RF24_1MPS);
//  openReadingPipe(0xCC);
//  openWritingPipe(0xDC);
//}

RF24 controller(CEpin, CSpin);

void setup() {
  //controller = new RF24(CEpin, CSpin);
  Serial.begin(9600);
  controller.begin();
  printf_begin();
  //put your setup code here, to run once:
//  controller.write_register(RF_SETUP, 0b00000110);
  delay(1000);
  Serial.println("The delay has finished.");
//  controller.setPALevel(RF24_PA_HIGH);
  //controller.printDetails();


  //pinMode(led, OUTPUT);
  //digitalWrite(led, HIGH);
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
