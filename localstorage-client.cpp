#include <ESP8266HTTPClient.h>  // To send HTTP requests.

#include "localstorage-client.h"

void LocalStorageClient::setHostUrl(String hostUrl) {
  _hostUrl = hostUrl;
}

void LocalStorageClient::setApiKey(String apiKey) {
  _apiKey = apiKey;
}

void LocalStorageClient::setDeviceName(String deviceName) {
  _deviceName = deviceName;
}

void LocalStorageClient::sendTemperature(float temperature, float humidity, float pressure, float altitude, float voltage) {
  String url = _hostUrl + "/remote-temperature.php";
  Serial.println("LocalStorageClient: Sending POST request to LocalStorage at " + url);

  String requestBody = "{ \"apikey\": \"" + _apiKey + "\", \"name\": \"" + String(_deviceName) + "\", \"temperature\": \"" + String(temperature) + "\", \"humidity\": \"" + String(humidity) + "\", \"pressure\": \"" + String(pressure) + "\", \"altitude\": \"" + String(altitude) + "\", \"battery\": \"" + String(voltage) + "\" }";
  Serial.println("LocalStorageClient: HTTP request body: " + requestBody);

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int statusCode = http.POST(requestBody);

  Serial.printf("LocalStorageClient: Received HTTP status code: %d\r\n", statusCode);
  if (statusCode != HTTP_CODE_OK) {
    String responseBody = http.getString();
    Serial.println("LocalStorageClient: Received HTTP response body: " + responseBody);
  }

  http.end();
}
