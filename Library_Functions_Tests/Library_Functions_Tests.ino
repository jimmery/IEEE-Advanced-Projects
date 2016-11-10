#include <printf.h>
#include <RF24.h>
#include <nrf24L01.h>

// these are for the teensy. 
//#define CEpin 23
//#define CSpin 21

// these are for the arduino. 
#define CEpin 14
#define CSpin 15

RF24 controller(14, 15);

typedef struct {
  int counter;
} Data;

Data d; 

void radio_init() {
  controller.begin();
  
  controller.setPALevel(RF24_PA_MIN);
  controller.setPayloadSize(32);
  controller.setChannel(7);
  controller.setCRCLength(RF24_CRC_16);
  controller.setDataRate(RF24_1MBPS);
  controller.openReadingPipe(1, 0xDC);
  controller.openWritingPipe(0xCC);
}

void setup() {
  radio_init();
  Serial.begin(9600);
  printf_begin(); 
  d.counter = 0;
  //put your setup code here, to run once:
//  controller.write_register(RF_SETUP, 0b00000110);
  delay(1000);
  //Serial.println("The delay has finished.");
}

void loop() {
  // put your main code here, to run repeatedly:

  controller.startListening();

  while (!controller.available());
  controller.read(&d, sizeof(d));
  d = (Data)d;
  Serial.println(d.counter);

  controller.stopListening();

  d.counter++;
  while(!controller.write(&d, sizeof(d)));
  Serial.println(d.counter);
  

}
