#include <LiquidCrystal.h>
#include <Servo.h>
#define trigPin 3
#define echoPin 2
#define trigPin2 10
#define echoPin2 11
#define openParking 0
#define closeParking 0

boolean full;
boolean openn;
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo servo;
int counter=0;
int parking_area;

void setup() {
Serial.begin (9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);

servo.attach(A5);
}
void loop() {
long duration, distance;
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
servo.write(closeParking);
lcd.begin(16, 2);

if(full){
  lcd.print("Sorry! Full");
}

else if(counter==40){
  lcd.print("full | people: ");
  lcd.print(counter);
}
else{
  lcd.print("open | people: ");
  lcd.print(counter);
}
if (distance < 5) {
    
  if(counter<40){
    full=false;  
    counter ++;
    parking_area ++;
    Serial.println("People in");

    lcd.clear();
    lcd.print("People in");
    
    Serial.println(counter);
    servo.write(openParking);
    delay(5000); 
  }
     else if(counter >= 40){ 
    servo.write(closeParking);
    Serial.println("Seat full");
    full=true;
   }
}
/* for leeting car out */
 long duration2, distance2;
digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2, HIGH);
distance2 = (duration2/2) / 29.1;
  if(distance2 < 5){
      if(counter==0){
        return;
      }
       counter --;
      servo.write(openParking);
      full=false;
      Serial.println("People out");
      lcd.clear();
      lcd.print(" People out");
      delay(5000);
  }
}
