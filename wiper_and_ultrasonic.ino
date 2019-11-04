#include <Servo.h> //include servo library
Servo myservo; //define servo as servo
const int waterSens = A0;//set water sensor to A0
int pos = 0;//define servo position
const int trigPin = 7;
const int echoPin = 4;
int buzz = 10;
long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  myservo.attach(9);//attach servo to pin 9
  
}

void loop() {
  int sensorValue = analogRead(waterSens);//read the water sensor value
          
  sensorValue = map(sensorValue, 0, 1023, 0, 180); 
     if (sensorValue >= 50) {
        for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
      
     }
     
  else{
    myservo.write(0);
  }

  
  delay(20);  

                  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
if(distance <= 50 && distance >= 20)
{
digitalWrite(13, HIGH);
// digitalWrite(13, LOW);
// turn the LED on (HIGH is the voltage level)
}
else
{
digitalWrite(13, LOW); // turn the LED off by making the voltage LOW
// wait for a second
}
if(distance <= 20)
{
digitalWrite(2, HIGH);
tone(buzz, 2000);
delay(100);
noTone(buzz);
delay(100);
tone(buzz, 2000);
delay(100);
noTone(buzz);
delay(100);
tone(buzz, 2000);
delay(100);
noTone(buzz);
tone(buzz, 2000);
delay(100);
noTone(buzz);
delay(100);
}
else
{
digitalWrite(2, LOW); // turn the LED off by making the voltage LOW
// wait for a second
}
Serial.println(distance);
}      
