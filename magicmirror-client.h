#include <Arduino.h>

#ifndef magicmirror_client_h
#define magicmirror_client_h

class MagicMirrorClient {
  public:
    void setHostUrl(String hostUrl);
    void setSensorId(String sensorId);
    void sendTemperature(float temperature, float humidity, float voltage);

  private:
    String _hostUrl;
    String _sensorId;
};

#endif /* magicmirror_client_h */
