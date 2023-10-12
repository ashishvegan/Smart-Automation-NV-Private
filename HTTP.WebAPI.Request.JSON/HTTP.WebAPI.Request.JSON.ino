#include<ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
HTTPClient http;
const char* ssid = "Vegan";
const char* password = "veganpower";
String URL = "http://smartautomationiot-api.azurewebsites.net/WeatherForecast";

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting..");
    delay(1000);
  }
  Serial.println("Connected to Wi-Fi");
}
void loop()
{
  http.begin(URL);
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    value = http.getString();
  }
  Serial.println(value);
  http.end();
  delay(10000);
}
