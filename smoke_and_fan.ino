#define SENS_PIN A1 // Defines A0 pin as "sensor"
#define FAN_PIN 9
int Vin; // Reads value from Arduino pin
float Temperature; // Receives converted voltage value to temp
float TF; // Receives converted value in °F
void setup() {

  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(FAN_PIN, OUTPUT); // Fan pin as an output
  Serial.begin(9600);
}

void loop() {

 int sens = analogRead(A0);
 if(sens >= 800){
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(12, LOW);
 }else{
  digitalWrite(13, LOW);
 }
 Serial.print("Smoke-Level: ");
 Serial.println(sens);

// Tells Arduino to read pin and stores value in Vin
Vin = analogRead(SENS_PIN);
// Converts voltage value into temperature and
// stores value in Temperature (as °F)
Temperature = (500 * Vin) / 1023 * (1.8) + 32;
TF = Temperature;
Serial.print("Temperature: "); // Sends text to display screen
Serial.print(TF); // Shows value of temperature in Serial Monitor
Serial.println(" F"); // Writes F to indicate it is in Fahrenheit
if (TF > 71) { // If temperature is more than 71
digitalWrite(FAN_PIN, HIGH); // Turn fan on
}
else if (TF < 71) {
digitalWrite(FAN_PIN, LOW); // Or keep fan off
}
delay(1000); // Waits for a second to read the pin again
}
