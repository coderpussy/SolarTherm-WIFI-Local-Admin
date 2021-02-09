/*----------------------------------------------------------------------------------------------------
  Project Name : SolarTherm WIFI Local Admin aka. Solar Powered WiFi Weather Station V2
  Features: temperature, humidity, pressure, altitude and battery status
  Authors: Keith Hungerford, Debasish Dutta
  Website : www.opengreenenergy.com
  
  Main microcontroller (ESP8266) and BME280 both sleep between measurements
  CODE: https://github.com/coderpussy/SolarTherm-WIFI-Local-Admin
  INSTRUCTIONS & HARDWARE: https://www.instructables.com/id/Solar-Powered-WiFi-Weather-Station-V20/

  CREDITS:
  
  Inspiration, code fragments and document parts are taken from:
  https://github.com/3KUdelta/Solar_WiFi_Weather_Station
  https://github.com/balassy/solar-wifi-weather-station
  https://github.com/esp8266/Arduino/blob/master/doc/ota_updates/readme.rst
  
  Needed libraries:
  <Adafruit_Sensor.h>    --> Adafruit unified sensor
  <Adafruit_BME280.h>    --> Adafrout BME280 sensor
  <ESP8266WiFi.h>        --> To connect to the WiFi network.
  <WiFiManager.h>        --> To manage network configuration and connection.
  <ESP8266HTTPClient.h>  --> To send HTTP requests.
  <ESP8266httpUpdate.h>  --> To get new binary update

  CREDITS for Adafruit libraries:
  
  This is a library for the BME280 humidity, temperature & pressure sensor
  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650
  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.
  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!
  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

////  Features :  //////////////////////////////////////////////////////////////////////////////////////////////////////
                                                                                                                         
// 1. Connect to Wi-Fi, and upload the data to either local network storage or webspace
// 2. Monitoring Weather parameters like Temperature, Pressure, Humidity.
// 3. Over The Air device update possible with ESP8266 http Update functionality
// 4. Extra Ports to add more Weather Sensors like UV Index, Light and Rain Guage etc.
// 5. Remote Battery Status Monitoring
// 6. Using Sleep mode to reduce the energy consumed

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/***************************************************
 * IMPORTANT:                                      *
 * All settings located in config.h !              *
 **************************************************/

// Platform libraries.
#include <Arduino.h>           // To add IntelliSense for platform constants.
#include <ESP8266WiFi.h>       // To connect to the WiFi network.

// Third-party libraries.
#include <WiFiManager.h>  // To manage network configuration and connection.

// My classes.
#include "bme280.h"              // To read the temperature, pressure and humidity sensor.
#include "localstorage-client.h" // To send measured data to a local database.
#include "magicmirror-client.h"  // To manage the communication with the MagicMirror.
#include "ota-updater.h"         // To manage over-the-air updates trough web server script.

#include "config.h"  // To store configuration and secrets (uncomment for config-sz.h).

WiFiManager wifiManager;
BME280 bme;

LocalStorageClient localStorage;
MagicMirrorClient magicMirror;
OTAUpdater updater;

void setup() {
  initSerial();

  initNetwork();

  initLocalStorageClient();
  initMagicMirrorClient();
  initTemperatureSensor();

  sendStartNotification();

  Serial.printf("Application version: %s\n", APP_VERSION);
  Serial.println("Setup completed.");

  initUpdater();
  
  measureAndUpdateTargets();

  goToDeepSleep();
}

void loop() {
  // No code here, because all logic is running in the setup phase and then the device goes to deep sleep.
  // After waking up the setup phase is executed again.
}

void initSerial() {
  Serial.begin(115200); // 115200 is the speed used to print boot messages.
  Serial.setDebugOutput(false); // Show debug output
  Serial.println();
  Serial.println("Initializing serial connection DONE.");
}

void initNetwork() {
  Serial.printf("Initializing connection to the network with MAC address %s using WiFiManager (SSID: %s)...\n", WiFi.macAddress().c_str(), WIFI_AP_SSID);
  wifiManager.setAPCallback([&](WiFiManager *mgr) {
  });
  wifiManager.setSaveConfigCallback([&]() {
  });

  // Configuration portal timeout and automatic restart is required because sometimes the device does not find the
  // network credentials after booting up, but after a restart it can connect to the network without any issue.
  // This ensures that the device retries connecting to the network without waiting for user interaction.
  wifiManager.setConfigPortalTimeout(WIFI_CONFIG_PORTAL_TIMEOUT_SECONDS);

  if (!wifiManager.autoConnect(WIFI_AP_SSID, WIFI_AP_PASSWORD)) {
    Serial.println("Failed to connect to the network and the WiFi configuration portal hit inactivity timeout. Restarting the device in 3 seconds and trying again...");
    delay(3000);

    // The restart() function triggers a more clean reboot than reset(), so this one is the preferred.
    // Read more: https://www.pieterverhees.nl/sparklesagarbage/esp8266/130-difference-between-esp-reset-and-esp-restart
    ESP.restart();
  }

  Serial.printf("DONE. IP address: %s, MAC address: %s\n", WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());
}

void initLocalStorageClient() {
  Serial.printf("Initializing LocalStorage client to host %s...", LOCAL_STORAGE_HOST);
  localStorage.setHostUrl(LOCAL_STORAGE_HOST);
  localStorage.setApiKey(LOCAL_STORAGE_API_KEY);
  localStorage.setDeviceName(LOCAL_STORAGE_HOSTNAME);
  Serial.println("DONE.");
}

void initMagicMirrorClient() {
  Serial.printf("Initializing MagicMirror client to host %s...", MAGIC_MIRROR_HOST);
  magicMirror.setHostUrl(MAGIC_MIRROR_HOST);
  magicMirror.setSensorId(MAGIC_MIRROR_SENSORID);
  Serial.println("DONE.");
}

void initTemperatureSensor() {
  Serial.print("Initializing the temperature sensor...");
  bme.init();
  Serial.println("DONE.");
}

void initUpdater() {
  Serial.print("Initializing Over-The-Air Updater...");
  updater.initialize(OTA_UPDATE_HOST, OTA_UPDATE_PORT, OTA_UPDATE_SCRIPT_PATH, OTA_UPDATE_VERSION);
  Serial.println("DONE.");
}

void sendStartNotification() {
  // Send start notification only if the device has not waken up from deep sleep,
  // so the start happened unexpectedly.

  String resetReason = ESP.getResetReason();
  Serial.println("Reset reason: " + resetReason);

  if (resetReason != "Deep-Sleep Wake") {
    Serial.println("Sending notification about unexpected start...");
    String message = String("Your device is starting. Reason: ") +  resetReason + " (" + ESP.getResetInfo() + ")";
    Serial.println("DONE.");
  }
}

void measureAndUpdateTargets() {
  Serial.println("Measuring...");
  
  Serial.print("Reading sensor data...");
  BME280::Measurement m = bme.getMeasuredData();
  Serial.println(" DONE.");

  Serial.printf("Temperature: %.1f °C\n", m.temperature);
  Serial.printf("Humidity: %.1f %%\n", m.humidity);
  Serial.printf("Pressure: %.1f hPa\n", m.pressure);
  Serial.printf("Altitude: %.1f m\n", m.altitude);

  Serial.print("Reading battery voltage... ");
  float voltage = readBatteryVoltage();
  Serial.print(voltage);
  Serial.println("V DONE.");

  Serial.println("Sending data to LocalStorage...");
  localStorage.sendTemperature(m.temperature, m.humidity, m.pressure, m.altitude, voltage);
  Serial.println("DONE.");
  
  Serial.println("Sending data to MagicMirror...");
  magicMirror.sendTemperature(m.temperature, m.humidity, voltage);
  Serial.println("DONE.");

  Serial.println("Measuring: DONE.");
}

float readBatteryVoltage() {
  // Voltage divider R1 = 220k+100k+220k =540k and R2=100k
  float calib_factor = 5.28; // change this value to calibrate the battery voltage
  unsigned long raw = analogRead(A0);
  float volt = raw * calib_factor/1024; 
  return volt;
}

void goToDeepSleep() {
  delay(50);
  
  Serial.printf("Going to Deep-Sleep for %.0f seconds...\n", UPDATE_INTERVAL_SECONDS);
  //pinMode(D0, WAKEUP_PULLUP);
  ESP.deepSleep(UPDATE_INTERVAL_SECONDS * 1e6); // Microseconds.
}
