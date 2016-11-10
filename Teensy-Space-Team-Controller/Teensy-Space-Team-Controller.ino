#include <RF24.h>
#include <printf.h>
// define all the pins we are using. 
#define LED_PIN 13
#define RED_LED 0
#define YEL_LED 1
#define GRN_LED 2

// maximum buffer size, for the purpose of 
#define BUFFER_SIZE 56
uint8_t buf[BUFFER_SIZE];
typedef struct{
  uint8_t buf[28];
  uint8_t turn = 1;
  }Data;

typedef struct{
  uint8_t correct;
  }Check;
  
uint32_t turn = 1;

uint8_t r = 0; 
uint8_t y = 0;
uint8_t g = 0;

Data data;
Check ch;
RF24 controller(23, 21);

void radio_init() {
  controller.setPALevel(RF24_PA_LOW);
  controller.setPayloadSize(32);
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
  
  delay(1000);
  // turn the LED on, so that we know that the Teensy is working. 
  digitalWrite(LED_PIN, HIGH);

  // startup with LEDs off. 
  digitalWrite(RED_LED, LOW);
  digitalWrite(YEL_LED, LOW);
  digitalWrite(GRN_LED, LOW);

  // generate a random seed from noise values in an unused pin. 
  randomSeed(analogRead(5));
}

// chooses a random color, based on a pseudo-random 
// number generator. 
uint8_t random_color()
{
  int n = random(3);
  if (n == 0)
    return 'R';
  if (n == 1)
    return 'G';
  if (n == 2)
    return 'Y';
}

// writes HIGH or LOW (logic_level)
// into the color led (color is a character)
void writeLED(uint8_t color, uint8_t logic_level)
{
  switch(color)
  {
    case 'R': 
      digitalWrite(RED_LED, logic_level);
      break;
    case 'Y': 
      digitalWrite(YEL_LED, logic_level);
      break;
    case 'G':
      digitalWrite(GRN_LED, logic_level); 
      break;
  }
}

void loop() 
{
  buf[turn - 1] = random_color();

  // flashes the sequence of LEDs. 
  for ( uint32_t i = 0; i < turn; i++)
  {
    writeLED(buf[i], HIGH);   
    delay(1000);
    writeLED(buf[i], LOW);
    delay(250);
  }
data.turn = turn;

  for ( int i = 0; i < 28; i++ )
    data.buf[i] = buf[i];
  if(controller.write(&data, sizeof(data)))
    Serial.println("sent");
  for ( int i = 0; i < 28; i++ )
    data.buf[i] = buf[28 + i];
  if(controller.write(&data, sizeof(data)))
    Serial.println("sent");
  
  controller.startListening();
  while ( true )
  {
    if ( controller.available() )
    {
      // wait for presses. 
      controller.read(&ch, sizeof(ch));
      uint8_t rec_val = ch.correct;

      switch(rec_val) 
      {
        case 0: // game over. 
          turn = 1;
          digitalWrite(RED_LED, HIGH);
          delay(2500);
          digitalWrite(RED_LED, LOW);
          delay(250);
          return;
        case 1: // game continue. 
          turn++;
          digitalWrite(GRN_LED, HIGH);
          delay(2500);
          digitalWrite(GRN_LED, LOW);
          delay(250);
          return;
//        case 'R':
//          r = !r;
//          writeLED('R', r);
//          break;
//        case 'Y':
//          y = !y;
//          writeLED('Y', y);
//          break;
//        case 'G':
//          g = !g;
//          writeLED('G', g);
//          break;
        default: 
          // IDK. 
          break;
      }
    }
  }
  controller.stopListening();
}
