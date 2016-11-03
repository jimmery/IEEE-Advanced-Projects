void radio_init() {
  setPALevel(RF_PA_LOW);
  setPayloadSize(32);
  setChannel(7);
  setCRCLength(RF24_CRC_16);
  setDataRate(RF24_1MPS);
  openReadingPipe(0xCC);
  openWritingPipe(0xDC);
}
void setup() {
  // put your setup code here, to run once:
setPALevel(RF_PA_LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

}
