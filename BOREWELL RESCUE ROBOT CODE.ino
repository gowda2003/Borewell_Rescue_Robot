#include <IOXhop_FirebaseESP32.h>


#define TRIG_PIN 15
#define ECHO_PIN 4
#define MOISTURE_SENSOR_PIN 32
const int motor1Pin1 = 26;
const int motor1Pin2 = 27;
const int motor2Pin1 = 19;
const int motor2Pin2 = 18;
const int buzzerpin = 13;



float sensorValue;

#define FIREBASE_HOST "borewell-rescue-robot-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "sVTZQ0XzPWIQX2OXbQPlO2cP9e0fjDJPJrXKkFxC"
#define WIFI_SSID "Realme6"
#define WIFI_PASSWORD "123456789"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("WELCOME");
  delay(200);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(buzzerpin, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  delay(200);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(200);
}



void readmoisture() {
  int moistureLevel = analogRead(MOISTURE_SENSOR_PIN); // Read the moisture level
  Serial.print("Moisture Level: ");
  Serial.println(moistureLevel);
  Firebase.setFloat("WaterLevel", moistureLevel);
  delay(200);
  if (moistureLevel > 3000)
  {
    Serial.println("No Water detected ");
  }
  else
  {
    buzzeron();
    Serial.println("Water detected");
    Firebase.setFloat("waterflag", 1);
    delay(200);
    Firebase.setFloat("waterflag", 0);
    delay(200);
  }


}

void readUltrasonic()
{
  long duration, distance;

  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Set the trigger pin high for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin, calculate distance
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // Speed of sound is 0.034 cm/us

  Serial.print("Object Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Firebase.setFloat("Distance", distance);
  delay(200);
  // If an object is within 100 cm, activate the buzzer
  if (distance <= 20)
  {
    Serial.print("Something Found");
    buzzeron();
    Firebase.setFloat("objectflag", 1);
    delay(200);
    Firebase.setFloat("objectflag", 0);
    delay(200);
  }

}

void up()
{
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  delay(1000);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(200);
}

void down()
{
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(200);
}

void jawopen()
{
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  delay(500);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(200);
}

void jawclose()
{
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  delay(500);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(200);
}

void buzzeron()
{
  digitalWrite(buzzerpin, HIGH);
  delay(1000);
  digitalWrite(buzzerpin, LOW);
  delay(100);
}

void readfirebase()
{
  String status1 = Firebase.getString("STATUS");
  Serial.println(status1);
  delay(100);
  if (status1 == "1")
  {
    Serial.println("UP");
    up();

  }
  else if (status1 == "2")
  {
    Serial.println("DOWN");
    down();
  }
  else if (status1 == "4") {
    Serial.println("Jaw OPEN");
    jawopen();
  } else if (status1 == "3")
  {
    Serial.println("Jaw Close");
    jawclose();
  }
  else if (status1 == "5")
  {
    Serial.println("CLEARED");

  }
}
void loop()
{
  readfirebase();
  readmoisture();
  readUltrasonic();
}


