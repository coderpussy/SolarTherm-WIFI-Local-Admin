#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include "ota-updater.h"

void OTAUpdater::initialize(const char* otaHost, const int otaPort, const char* otaPath, const char* otaVersion) {
  
  t_httpUpdate_return ret = ESPhttpUpdate.update(String(otaHost) + ":" + String(otaPort) + String(otaPath), String(otaVersion));
  
  switch(ret) {
      case HTTP_UPDATE_FAILED:
          Serial.printf("Update Error (%d): %s\r\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;
      case HTTP_UPDATE_NO_UPDATES:
          Serial.println("No update available");
          break;
      case HTTP_UPDATE_OK:
          Serial.println("Update OK");
          break;
  }
}
