#include <Arduino_JSON.h>




#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

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





const char* ssid = "GlobeAtHome_8B94E";
const char* password = "GLRE3HT1Q16";

const char *host = "floodwatchbackend.herokuapp.com";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80

#define TEST_HOST_FINGERPRINT "39a9c4feb17e239e2f4dbbace8d7a34fce430e7b"

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  WiFiClientSecure httpsClient;

  // If you want to check the fingerprint
  httpsClient.setFingerprint(TEST_HOST_FINGERPRINT);

  //TESTING JSON CREATION
  Serial.println("Starting JSON.. Converting data to JSON (Serialization).....");
  
  //char json[] = "{\"wlLevel\":\"Level 2\",\"wlInfo\":\"It is a Yellow Warning\",\"wlTime\":\"04/21/2022 | 4:00 PM\",\"wlMonth\":\"April\",\"wlColor\":\"Yellow\"}";
  DynamicJsonDocument doc(192);
  
  JsonObject doc_0 = doc.createNestedObject();
  doc_0["wlLevel"] = "Level 2";
  doc_0["wlInfo"] = "It is a Yellow Warning";
  doc_0["wlTime"] = "04/6/2022 | 4:00 PM";
  doc_0["wlMonth"] = "April";
  doc_0["wlColor"] = "Yellow";

  String theData;
  serializeJson(doc, theData); //converting the data above in JSON format
  Serial.print("The Data: ");
  Serial.println(theData);     //print the converted data in JSON format

  //TESTING POST
  String apiUrl = "https://floodwatchbackend.herokuapp.com/flwaterlevelpost";

  Serial.println("TESTING POST.. connecting HTTPS....");
  
 //-------------------------------------------------------------------------
  //Declare an object of class HTTPClient
  HTTPClient http;
 
  if (WiFi.status() == WL_CONNECTED) {
    //Specify request destination
    http.begin(httpsClient, apiUrl);
    http.addHeader("Content-Type", "application/json");
  
   //Send the request
//   int httpCode = http.POST(theData);
   int httpCode = http.POST("{\"wlLevel\":\"Level 1\",\"wlInfo\":\"GREEN WARNING\",\"wlTime\":\"04/22/2022 | 6:57 PM\",\"wlMonth\":\"April\",\"wlColor\":\"Green\"}");
   

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
//-------------------------------------------------------------------------
  
}

void loop() {


}
