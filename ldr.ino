/*
https://somtips.com
https://youtube.com/somtips
*/
const int ledPin = 4;
const int ldrPin = A0;
void setup() {

Serial.begin(9600);

pinMode(ledPin, OUTPUT);

pinMode(ldrPin, INPUT);
}
void loop() {
int ldrStatus = analogRead(ldrPin);
if (ldrStatus < 675) {
  digitalWrite(ledPin, HIGH);
  }
else {
  digitalWrite(ledPin, LOW);
  }
//checking value
Serial.println(ldrStatus);
delay(500);
}
