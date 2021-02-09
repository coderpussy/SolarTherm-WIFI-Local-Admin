#include <Arduino.h>

#ifndef localstorage_client_h
#define localstorage_client_h

class LocalStorageClient {
  public:
    void setHostUrl(String hostUrl);
    void setApiKey(String apiKey);
    void setDeviceName(String deviceName);
    void sendTemperature(float temperature, float humidity, float pressure, float altitude, float voltage);

  private:
    String _hostUrl;
    String _apiKey;
    String _deviceName;
};

#endif /* localstorage_client_h */
