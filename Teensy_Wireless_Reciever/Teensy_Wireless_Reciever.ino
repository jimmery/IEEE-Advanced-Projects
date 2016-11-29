#include <RF24.h>
#include <printf.h>
#include <Wire.h>
// define all the pins we are using. 
#define LED_PIN 13
#define RED_LED 0
#define YEL_LED 1
#define GRN_LED 2

const int MPU_addr = 0x68;

typedef enum : uint8_t{
  GYRO_PREC_250 = 0,
  GYRO_PREC_500,
  GYRO_PREC_1000,
  GYRO_PREC_2000,
} gyro_precision_e;

typedef enum : uint8_t
{
  ACCEL_PREC_2 = 0,
  ACCEL_PREC_4,
  ACCEL_PREC_8,
  ACCEL_PREC_16
} accel_precision_e;

void setSleep(bool enable){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 1, true);
  uint8_t power = Wire.read();
  power = ((enable) ? (power | 0b01000000) : (power & 0b10111111));
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(power);
  Wire.endTransmission(true);
}

void getAccelData(int16_t* ax, int16_t* ay, int16_t* az){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true);
  *ax = Wire.read() << 8 | Wire.read();
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3D);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true);
  *ay = Wire.read() << 8 | Wire.read();
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3F);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true);
  *az = Wire.read() << 8 | Wire.read();
  Wire.endTransmission(true);
}

void getGyroData(int16_t* gx, int16_t* gy, int16_t* gz){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true);
  *gx = Wire.read() << 8 | Wire.read();
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x45);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true);
  *gy = Wire.read() << 8 | Wire.read();
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true);
  *gz = Wire.read() << 8 | Wire.read();
  Wire.endTransmission(true);
}

void setGyroPrec(gyro_precision_e prec){
  //prec &= 0b11;
  uint8_t test = 1;
  prec = test;
  prec = prec<<3;
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1B);
  Wire.write(prec);
  Wire.endTransmission(true);  
}

void setAccelPrec(accel_precision_e prec){
  prec &= 0b11;
  prec = prec<<3;
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1C);
  Wire.write(prec);
  Wire.endTransmission(true);
}

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
