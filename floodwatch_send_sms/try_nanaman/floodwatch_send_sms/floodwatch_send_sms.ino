#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // gsm module connected here
String textForSMS;
 
// cell numbers to which you want to send the security alert message
String f1001 = "+639686646782"; //kc
String f1002 = "+639456052639"; //che
String f1003 = "+639558877355"; //ban
String f1004 = "+639457850735";  
String f1005 = "+639217095100";  

 
void setup() {
 
 
  Serial.begin(9600);
  SIM900.begin(9600); // original 19200. while enter 9600 for sim900A
 Serial.println(" logging time completed!");

 delay(5000); // wait for 5 seconds

  textForSMS =  "\nHello World, We are the FloodWatch";  
  //sendSMS(textForSMS);
  sendsms(textForSMS, f1001); // you can use a variable of the type String
  Serial.println(textForSMS);
  Serial.println("message sent."); 
  delay(5000);
 
//  sendsms(textForSMS, f1002); // you can also write any message that you want to send.
//  Serial.println(textForSMS);
//  Serial.println("message sent."); 
//  delay(5000);
// 
//  sendsms(textForSMS, f1003); // you can also write any message that you want to send.
//  Serial.println(textForSMS);
//  Serial.println("message sent."); 
//  delay(5000);  
//
//  sendsms(textForSMS, f1004); // you can also write any message that you want to send.
//  Serial.println(textForSMS);
//  Serial.println("message sent."); 
//  delay(5000); 
//
//  sendsms(textForSMS, f1005); // you can also write any message that you want to send.
//  Serial.println(textForSMS);
//  Serial.println("message sent."); 
//  delay(5000); 
  
}
 
void loop() {
 
}
 
 
void sendsms(String message, String number){
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
