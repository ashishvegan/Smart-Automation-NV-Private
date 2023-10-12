#define RX D7
#define TX D8
#include<ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include<SoftwareSerial.h>
SoftwareSerial master(RX, TX);
const char* ssid = "Vegan";
const char* password = "veganpower";
const int RedLed = D0, GreenLed = D1;
const int WaterSupplySW = D2;
const int InternetSW = D4;
void setup()
{
  Serial.begin(9600);
  master.begin(9600);
  pinMode(RedLed, OUTPUT); // D0
  pinMode(GreenLed, OUTPUT); // D1
  pinMode(WaterSupplySW, OUTPUT); // D2
  pinMode(InternetSW, OUTPUT); // D4
  digitalWrite(WaterSupplySW, LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting..");
    digitalWrite(GreenLed, LOW);
    digitalWrite(InternetSW, LOW);
    digitalWrite(RedLed, HIGH);
    delay(500);
    digitalWrite(RedLed, LOW);
    delay(500);
  }
  Serial.println("Connected");
  digitalWrite(GreenLed, HIGH);
  digitalWrite(InternetSW, HIGH);
}
void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    MainLoop();
    delay(500);
  }
  else
  {
    WiFi.begin(ssid, password);
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Connecting..");
      digitalWrite(GreenLed, LOW);
      digitalWrite(InternetSW, LOW);
      digitalWrite(RedLed, HIGH);
      delay(500);
      digitalWrite(RedLed, LOW);
      delay(500);
    }
    else
    {
      Serial.println("Connected");
      digitalWrite(GreenLed, HIGH);
      digitalWrite(InternetSW, HIGH);
    }
  }
}
void MainLoop()
{
  if (master.available())
  {
    Serial.println(master.readString());
  }
  HTTPClient http;
  http.begin("http://172.20.10.3:5212/WeatherForecast");
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    value = http.getString();
    Serial.println(value);
  }
  delay(500);
}
