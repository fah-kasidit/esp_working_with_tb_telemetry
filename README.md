# First attempt to receive data from ThingsBoard

This code enables the ESP32 to wait for telemetry data from Thingsboard.

## ESP32 Configuration

Configure the following values in your code:

1. WiFi connection settings:

    ```cpp
    const char *ssid = "{wifi_name}";
    const char *password = "{wifi_password}";
    ```

2. URL and token configuration:

    ```cpp
    const char *url = "http://{thingsboard_server}:{port}/api/plugins/telemetry/DEVICE/{device_ID}/values/timeseries?keys={name_of_value}";
    const char *token = "{token}";
    ```

    - Replace `{thingsboard_server}` and `{port}` with your ThingsBoard server details.
    - Replace `{device_ID}` with your device ID from ThingsBoard.
    - `{token}` can be obtained using the following command:

      ```bash 
      curl -X POST --header 'Content-Type: application/json' --header 'Accept: application/json' -d '{"username":"{your_thingsboard_username}", "password":"{your_thingsboard_password}"}' 'https://{your_thingsboard_server}/api/auth/login'
      ```

## Testing

To test, run `main.cpp`. To update telemetry values, use the following command:

```bash
curl -v -X POST --data '{"{what_value_you_want}": 100}' http://{your_thingsboard_server}/api/v1/{device_ID}/telemetry --header "Content-Type:application/json"
