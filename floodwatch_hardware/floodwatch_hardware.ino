#include <SoftwareSerial.h>

//GSM Module connected here below
SoftwareSerial SIM900(7,8); //This is GSM Library -> 7 is rx then 8 is the tx
String textForSMS;
//SIM900 GSM Set up below
// cell numbers to which you want to send the security alert message
String f1001 = "+639686646782"; //kc
String f1002 = "+639456052639"; //che
String f1003 = "+639558877355"; //ban
String f1004 = "+639457850735";  
String f1005 = "+639217095100";  
String phoneNumbers [] = {f1001, f1002, f1003, f1004, f1005};

//ultrasonic set up below
const int trigPin = 3;
const int echoPin = 4;
const int piezoPin = 13;
long duration, inches, cm;

//millis config below
unsigned long previousMillis = 0;  //will store last time LED was blinked
const long period = 5000;         // period at which to send SMS
const long configSmsPeriod = 1000; //period for sms function/method (thats the method 3 on the last)

void setup() {
//---------ULTRASONIC SET UP START-----------------------------------------------------------
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);  
  Serial.begin(9600);  
//---------ULTRASONIC SET UP END-----------------------------------------------------------

//---------SIM900 GSM SET UP START-----------------------------------------------------------
  SIM900.begin(9600); // original 19200. while enter 9600 for sim900A
  Serial.println(" logging time completed!");
  delay(5000); // wait for 5 seconds
//---------SIM900 GSM SET UP END-----------------------------------------------------------

}

void loop() {
//---------CODE START-----------------------------------------------------------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  String display;
  if(inches > 157){ //greater than 4 meters
    
    //LEVEL 0 -> No SMS && NO HTTP Request
    unsigned long currentMillis = millis(); // store the current time
    if(currentMillis - previousMillis >= period) { // check if 1000ms passed
      previousMillis = currentMillis;   // save the last time you blinked the LED
      Serial.println("The Water Level Is Normal, Have a great Day!");    
    }
    
    
  }else if(inches > 118 && inches <= 157 ){ //greater than 3 meters and less than or equal to 4 meters
    
    //LEVEL 1 -> AMBON PALANG
    tone(piezoPin, 000);
    //send SMS
    textForSMS = "\nAlert LEVEL 1 \nGreen Warning"; //dagdagan pa to
    //Serial.println(textForSMS);
    for(int i = 0; i < 5; i++){ //5 is the number of phone numbers that is registered (the value may change)

        unsigned long currentMillis = millis(); // store the current time
        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
          previousMillis = currentMillis;   // save the last time you blinked the LED
            
          sendsms(textForSMS, phoneNumbers[i]); //i just iterate to all of the phone numbers
          Serial.println(textForSMS);
          Serial.println("message sent.");
          Serial.println("The Message: " + textForSMS); 
          //delay(5000); no more delays bc of millis()            
        }        

    }    


  }else if(inches > 78 && inches <= 118){ //greater than 2 meters and less than or equal to 3 meters

    //LEVEL 2 -> MEDYO BAHA
    tone(piezoPin, 100);
    //send SMS
    textForSMS = "\nAlert LEVEL 2 \nYellow Warning"; //dagdagan pa to
    //Serial.println(textForSMS);
    for(int i = 0; i < 5; i++){ //5 is the number of phone numbers that is registered (the value may change)

        unsigned long currentMillis = millis(); // store the current time
        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
          previousMillis = currentMillis;   // save the last time you blinked the LED
            
          sendsms(textForSMS, phoneNumbers[i]); //i just iterate to all of the phone numbers
          Serial.println(textForSMS);
          Serial.println("message sent.");
          Serial.println("The Message: " + textForSMS); 
          //delay(5000); no more delays bc of millis()            
        }        

    }     

 
  }else if(inches > 39 && inches <= 78){ //greater than 1 meter and less than or equal to 2 meters

    //LEVEL 3 => GA TUHOD NA BAHA
    tone(piezoPin, 200);
    //send SMS
    textForSMS = "\nAlert LEVEL 3 \nOrange Warning"; //dagdagan pa to
    //Serial.println(textForSMS);
    for(int i = 0; i < 5; i++){ //5 is the number of phone numbers that is registered (the value may change)

        unsigned long currentMillis = millis(); // store the current time
        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
          previousMillis = currentMillis;   // save the last time you blinked the LED
            
          sendsms(textForSMS, phoneNumbers[i]); //i just iterate to all of the phone numbers
          Serial.println(textForSMS);
          Serial.println("message sent.");
          Serial.println("The Message: " + textForSMS); 
          //delay(5000); no more delays bc of millis()            
        }        

    }        

  
  }else if(inches <= 39){ //less than or equal to 1 meter
 
    //LEVEL 4 -> NALUNOD NA
    tone(piezoPin, 500);
    //send SMS
    textForSMS = "\nAlert LEVEL 4 \nRed Warning"; //dagdagan pa to
    //Serial.println(textForSMS);
    for(int i = 0; i < 5; i++){ //5 is the number of phone numbers that is registered (the value may change)

        unsigned long currentMillis = millis(); // store the current time
        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
          previousMillis = currentMillis;   // save the last time you blinked the LED
            
          sendsms(textForSMS, phoneNumbers[i]); //i just iterate to all of the phone numbers
          Serial.println(textForSMS);
          Serial.println("message sent.");
          Serial.println("The Message: " + textForSMS); 
          //delay(5000); no more delays bc of millis()            
        }        

    }    
 
  }
 
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
  
 //---------CODES-----------------------------------------------------------
}

void sendsms(String message, String number){ //SEND SMS
  String mnumber = "AT + CMGS = \""+number+"\""; 
  SIM900.print("AT+CMGF=1\r");                   
  delay(1000);
  SIM900.println(mnumber);  // recipient's mobile number, in international format
 
  delay(1000);
  SIM900.println(message);                         // message to send
  delay(1000);
  SIM900.println((char)26);                        // End AT command with a ^Z, ASCII code 26
  delay(1000); 
  SIM900.println();
  delay(100);                                     // give module time to send SMS
 // SIM900power();  
}

long microsecondsToInches(long microseconds) { //Function for MS to INCHES [ultrasonic function]
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) { //Function for MS to CM [ultrasonic function]
  return microseconds / 29 / 2;
}
