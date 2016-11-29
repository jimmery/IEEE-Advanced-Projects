//#include <printf.h>
#include <RF24.h>
#include <nrf24L01.h>
#include <Wire.h>

#define BATT_CHECK A3
#define CS_Pin 10
#define CE_Pin 14
#define l_click 9
#define r_click 8

const int MPU_addr = 0x68;

bool lbutt[3] = {false, false, false};
bool rbutt[3] = {false, false, false};

RF24 rad(CE_Pin, CS_Pin);

typedef struct {
  int16_t ax;
  int16_t ay;
  int16_t az;
} Accel_Data; // 6 bytes. 

typedef struct {
  int16_t gx;
  int16_t gy;
  int16_t gz;
} Gyro_Data; // 6 bytes. 

typedef struct {
  uint16_t voltage;
  uint8_t l_clicked;
  uint8_t r_clicked; 
  Accel_Data ad;
  Gyro_Data gd;
} Data; // 16 bytes. 

Data d;

typedef enum : uint8_t {
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
  prec &= 0b11;
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

void radio_init() {
  rad.begin();
  
  rad.setPALevel(RF24_PA_MIN);
  rad.setPayloadSize(sizeof(Data));
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
//  printf_begin();
  Serial.begin(9600);
  
  pinMode(BATT_CHECK, INPUT);
  pinMode(l_click, INPUT);
  pinMode(r_click, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  d.voltage = analogRead(BATT_CHECK);
  //val * 3.31 / 1023 * (5500 + 1500) / 5500;

  d.l_clicked = updatevalues(l_click, lbutt);
  d.r_clicked = updatevalues(r_click, rbutt);

  getAccelData(&(d.ad.ax), &(d.ad.ay), &(d.ad.az));
  getGyroData(&(d.gd.gx), &(d.gd.gy), &(d.gd.gz));
  while(!rad.write(&d, sizeof(d)));
}
