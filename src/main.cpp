#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String payload;
int httpResponseCode;
float distance;

const char *ssid = "NDRS-Wongsawang";
const char *password = "ndrs_2010";

const char *url = "http://143.198.195.172:9090/api/plugins/telemetry/DEVICE/fe9a0f60-38ff-11ef-bff0-835a0b74444f/values/timeseries?keys=distance";
const char *token = "eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJmYWgua2FzaWRpdEBnbWFpbC5jb20iLCJ1c2VySWQiOiI1MGI2ZmUwMC0zOGU5LTExZWYtYmZmMC04MzVhMGI3NDQ0NGYiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6ImU2MDViNzIxLTlhMjItNDQ4NC04ZWE5LTJmMGJjODU0NWUyZCIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzIwNjAwMzE3LCJleHAiOjE3MjA2ODY3MTcsImZpcnN0TmFtZSI6Ikthc2lkaXQiLCJsYXN0TmFtZSI6IlBvc2FwaGFuIiwiZW5hYmxlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6IjZhYjUwOWMwLTM4NjAtMTFlZi1iZmYwLTgzNWEwYjc0NDQ0ZiIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.ZRkFMpWgonTq-MsBOD7Mcw08HIzALRATaKeo18V5xnoZp-fvTemRTMI_x_EEtvBzT2sNZ4DQbxYituuoQfu4Jg";

void connect_to_wifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void fetch_distance(void *pvParameters)
{
  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;
      http.begin(url);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-Authorization", String("Bearer ") + token);

      httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        payload = http.getString();
        // Serial.println("HTTP Response code: " + String(httpResponseCode));
        // Serial.println("Response payload: " + payload);

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        distance = doc["distance"][0]["value"].as<float>();
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
      connect_to_wifi();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  connect_to_wifi();

  xTaskCreate(fetch_distance, "Fetch Distance", 4096, NULL, 1, NULL);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
    connect_to_wifi();
}