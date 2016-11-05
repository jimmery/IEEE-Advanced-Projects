#include <printf.h>
#include <RF24.h>
#include <nrf24L01.h>

//void radio_init() {
//  setPALevel(RF24_PA_LOW);
//  setPayloadSize(32);
//  setChannel(7);
//  setCRCLength(RF24_CRC_16);
//  setDataRate(RF24_1MPS);
//  openReadingPipe(0xCC);
//  openWritingPipe(0xDC);
//}

RF24 controller(23, 18);

void setup() {
  Serial.begin(115200);
  controller.begin();
  printf_begin();
  // put your setup code here, to run once:
//  controller.write_register(RF_SETUP, 0b00000110);
  delay(1000);
  Serial.println("The delay has finished.");
  controller.setPALevel(RF24_PA_HIGH);
  controller.printDetails();

}

void loop() {
  // put your main code here, to run repeatedly:
  controller.printDetails();
  delay(500);

}
