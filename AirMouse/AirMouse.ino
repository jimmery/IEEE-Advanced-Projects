#define BATT_CHECK A0
#define LED_PIN 13

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(BATT_CHECK, INPUT);
  pinMode(LED_PIN, OUTPUT);

  delay(10);
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(BATT_CHECK);

  float voltage = val * 3.3 / 1023 * (5500 + 1500) / 5500;

  Serial.println(voltage);
}
