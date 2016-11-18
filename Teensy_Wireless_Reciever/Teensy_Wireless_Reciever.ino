#include <RF24.h>
#include <printf.h>
// define all the pins we are using. 
#define LED_PIN 13
#define RED_LED 0
#define YEL_LED 1
#define GRN_LED 2

// maximum buffer size, for the purpose of 
#define BUFFER_SIZE 56
//uint8_t buf[BUFFER_SIZE];
//typedef struct{
//  uint8_t buf[28];
//  uint8_t turn = 1;
//  }Data;

  
typedef struct{
  uint16_t battery_lvl;
  uint8_t l_click = 0;
  uint8_t r_click = 0;
  }Check; //6 bytes
  
//Data data;
Check ch;
RF24 controller(23, 21);

void radio_init() {
  controller.setPALevel(RF24_PA_LOW);
  controller.setPayloadSize(4);
  controller.setChannel(7);
  controller.setCRCLength(RF24_CRC_16);
  controller.setDataRate(RF24_1MBPS);
  controller.openReadingPipe(1, 0xCC);
  controller.openWritingPipe(0xDC);
}

void setup() {
  
  // setup the pins as OUTPUT. 
  pinMode(LED_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YEL_LED, OUTPUT);
  pinMode(GRN_LED, OUTPUT);

  // startup the Serial ports. 
  Serial.begin(9600);
  Serial.println("pins have been setup as outputs.");
  controller.begin();
  printf_begin();
  radio_init();
  
  // turn the LED on, so that we know that the Teensy is working. 
  digitalWrite(LED_PIN, HIGH);

  // startup with LEDs off. 
  digitalWrite(RED_LED, LOW);
  digitalWrite(YEL_LED, LOW);
  digitalWrite(GRN_LED, LOW);
}

void loop() 
{
  controller.startListening();
  while ( true )
  {
    if ( controller.available() )
    {
      // wait for presses. 
      controller.read(&ch, sizeof(ch));
      float rec_val = ch.battery_lvl*3.31/1023*(5500+1500)/5500;
      
      if (ch.l_click == 1)
        Serial.println("left click");
      if (ch.r_click == 1)
        Serial.println("right click");
        
      if (rec_val< 3.7)
      {
        digitalWrite(GRN_LED, LOW);
        digitalWrite(YEL_LED, LOW);
        digitalWrite(RED_LED, HIGH);
      }
      else if (3.7<=rec_val && rec_val<3.9)
      {
        digitalWrite(GRN_LED, LOW);
        digitalWrite(YEL_LED, HIGH);
        digitalWrite(RED_LED, LOW);
      }
      else if (3.9<=rec_val)
      {
        digitalWrite(GRN_LED, HIGH);
        digitalWrite(YEL_LED, LOW);
        digitalWrite(RED_LED, LOW);
      }
    }
  }
  controller.stopListening();
}
