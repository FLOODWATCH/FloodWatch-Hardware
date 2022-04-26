# FloodWatch-Hardware
Arduino program of FloodWatch


Trig = pin number 3
Echo = pin number 4

Piezo = pin number 13

Convertion:

Distance = (speed * Time) / 2

1 meter == 100cm && 39.3701 inch |
2 meter == 200cm && 78.7402 inch |
3 meter == 300cm && 118.11 inch |
4 meter == 400cm && 157.48 inch |


-------------------------------- And we are the FLOODWATCH hehe ----------------------------------

codes for nodeMCU in terms of linking ultrasonic and nodeMCU (dito ko na ilagay sa readme file HAHA recompile na lang natin para d gumulo repo for nodeMCU)

//((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((

const int trigPin = 12; //GPIO 12 = D6
const int echoPin = 14; //GPIO 14 = D5
long duration, inches, cm;


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200); // Starts the serial communication
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

   Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
  
}
//Function for MS to INCHES [ultrasonic function]
long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}
//Function for MS to CM [ultrasonic function]
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
//((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((


