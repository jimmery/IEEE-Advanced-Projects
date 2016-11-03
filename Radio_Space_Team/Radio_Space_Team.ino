#include <printf.h>
#include <RF24.h>
//#include <nrf24l01p.h>

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
  printf_begin();
  //RF24 controller(23, 18);
  // put your setup code here, to run once:
  controller.setPALevel(RF24_PA_LOW);
  controller.printDetails();

}

void loop() {
  // put your main code here, to run repeatedly:
  controller.printDetails();
  delay(500);

}
