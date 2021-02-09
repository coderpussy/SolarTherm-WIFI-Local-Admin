#include <Arduino.h>

#ifndef ota_updater_h
#define ota_updater_h

class OTAUpdater {
 public:
  void initialize(const char* otaHost, const int otaPort, const char* otaPath, const char* otaVersion);
};

#endif /* ota_updater_h */
