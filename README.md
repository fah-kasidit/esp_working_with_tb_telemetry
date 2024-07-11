# First attempt to receive data from ThingsBoard

This code enables the ESP32 to wait for telemetry data from Thingsboard.

## ESP32 Configuration

Configure the following values in your code:

1. WiFi connection settings:

    ```cpp
    const char *ssid = "{wifi_name}";
    const char *password = "{wifi_password}";
    ```

2. User name and password configuration:

    ```cpp
    const char *tb_username = "{your_username}";
    const char *tb_password = "{your_password}";
    ```

3. Thingsboard server and device ID configuration:

    ```cpp
    const char *thingsboard_server = "{Thingsboard_server}";
    const char *device_id = "{device_id}";
    ```

4. Value pulling configuration:

    ```cpp
    const char *request_value[] = {"{value_1}", "{value_2}"};
    String response_value[ {Quantity_of_request_value} ];
    ```

    - Replace `{Quantity_of_request_value}` with quantity of request value.
    - `{Quantity_of_request_value}` must be integer.
    - `response_value[]` will recive the response, respectively.

## Testing

To test, run `main.cpp`. To update telemetry values, use the following command:

```bash
curl -v -X POST --data '"{value_name}": {value}' http://{thingsboard_server}:{port}/api/v1/{device_ID}/telemetry --header "Content-Type:application/json"
```

- Replace `{value_name}` and `{value}` with some data.
- Replace `{thingsboard_server}`, `{port}` and `{device_ID}` with your ThingsBoard server details.
  