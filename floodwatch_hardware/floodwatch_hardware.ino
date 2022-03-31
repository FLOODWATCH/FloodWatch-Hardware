#include <SoftwareSerial.h>

SoftwareSerial myGSM(8,9); //This is GSM Library -> 8 is rx then 9 is the tx

const int trigPin = 3;
const int echoPin = 4;
const int piezoPin = 13;
long duration, inches, cm;

void setup() {
//---------ULTRASONIC SET UP START-----------------------------------------------------------
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);  
  Serial.begin(9600);  
//---------ULTRASONIC SET UP END-----------------------------------------------------------

}

void loop() {
//---------ULTRASONIC CODE START-----------------------------------------------------------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  String display;
  if(inches >= 150 ){
    //LEVEL 1 -> AMBON PALANG
    tone(piezoPin, 000);

  }else if(inches > 80 && inches < 100){
    //LEVEL 2 -> MEDYO BAHA
    tone(piezoPin, 100);
 
  }else if(inches >= 50 && inches <= 80){
    //LEVEL 3 => GA TUHOD NA BAHA
    tone(piezoPin, 200);
  
  }else if(inches < 50){
    //LEVEL 4 -> NALUNOD NA
    tone(piezoPin, 500);
  
  }
 
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
 //---------ULTRASONIC CODE START-----------------------------------------------------------
}

//Function for MS to INCHES [ultrasonic function]
long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}
//Function for MS to CM [ultrasonic function]
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
