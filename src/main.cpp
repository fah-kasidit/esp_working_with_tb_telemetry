#include <WiFi.h>
#include <iostream>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "NDRS-Wongsawang";
const char *password = "ndrs_2010";

const char *tb_username = "fah.kasidit@gmail.com";
const char *tb_password = "tni-ksd";

const char *thingsboard_server = "http://143.198.195.172:9090";
const char *device_id = "fe9a0f60-38ff-11ef-bff0-835a0b74444f";

const char *request_value[] = {"distance", "status"};

// Iterate through the request_value array
for (int i = 0; i < sizeof(request_value) / sizeof(request_value[0]); i++) {
  const char *item = request_value[i];
  printf("Item %d: %s\n", i + 1, item);
}

char url[256];
char *token = NULL;

String payload;
int http_response_code;
int http_request_code;
int n_request_value = 0;
int temp_value = 0;


int build_request_url()
{
  snprintf(url, sizeof(url), "%s/api/plugins/telemetry/DEVICE/%s/values/timeseries?keys=", thingsboard_server, device_id);
  for (const auto &str : request_value)
  {
    strcat(url, str);
    strcat(url, ",");
    n_request_value++;
  }
  url[strlen(url) - 1] = '\0';

  return n_request_value;
}

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

void fetch_token()
{
  HTTPClient http;
  http.begin(String(thingsboard_server) + "/api/auth/login");
  http.addHeader("Content-Type", "application/json");

  // POST payload
  StaticJsonDocument<200> loginJson;
  loginJson["username"] = tb_username;
  loginJson["password"] = tb_password;
  serializeJson(loginJson, payload);
  http_request_code = http.POST(payload);

  // Just in case
  if (token != NULL)
  {
    free(token);
  }

  if (http_request_code > 0)
  {
    String response = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, response);

    token = strdup(doc["token"].as<String>().c_str());
    // Serial.println("\nToken received: " + String(token) + "\n");
  }
  else
  {
    Serial.print("Error getting token. HTTP error code: ");
    Serial.println(http_request_code);
  }
  http.end();
}

void fetch_data(void *pvParameters)
{
  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;
      http.begin(url);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-Authorization", String("Bearer ") + token);

      http_response_code = http.GET();

      if (http_response_code > 0)
      {
        payload = http.getString();
        // Serial.println("HTTP Response code: " + String(http_response_code));
        // Serial.println("Response payload: " + payload);

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        for (temp_value = 0; temp_value < n_request_value; temp_value++)
        {
          response_value[temp_value] = doc[request_value[temp_value]][0]["value"].as<String>();
          Serial.print(request_value[temp_value]);
          Serial.println(": " + response_value[temp_value]);
        }
        Serial.println("");
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
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  connect_to_wifi();
  build_request_url();
  fetch_token();

  xTaskCreate(fetch_data, "Fetch Data", 4096, NULL, 1, NULL);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
    connect_to_wifi();
}