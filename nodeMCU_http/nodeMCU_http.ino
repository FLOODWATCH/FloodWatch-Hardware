#include <Arduino_JSON.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#include <NTPClient.h> //for updated real date and time
#include <WiFiUdp.h>



//ultrasonic fields config below
//const int trigPin = 12; //GPIO 12 = D6
//const int echoPin = 14; //GPIO 14 = D5
//long duration, inches, cm;
long inches, cm;
#define trigPin 12 //GPIO 12 = D6
#define echoPin 14 //GPIO 14 = D5
long duration;
int distance;

//http nodeMCU wifi credentials below
const char* ssid = "GlobeAtHome_8B94E";
const char* password = "GLRE3HT1Q16";

//our floodwatch backend api url below
const char *host = "floodwatchbackend.herokuapp.com";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

//FW BE fingerprint below
#define TEST_HOST_FINGERPRINT "2aeeafbb002b5811729e1e98c88cc782525a37e6"

//Declare an object of class HTTPClient
HTTPClient http;

//for NTP Client get date and time config below
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


void setup() {
  //ultrasonic setup below
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200); // Starts the serial communication

  //nodeMCU wifi && http setup below
  Serial.begin(115200);
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { //testing if we are connected on wifi/internet
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


  //set up date and time config below
  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);
}

void loop() {
  timeClient.update(); //just update time

  time_t epochTime = timeClient.getEpochTime();

  String formattedTime = timeClient.getFormattedTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  String weekDay = weekDays[timeClient.getDay()];

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  
  int monthDay = ptm->tm_mday; //month day in number
  int currentMonth = ptm->tm_mon+1; //month in number
  String currentMonthName = months[currentMonth-1]; //month in name
  int currentYear = ptm->tm_year+1900; //year in number


  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  String DatenTime = (formattedTime + " " + "||" + " " + currentDate);
  Serial.print("Date and Time: ");
  Serial.println(DatenTime);
  Serial.print("Month: ");
  Serial.println(currentMonthName);

  

  //---------------------------------------------
  
  //ultrasonic digital config below
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
  
  //nodeMCU http configs below
  WiFiClientSecure httpsClient;
  
  httpsClient.setFingerprint(TEST_HOST_FINGERPRINT); //check BE webapp fingerprint
  
  String apiUrl = "https://floodwatchbackend.herokuapp.com/flwaterlevelpost"; //BE API URL
  
  if (WiFi.status() == WL_CONNECTED) { //if connected to wifi, read the ff codes || conditions
    //Specify request destination
    http.begin(httpsClient, apiUrl);
    http.addHeader("Content-Type", "application/json");
  
    //***********************conditions of ultrasonic and http post******************
    //conditions logic for http post request
    String display;
    if(distance > 400 || distance == 0){ //LEVEL 0 //greater than 4 meters
      //LEVEL 0 -> No SMS && NO HTTP Request
      Serial.println("The Water Level Is Normal, Have a great Day!");  

        
    }else if(distance > 300 && distance <= 400 ){ //LEVEL 1 //greater than 3 meters and less than or equal to 4 meters
      DynamicJsonDocument doc(1024);
  
      doc["wlLevel"] = "Level 1";
      doc["wlInfo"] = "Green Warning";
      doc["wlTime"] = DatenTime;
      doc["wlMonth"] = currentMonthName;
      doc["wlColor"] = "Green";
      
      String theData;
      serializeJson(doc, theData);
      Serial.println(theData);
      //LEVEL 1 -> AMBON PALANG || GREEN WARNING
      //send POST HTTP request
      Serial.println("connecting HTTPS.... Sending HTTP POST request to FloodWatch....");
      int httpCode = http.POST(theData); 
//      int httpCode = http.POST("{\"wlLevel\":\"Level 1\",\"wlInfo\":\"GREEN WARNING\",\"wlTime\":DatenTime,\"wlMonth\":currentMonthName,\"wlColor\":\"Green\"}");
      getHttpCode(httpCode);
      Serial.println("wait for 5 minutes..... sapagkat d naman ganoon kabilis ang pag angat ng tubig hehe");
      delay(600000); //the delay is 300 sec or 5 minutes, and that is according to what i have research that flash floods can rise one foor in just 5 minutes but we can increase that time delay definitely && and include natin to sa paper
      //for 10 minutes just put 600000
    }else if(distance > 200 && distance <= 300){ //LEVEL 2 //greater than 2 meters and less than or equal to 3 meters
      DynamicJsonDocument doc(1024);
  
      doc["wlLevel"] = "Level 2";
      doc["wlInfo"] = "Yellow Warning";
      doc["wlTime"] = DatenTime;
      doc["wlMonth"] = currentMonthName;
      doc["wlColor"] = "Yellow";
      
      String theData;
      serializeJson(doc, theData);
      Serial.println(theData);
      //LEVEL 2 -> MEDYO BAHA || YELLOW WARNING
      //send post http request
      Serial.println("connecting HTTPS.... Sending HTTP POST request to FloodWatch...."); 
      int httpCode = http.POST(theData);
//      int httpCode = http.POST("{\"wlLevel\":\"Level 2\",\"wlInfo\":\"YELLOW WARNING\",\"wlTime\":DatenTime,\"wlMonth\":currentMonthName,\"wlColor\":\"Yellow\"}");
      getHttpCode(httpCode);
      Serial.println("wait for 5 minutes..... sapagkat d naman ganoon kabilis ang pag angat ng tubig hehe");
      delay(600000);
   
    }else if(distance > 100 && distance <= 200){ //LEVEL 3 //greater than 1 meter and less than or equal to 2 meters

      DynamicJsonDocument doc(1024);
  
      doc["wlLevel"] = "Level 3";
      doc["wlInfo"] = "Orange Warning";
      doc["wlTime"] = DatenTime;
      doc["wlMonth"] = currentMonthName;
      doc["wlColor"] = "Orange";
      
      String theData;
      serializeJson(doc, theData);
      Serial.println(theData);
      //LEVEL 3 => GA TUHOD NA BAHA || ORANGE WARNING
      //send post http request
      Serial.println("connecting HTTPS.... Sending HTTP POST request to FloodWatch...."); 
      int httpCode = http.POST(theData);
//      int httpCode = http.POST("{\"wlLevel\":\"Level 3\",\"wlInfo\":\"ORANGE WARNING\",\"wlTime\":DatenTime,\"wlMonth\":currentMonthName,\"wlColor\":\"Orange\"}");
      getHttpCode(httpCode);
      Serial.println("wait for 5 minutes..... sapagkat d naman ganoon kabilis ang pag angat ng tubig hehe");
      delay(600000);
    
    }else if(distance > 0 && distance <= 100){ //LEVEL 4 //less than or equal to 1 meter
      DynamicJsonDocument doc(1024);
  
      doc["wlLevel"] = "Level 4";
      doc["wlInfo"] = "Red Warning";
      doc["wlTime"] = DatenTime;
      doc["wlMonth"] = currentMonthName;
      doc["wlColor"] = "Red";
      
      String theData;
      serializeJson(doc, theData);
      Serial.println(theData);
      //LEVEL 4 -> NALUNOD NA || RED WARNING
      //send post http request
      Serial.println("connecting HTTPS.... Sending HTTP POST request to FloodWatch....");
      int httpCode = http.POST(theData); 
//      int httpCode = http.POST("{\"wlLevel\":\"Level 4\",\"wlInfo\":\"RED WARNING\",\"wlTime\":DatenTime,\"wlMonth\":currentMonthName,\"wlColor\":\"Red\"}");
      getHttpCode(httpCode);
      Serial.println("wait for 5 minutes..... sapagkat d naman ganoon kabilis ang pag angat ng tubig hehe");
      delay(600000);
    
    }
    //*******************************************************************************
   
  }


  
}

void getHttpCode(int httpCode){ //function for getting httpcode and test it (the returning code)
  //Check the returning code
  if (httpCode > 0) { 
    //Get the request response payload
    String payload = http.getString();
    //Print the response payload
    Serial.println(payload);
  }else{ //the code is negative ex: -11
    //Close connection
    Serial.println(httpCode);
    http.end();
  }
}

long microsecondsToInches(long microseconds) { //Function for MS to INCHES [ultrasonic function]
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) { //Function for MS to CM [ultrasonic function]
  return microseconds / 29 / 2;
}
















////--------------------------------------------------------------------------------------
////for POST
//const char* ssid = "GlobeAtHome_8B94E";
//const char* password = "GLRE3HT1Q16";
//
//const char *host = "floodwatchbackend.herokuapp.com";
//const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
//
//#define TEST_HOST_FINGERPRINT "39a9c4feb17e239e2f4dbbace8d7a34fce430e7b"
//
//void setup() {
//  Serial.begin(115200);
//  Serial.print("Connecting Wifi: ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  IPAddress ip = WiFi.localIP();
//  Serial.println(ip);
//
//  WiFiClientSecure httpsClient;
//
//  // If you want to check the fingerprint
//  httpsClient.setFingerprint(TEST_HOST_FINGERPRINT);
//
//  //TESTING JSON CREATION
//  Serial.println("Starting JSON.. Converting data to JSON (Serialization).....");
//  
//  //char json[] = "{\"wlLevel\":\"Level 2\",\"wlInfo\":\"It is a Yellow Warning\",\"wlTime\":\"04/21/2022 | 4:00 PM\",\"wlMonth\":\"April\",\"wlColor\":\"Yellow\"}";
//  DynamicJsonDocument doc(192);
//  
//  JsonObject doc_0 = doc.createNestedObject();
//  doc_0["wlLevel"] = "Level 2";
//  doc_0["wlInfo"] = "It is a Yellow Warning";
//  doc_0["wlTime"] = "04/6/2022 | 4:00 PM";
//  doc_0["wlMonth"] = "April";
//  doc_0["wlColor"] = "Yellow";
//
//  String theData;
//  serializeJson(doc, theData); //converting the data above in JSON format
//  Serial.print("The Data: ");
//  Serial.println(theData);     //print the converted data in JSON format
//
//  //TESTING POST
//  String apiUrl = "https://floodwatchbackend.herokuapp.com/flwaterlevelpost";
//
//  Serial.println("TESTING POST.. connecting HTTPS....");
//  
// //-------------------------------------------------------------------------
//  //Declare an object of class HTTPClient
//  HTTPClient http;
// 
//  if (WiFi.status() == WL_CONNECTED) {
//    //Specify request destination
//    http.begin(httpsClient, apiUrl);
//    http.addHeader("Content-Type", "application/json");
//  
//   //Send the request
////   int httpCode = http.POST(theData);
//   int httpCode = http.POST("{\"wlLevel\":\"Level 1\",\"wlInfo\":\"GREEN WARNING\",\"wlTime\":\"04/22/2022 | 6:57 PM\",\"wlMonth\":\"April\",\"wlColor\":\"Green\"}");
//   
//
//   //Check the returning code
//   if (httpCode > 0) { 
//     //Get the request response payload
//     String payload = http.getString();
//     //Print the response payload
//     Serial.println(payload);
//   }else{ //the code is negative ex: -11
//     //Close connection
//     Serial.println(httpCode);
//     http.end();
//   }
//    
//  }
////-------------------------------------------------------------------------
//  
//}
//
//void loop() {
//
//
//}
////--------------------------------------------------------------------------------------







//DO NOT INCLUDE THIS BLOCKS OF CODE BELOW=========================================================================================
//for GET
//================================================================================================

////------- Replace the following! ------
//char ssid[] = "GlobeAtHome_8B94E";       // your network SSID (name)
//char password[] = "GLRE3HT1Q16";  // your network key
//
//// For Non-HTTPS requests
//// WiFiClient client;
//
//// For HTTPS requests
//WiFiClientSecure client;
//
//
//// Just the base of the URL you want to connect to
//#define TEST_HOST "floodwatchbackend.herokuapp.com"
//
//// OPTIONAL - The finferprint of the site you want to connect to.
//#define TEST_HOST_FINGERPRINT "39a9c4feb17e239e2f4dbbace8d7a34fce430e7b"
//// The finger print will change every few months.
//
//
//void setup() {
//
//  Serial.begin(115200);
//
//  // Connect to the WiFI
//  WiFi.mode(WIFI_STA);
//  WiFi.disconnect();
//  delay(100);
//
//  // Attempt to connect to Wifi network:
//  Serial.print("Connecting Wifi: ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  IPAddress ip = WiFi.localIP();
//  Serial.println(ip);
//
//  //-------------------
//
//  // If you don't need to check the fingerprint
//  // client.setInsecure();
//
//  // If you want to check the fingerprint
//  client.setFingerprint(TEST_HOST_FINGERPRINT);
//
//  makeHTTPRequest();
//  
//}
//
//void makeHTTPRequest() {
//
//  // Opening connection to server (Use 80 as port if HTTP)
//  if (!client.connect(TEST_HOST, 443))
//  {
//    Serial.println(F("Connection failed"));
//    return;
//  }
//
//  // give the esp a breather
//  yield();
//
//  // Send HTTP request
//  client.print(F("GET "));
//  // This is the second half of a request (everything that comes after the base URL)
//  client.print("/flwaterlevelpost"); // %2C == ,
//  client.println(F(" HTTP/1.1"));
//
//  //Headers
//  client.print(F("Host: "));
//  client.println(TEST_HOST);
//
//  client.println(F("Cache-Control: no-cache"));
//
//  if (client.println() == 0)
//  {
//    Serial.println(F("Failed to send request"));
//    return;
//  }
//  //delay(100);
//  // Check HTTP status
//  char status[32] = {0};
//  client.readBytesUntil('\r', status, sizeof(status));
//  if (strcmp(status, "HTTP/1.1 200 OK") != 0)
//  {
//    Serial.print(F("Unexpected response: "));
//    Serial.println(status);
////    return;
//  }
//
//  // Skip HTTP headers
//  char endOfHeaders[] = "\r\n\r\n";
//  if (!client.find(endOfHeaders))
//  {
//    Serial.println(F("Invalid response"));
//    return;
//  }
//
//
//    // While the client is still availble read each
//    // byte and print to the serial monitor
//   while (client.available()) {
//     char c = 0;
//     client.readBytes(&c, 1);
//     Serial.print(c);
//   }
//
//  //Use the ArduinoJson Assistant to calculate this:
//
//  //StaticJsonDocument<192> doc;
//  DynamicJsonDocument doc(192); //For ESP32/ESP8266 you'll mainly use dynamic.
//
//  DeserializationError error = deserializeJson(doc, client);
//  
//  if (error) {
// 
//
//    const char* wlLevel = doc["wlLevel"]; // "Level 3"
//    const char* wlInfo = doc["wlInfo"]; // "It is a Green Warning"
//    const char* wlTime = doc["wlTime"]; // "04/15/2022 | 1:22 AM"
//    const char* wlMonth = doc["wlMonth"]; // "April"
//    const char* wlColor = doc["wlColor"]; // "Green"
//
//    Serial.print("wlLevel:");
//    Serial.println(wlLevel);
//    Serial.print("wlInfo:");
//    Serial.println(wlInfo);
//    Serial.print("wlTime:");
//    Serial.println(wlTime);
//    Serial.print("wlMonth:");
//    Serial.println(wlMonth);
//    Serial.print("wlColor:");
//    Serial.println(wlColor);
//  }
//  
//
// else {
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(error.f_str());
//    return;
//  }
//
//
//
//
//
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//
//}
//====================================================================================================
