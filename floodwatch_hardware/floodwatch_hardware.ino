#include <LiquidCrystal.h>

#include <SoftwareSerial.h>


LiquidCrystal lcd(5, 6, 12, 11, 10, 9);

//GSM Module connected here below
SoftwareSerial SIM900(7,8); //This is GSM Library -> 7 is rx then 8 is the tx
String textForSMS;
//SIM900 GSM Set up below
// cell numbers to which you want to send the security alert message
String f1001 = "+639686646782"; //kc
String f1002 = "+639457850735"; //rhy 
String f1003 = "+639217095100"; //mer
String f1004 = "+639260722719"; // jannel`
String f1005 = "+639456052639"; //che
String f1006 = "+639558877355"; //ban
//String phoneNumbers [] = {f1001, f1002, f1003, f1004, f1005};

//ultrasonic set up below
//const int trigPin = 3;
//const int echoPin = 4;
const int piezoPin = 13;
long inches, cm;
#define trigPin 3
#define echoPin 4
long duration;
int distance;



//millis config below
unsigned long previousMillis = 0;  //will store last time LED was blinked
const long period = 40000;         // period at which to send SMS 40 seconds
const long configSmsPeriod = 1000; //period for sms function/method (thats the method 3 on the last)

void setup() {
  int indicators [] = {9, 10, 11, 12};
  for(int i = 0; i < 4; i++){
    pinMode(indicators[i], OUTPUT);
  }
//---------ULTRASONIC SET UP START-----------------------------------------------------------
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);  
  Serial.begin(115200); 
//---------ULTRASONIC SET UP END-----------------------------------------------------------

//---------SIM900 GSM SET UP START-----------------------------------------------------------
  SIM900.begin(9600); // original 19200. while enter 9600 for sim900A
  Serial.println(" logging time completed!");
  delay(5000); // wait for 5 seconds
//---------SIM900 GSM SET UP END-----------------------------------------------------------
    lcd.clear();
    String display = "AMBON PALANG";
    lcd.print(display);


}

void loop() {
//---------CODE START-----------------------------------------------------------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  distance = duration*0.034/2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.print(",  ");
  Serial.print(inches);
  Serial.print(" inches");
  Serial.println();
  delay(100);

  String display;
  if(distance > 350 || distance == 0){ //greater than 4 meters
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    
    //LEVEL 0 -> No SMS && NO HTTP Request
    unsigned long currentMillis = millis(); // store the current time
    if(currentMillis - previousMillis >= period) { // check if 1000ms passed
      previousMillis = currentMillis;   // save the last time you blinked the LED
      Serial.println("The Water Level Is Normal, Have a great Day!");    
    }
    digitalWrite(piezoPin, LOW);

    
    
  }else if(distance > 250 && distance <= 350 ){ //greater than 3 meters and less than or equal to 4 meters
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    
    //LEVEL 1 -> AMBON PALANG
    tone(piezoPin, LOW);
    //send SMS
    textForSMS = "\nAlert LEVEL 1 \nRainfall is at normal level"; //dagdagan pa to
    //Serial.println(textForSMS);
   
    //dsasdasdsadsdasdasdasdsdadasdasd
        //5 is the number of phone numbers that is registered (the value may change)

        unsigned long currentMillis = millis(); // store the current time
        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
          previousMillis = currentMillis;   // save the last time you blinked the LED
            
          sendsms(textForSMS, f1001); //i just iterate to all of the phone numbers
           sendsms(textForSMS, f1002);
            sendsms(textForSMS, f1003);
             sendsms(textForSMS, f1004);
              sendsms(textForSMS, f1005);
               sendsms(textForSMS, f1006);
          Serial.println(textForSMS);
          Serial.println("message sent.");
          Serial.println("The Message: " + textForSMS); 
          //delay(5000); no more delays bc of millis()            
        }        

   
    //dasdasdsasdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf


  }else if(distance > 150 && distance <= 250){ //greater than 2 meters and less than or equal to 3 meters
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    
    //LEVEL 2 -> MEDYO BAHA
    tone(piezoPin, 180);
    //send SMS
    textForSMS = "\nAlert LEVEL 2 \nRainfall is above normal level. Flooding is possible"; //dagdagan pa to
    //Serial.println(textForSMS);
     
    //dsasdasdsadsdasdasdasdsdadasdasd
        //5 is the number of phone numbers that is registered (the value may change)

        unsigned long currentMillis = millis(); // store the current time
        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
          previousMillis = currentMillis;   // save the last time you blinked the LED
            
          sendsms(textForSMS, f1001); //i just iterate to all of the phone numbers
           sendsms(textForSMS, f1002);
            sendsms(textForSMS, f1003);
             sendsms(textForSMS, f1004);
              sendsms(textForSMS, f1005);
               sendsms(textForSMS, f1006);
          Serial.println(textForSMS);
          Serial.println("message sent.");
          Serial.println("The Message: " + textForSMS); 
          //delay(5000); no more delays bc of millis()            
        }        

   
    //dasdasdsasdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf

 
  }else if(distance > 50 && distance <= 150){ //greater than 1 meter and less than or equal to 2 meters
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    
    //LEVEL 3 => GA TUHOD NA BAHA
    tone(piezoPin, 300);
    //send SMS
    textForSMS = "\nAlert LEVEL 3 \nFlooding is threatening, Prepare for Evacuation"; //dagdagan pa to
    //Serial.println(textForSMS);
   
    //dsasdasdsadsdasdasdasdsdadasdasd
        //5 is the number of phone numbers that is registered (the value may change)

        unsigned long currentMillis = millis(); // store the current time
        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
          previousMillis = currentMillis;   // save the last time you blinked the LED
            
          sendsms(textForSMS, f1001); //i just iterate to all of the phone numbers
           sendsms(textForSMS, f1002);
            sendsms(textForSMS, f1003);
             sendsms(textForSMS, f1004);
              sendsms(textForSMS, f1005);
               sendsms(textForSMS, f1006);
          Serial.println(textForSMS);
          Serial.println("message sent.");
          Serial.println("The Message: " + textForSMS); 
          //delay(5000); no more delays bc of millis()            
        }        

   
    //dasdasdsasdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf    

  
  }else if(distance > 0 && distance <= 50 ){ //less than or equal to 1 meter
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    
    //LEVEL 4 -> NALUNOD NA
    tone(piezoPin, 500);
    //send SMS
    textForSMS = "\nAlert LEVEL 4 \nFULL EVACUATION IS NEEDED, please evacuate at nearest evacuation centers"; //dagdagan pa to
    //Serial.println(textForSMS);
    

    //dsasdasdsadsdasdasdasdsdadasdasd
        //5 is the number of phone numbers that is registered (the value may change)

        unsigned long currentMillis = millis(); // store the current time
        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
          previousMillis = currentMillis;   // save the last time you blinked the LED
            
          sendsms(textForSMS, f1001); //i just iterate to all of the phone numbers
           sendsms(textForSMS, f1002);
            sendsms(textForSMS, f1003);
             sendsms(textForSMS, f1004);
              sendsms(textForSMS, f1005);
               sendsms(textForSMS, f1006);
          Serial.println(textForSMS);
          Serial.println("message sent.");
          Serial.println("The Message: " + textForSMS); 
          //delay(5000); no more delays bc of millis()            
        }        

   
    //dasdasdsasdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf
 
  }
 

  
 //---------CODES-----------------------------------------------------------
}

void sendsms(String message, String number){ //SEND SMS
  String mnumber = "AT + CMGS = \""+number+"\""; 
  SIM900.print("AT+CMGF=1\r");                   
  delay(1000);
  SIM900.println(mnumber);  // recipient's mobile number, in international format
 
  delay(500);
  SIM900.println(message);                         // message to send
  delay(500);
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

















//-------------------||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||-----------------------------------------------------------------------------------------------------------------
//#include <SoftwareSerial.h>
//
////GSM Module connected here below
//SoftwareSerial SIM900(7,8); //This is GSM Library -> 7 is rx then 8 is the tx
//String textForSMS;
////SIM900 GSM Set up below
//// cell numbers to which you want to send the security alert message
//String f1001 = "+639686646782"; //kc
//String f1002 = "+639456052639"; //che
//String f1003 = "+639558877355"; //ban
//String f1004 = "+639457850735";  
//String f1005 = "+639217095100";  
//String phoneNumbers [] = {f1001, f1002, f1003, f1004, f1005};
//
////ultrasonic set up below
////const int trigPin = 3;
////const int echoPin = 4;
//const int piezoPin = 13;
//long inches, cm;
//#define trigPin 3
//#define echoPin 4
//long duration;
//int distance;
//
//
//
////millis config below
//unsigned long previousMillis = 0;  //will store last time LED was blinked
//const long period = 15000;         // period at which to send SMS 15 seconds
//const long configSmsPeriod = 1000; //period for sms function/method (thats the method 3 on the last)
//
//void setup() {
////---------ULTRASONIC SET UP START-----------------------------------------------------------
//  pinMode(trigPin, OUTPUT);  
//  pinMode(echoPin, INPUT);  
//  Serial.begin(115200); 
////---------ULTRASONIC SET UP END-----------------------------------------------------------
//
////---------SIM900 GSM SET UP START-----------------------------------------------------------
//  SIM900.begin(9600); // original 19200. while enter 9600 for sim900A
//  Serial.println(" logging time completed!");
//  delay(5000); // wait for 5 seconds
////---------SIM900 GSM SET UP END-----------------------------------------------------------
//
//}
//
//void loop() {
////---------CODE START-----------------------------------------------------------
//  digitalWrite(trigPin, LOW);
//  delayMicroseconds(2);
//  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(20);
//  digitalWrite(trigPin, LOW);
//
//  duration = pulseIn(echoPin, HIGH);
//  inches = microsecondsToInches(duration);
//  cm = microsecondsToCentimeters(duration);
//  distance = duration*0.034/2;
//
//  Serial.print("Distance: ");
//  Serial.print(distance);
//  Serial.print(" cm");
//  Serial.print(",  ");
//  Serial.print(inches);
//  Serial.print(" inches");
//  Serial.println();
//  delay(100);
//
//  String display;
//  if(distance > 400 || distance == 0){ //greater than 4 meters
//    
//    //LEVEL 0 -> No SMS && NO HTTP Request
//    unsigned long currentMillis = millis(); // store the current time
//    if(currentMillis - previousMillis >= period) { // check if 1000ms passed
//      previousMillis = currentMillis;   // save the last time you blinked the LED
//      Serial.println("The Water Level Is Normal, Have a great Day!");    
//    }
//    
//    
//  }else if(distance > 300 && distance <= 400 ){ //greater than 3 meters and less than or equal to 4 meters
//    
//    //LEVEL 1 -> AMBON PALANG
//    tone(piezoPin, 000);
//    //send SMS
//    textForSMS = "\nAlert LEVEL 1 \nGreen Warning"; //dagdagan pa to
//    //Serial.println(textForSMS);
//    for(int i = 0; i < 5; i++){ //5 is the number of phone numbers that is registered (the value may change)
//
//        unsigned long currentMillis = millis(); // store the current time
//        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
//          previousMillis = currentMillis;   // save the last time you blinked the LED
//            
//          sendsms(textForSMS, phoneNumbers[i]); //i just iterate to all of the phone numbers
//          Serial.println(textForSMS);
//          Serial.println("message sent.");
//          Serial.println("The Message: " + textForSMS); 
//          //delay(5000); no more delays bc of millis()            
//        }        
//
//    }    
//
//
//  }else if(distance > 200 && distance <= 300){ //greater than 2 meters and less than or equal to 3 meters
//
//    //LEVEL 2 -> MEDYO BAHA
//    tone(piezoPin, 100);
//    //send SMS
//    textForSMS = "\nAlert LEVEL 2 \nYellow Warning"; //dagdagan pa to
//    //Serial.println(textForSMS);
//    for(int i = 0; i < 5; i++){ //5 is the number of phone numbers that is registered (the value may change)
//
//        unsigned long currentMillis = millis(); // store the current time
//        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
//          previousMillis = currentMillis;   // save the last time you blinked the LED
//            
//          sendsms(textForSMS, phoneNumbers[i]); //i just iterate to all of the phone numbers
//          Serial.println(textForSMS);
//          Serial.println("message sent.");
//          Serial.println("The Message: " + textForSMS); 
//          //delay(5000); no more delays bc of millis()            
//        }        
//
//    }     
//
// 
//  }else if(distance > 100 && distance <= 200){ //greater than 1 meter and less than or equal to 2 meters
//
//    //LEVEL 3 => GA TUHOD NA BAHA
//    tone(piezoPin, 200);
//    //send SMS
//    textForSMS = "\nAlert LEVEL 3 \nOrange Warning"; //dagdagan pa to
//    //Serial.println(textForSMS);
//    for(int i = 0; i < 5; i++){ //5 is the number of phone numbers that is registered (the value may change)
//
//        unsigned long currentMillis = millis(); // store the current time
//        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
//          previousMillis = currentMillis;   // save the last time you blinked the LED
//            
//          sendsms(textForSMS, phoneNumbers[i]); //i just iterate to all of the phone numbers
//          Serial.println(textForSMS);
//          Serial.println("message sent.");
//          Serial.println("The Message: " + textForSMS); 
//          //delay(5000); no more delays bc of millis()            
//        }        
//
//    }        
//
//  
//  }else if(distance > 0 && distance <= 100 ){ //less than or equal to 1 meter
// 
//    //LEVEL 4 -> NALUNOD NA
//    tone(piezoPin, 500);
//    //send SMS
//    textForSMS = "\nAlert LEVEL 4 \nRed Warning"; //dagdagan pa to
//    //Serial.println(textForSMS);
//    for(int i = 0; i < 5; i++){ //5 is the number of phone numbers that is registered (the value may change)
//
//        unsigned long currentMillis = millis(); // store the current time
//        if(currentMillis - previousMillis >= period) { // check if 1000ms passed
//          previousMillis = currentMillis;   // save the last time you blinked the LED
//            
//          sendsms(textForSMS, phoneNumbers[i]); //i just iterate to all of the phone numbers
//          Serial.println(textForSMS);
//          Serial.println("message sent.");
//          Serial.println("The Message: " + textForSMS); 
//          //delay(5000); no more delays bc of millis()            
//        }        
//
//    }    
// 
//  }
// 
//
//  
// //---------CODES-----------------------------------------------------------
//}
//
//void sendsms(String message, String number){ //SEND SMS
//  String mnumber = "AT + CMGS = \""+number+"\""; 
//  SIM900.print("AT+CMGF=1\r");                   
//  delay(1000);
//  SIM900.println(mnumber);  // recipient's mobile number, in international format
// 
//  delay(1000);
//  SIM900.println(message);                         // message to send
//  delay(1000);
//  SIM900.println((char)26);                        // End AT command with a ^Z, ASCII code 26
//  delay(1000); 
//  SIM900.println();
//  delay(100);                                     // give module time to send SMS
// // SIM900power();  
//}
//
//long microsecondsToInches(long microseconds) { //Function for MS to INCHES [ultrasonic function]
//  return microseconds / 74 / 2;
//}
//
//long microsecondsToCentimeters(long microseconds) { //Function for MS to CM [ultrasonic function]
//  return microseconds / 29 / 2;
//}
