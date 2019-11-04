#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
WiFiClient client;
WiFiServer server(80);

/* WIFI settings */

String command;             //String to store app command state.
int speedCar = 1000;         // 400 - 1023.
int speed_Coeff = 3;


const char* ssid = "agilea";
const char* password = "agile1234";




//  Enter your Write API key from ThingSpeak
const char* servers = "api.thingspeak.com";
String apiKey = "WSZVIO54OURNPW20";  

//define the parameters for the IFTTT
#define HOSTIFTTT "maker.ifttt.com"
#define EVENTO "SmartCar"
#define IFTTTKEY "cZRkTCYVS8WkXQE_2JpcLh"

///* data received from application */
String  data =""; 

/* define L298N or L293D motor control pins */
int leftMotorForward = 2;     /* GPIO2(D4) -> IN3   */
int rightMotorForward = 15;   /* GPIO15(D8) -> IN1  */
int leftMotorBackward = 0;    /* GPIO0(D3) -> IN4   */
int rightMotorBackward = 13;  /* GPIO13(D7) -> IN2  */


/* define L298N or L293D enable pins */
int rightMotorENB = 14; /* GPIO14(D5) -> Motor-A Enable */
int leftMotorENB = 12;  /* GPIO12(D6) -> Motor-B Enable */


/*Ultra sonic code*/
const int trigPin = D1;
const int echoPin = D0;
int buzz = D2;
long duration;
int distance;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
 Serial.begin(115200);
  

  /* initialize motor control pins as output */
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT); 
  pinMode(leftMotorBackward, OUTPUT);  
  pinMode(rightMotorBackward, OUTPUT);

  /* initialize motor enable pins as output */
  pinMode(leftMotorENB, OUTPUT); 
  pinMode(rightMotorENB, OUTPUT);

  /* start server communication */
  server.begin();

 

 WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Waiting to connect to WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);

 
//  
}

void loop()
{

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
Serial.println(distance);
if(distance <= 50 && distance >= 20)
{
  digitalWrite(13, HIGH); 
  
 // digitalWrite(13, LOW);
  // turn the LED on (HIGH is the voltage level)

}


else
{ 
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
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
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
             // wait for a second    
}

  

    /* If the server available, run the "checkClient" function */  
    client = server.available();
    if (!client) return; 
    data = checkClient ();

/************************ Run function according to incoming data from application *************************/

    /* If the incoming data is "forward", run the "MotorForward" function */
    if (data == "forward") MotorForward();
    /* If the incoming data is "backward", run the "MotorBackward" function */
    else if (data == "backward") MotorBackward();
    /* If the incoming data is "left", run the "TurnLeft" function */
    else if (data == "left") TurnLeft();
    /* If the incoming data is "right", run the "TurnRight" function */
    else if (data == "right") TurnRight();
    /* If the incoming data is "stop", run the "MotorStop" function */
    else if (data == "stop") MotorStop();






//**************************************wifi*****************************************************
if (client.connected())
  {
    client.stop();
  }

  client.flush();
  if (client.connect(HOSTIFTTT,80)) {
    Serial.println("Connected");
    // build the HTTP request
    String toSend = "GET /trigger/";
    toSend += EVENTO;
    toSend += "/with/key/";
    toSend += IFTTTKEY;
    toSend += "?value1=";
    toSend += distance;
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += HOSTIFTTT;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";
    client.print(toSend);
  }
  client.flush();
  client.stop();  
  delay(500);
   
     if (isnan(distance)) 
                 {
                     Serial.println("Failed to read from  sensor!");
                      return;
                 }

                         if (client.connect(servers,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;

                             
                             postStr +="&field1=";
                             postStr += String(distance);
                            
                             postStr += "\r\n\r\n";

                             
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print(" Total Distance: ");
                             Serial.print(distance);
                             
                             Serial.println("%. Send to Thingspeak.");

                         
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
         
    
 
    
}

/********************************************* FORWARD *****************************************************/

void MotorForward(void)   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  analogWrite(leftMotorENB, speedCar);
  analogWrite(rightMotorENB, speedCar);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(rightMotorForward,HIGH);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorBackward,LOW);
}

/********************************************* BACKWARD *****************************************************/
void MotorBackward(void)   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  analogWrite(leftMotorENB, speedCar);
  analogWrite(rightMotorENB, speedCar);
  digitalWrite(leftMotorBackward,HIGH);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,LOW);
}

/********************************************* TURN LEFT *****************************************************/
void TurnLeft(void)   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH); 
  analogWrite(leftMotorENB, speedCar);
  analogWrite(rightMotorENB, speedCar);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,HIGH);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorBackward,HIGH);  
}

/********************************************* TURN RIGHT *****************************************************/
void TurnRight(void)   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  analogWrite(leftMotorENB, speedCar);
  analogWrite(rightMotorENB, speedCar);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorBackward,LOW);
}

/********************************************* STOP *****************************************************/
void MotorStop(void)   
{
  digitalWrite(leftMotorENB,LOW);
  digitalWrite(rightMotorENB,LOW);
  analogWrite(leftMotorENB, speedCar);
  analogWrite(rightMotorENB, speedCar);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
}

/********************************** RECEIVE DATA FROM the APP ******************************************/
String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}
