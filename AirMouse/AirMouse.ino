#include <printf.h>
#include <RF24.h>
#include <nrf24L01.h>

#define BATT_CHECK A3
#define CS_Pin 10
#define CE_Pin 14
#define l_click 9
#define r_click 8

bool lbutt[3] = {false, false, false};
bool rbutt[3] = {false, false, false};

RF24 rad(CE_Pin, CS_Pin);

typedef struct {
  uint16_t voltage;
  uint8_t l_clicked;
  uint8_t r_clicked; 
} Data; // 4 bytes. 

Data d;

void radio_init() {
  rad.begin();
  
  rad.setPALevel(RF24_PA_MIN);
  rad.setPayloadSize(4);
  rad.setChannel(7);
  rad.setCRCLength(RF24_CRC_16);
  rad.setDataRate(RF24_1MBPS);
  rad.openReadingPipe(1, 0xDC);
  rad.openWritingPipe(0xCC);
}

uint8_t updatevalues(int button, bool arr[3]){  //check for button presses
  //constantly keeps the last three read button values
  arr[0] = arr[1];
  arr[1] = arr[2];
  arr[2] = (bool) digitalRead(button);
  
  //basically if it detects that it was low, and then becomes high
  // then we say a button has been pressed. 
  if ( !arr[0] && arr[1] && arr[2] ) {
    return 1;
  }
  // on the other hand, we also note when the button gets unpressed.
  if ( arr[0] && !arr[1] && !arr[2] ) {
    return 1;
  }
  return 0;
}

void setup() {
  // put your setup code here, to run once:
  radio_init();
  printf_begin();
  Serial.begin(9600);
  
  pinMode(BATT_CHECK, INPUT);
  pinMode(l_click, INPUT);
  pinMode(r_click, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //int val = analogRead(BATT_CHECK);
  d.voltage = analogRead(BATT_CHECK);
  //val * 3.31 / 1023 * (5500 + 1500) / 5500;

  d.l_clicked = updatevalues(l_click, lbutt);
  d.r_clicked = updatevalues(r_click, rbutt);
  
  while(!rad.write(&d, sizeof(d)));
}
