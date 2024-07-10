#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "Home-2_2.4G";
const char *password = "333555679";

const char *serverUrl = "http://143.198.195.172:9090/api/plugins/telemetry/DEVICE/fe9a0f60-38ff-11ef-bff0-835a0b74444f/values/timeseries?keys=distance";
const char *token = "eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJmYWgua2FzaWRpdEBnbWFpbC5jb20iLCJ1c2VySWQiOiI1MGI2ZmUwMC0zOGU5LTExZWYtYmZmMC04MzVhMGI3NDQ0NGYiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6ImU2MDViNzIxLTlhMjItNDQ4NC04ZWE5LTJmMGJjODU0NWUyZCIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzIwNjAwMzE3LCJleHAiOjE3MjA2ODY3MTcsImZpcnN0TmFtZSI6Ikthc2lkaXQiLCJsYXN0TmFtZSI6IlBvc2FwaGFuIiwiZW5hYmxlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6IjZhYjUwOWMwLTM4NjAtMTFlZi1iZmYwLTgzNWEwYjc0NDQ0ZiIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.ZRkFMpWgonTq-MsBOD7Mcw08HIzALRATaKeo18V5xnoZp-fvTemRTMI_x_EEtvBzT2sNZ4DQbxYituuoQfu4Jg";

void fetchDistance()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Authorization", String("Bearer ") + token);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      String payload = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response payload: " + payload);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      float distance = doc["distance"][0]["value"].as<float>();
      Serial.println("Distance: " + String(distance));
    }
    else
    {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  fetchDistance();
}

void loop()
{
}