// define all the pins we are using. 
#define LED_PIN 13
#define RED_LED 12
#define YEL_LED 11
#define GRN_LED 10

// maximum buffer size, for the purpose of 
#define BUFFER_SIZE 64
uint8_t buf[BUFFER_SIZE];

uint32_t turn = 0;

uint8_t r = 0; 
uint8_t y = 0;
uint8_t g = 0;

void setup() {
  // setup the pins as OUTPUT. 
  pinMode(LED_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YEL_LED, OUTPUT);
  pinMode(GRN_LED, OUTPUT);

  // startup the Serial ports. 
  Serial.begin(9600);
  // startup pins 0 and 1 as Serial ports. 
  Serial1.begin(9600);
  
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
  buf[turn] = random_color();

  // flashes the sequence of LEDs. 
  for ( uint32_t i = 0; i <= turn; i++)
  {
    writeLED(buf[i], HIGH);   
    delay(1000);
    writeLED(buf[i], LOW);
    delay(250);
  }

  Serial1.write(buf, BUFFER_SIZE);

  while ( true )
  {
    if ( Serial1.available() )
    {
      // wait for presses. 
      uint8_t rec_val = Serial1.read();

      switch(rec_val) 
      {
        case 0: // game over. 
          turn = 0;
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
        case 'R':
          r = !r;
          writeLED('R', r);
          break;
        case 'Y':
          y = !y;
          writeLED('Y', y);
          break;
        case 'G':
          g = !g;
          writeLED('G', g);
          break;
        default: 
          // IDK. 
          break;
      }
    }
  }
}
