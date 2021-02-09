const char* APP_VERSION = "0.0.1";

const char* WIFI_AP_SSID = "SOLAR_THERM_AP";                                                            // The name of the wireless network to create if cannot connect using the previously saved credentials.
const char* WIFI_AP_PASSWORD = "MYPASS";                                                                // The password required to connect to the wireless network used to configure the network parameters.
const unsigned long WIFI_CONFIG_PORTAL_TIMEOUT_SECONDS = 60;                                            // The number of seconds the WiFi configuration page waits for user interaction before restarting the device and try again.

const char* OTA_UPDATE_HOST = "http://solartherm.dyndns-what-ever.net";                                 // The FQDN or IP address of the OTA update server on the network.
const int OTA_UPDATE_PORT = 80;                                                                         // Port of OTA update Server
const char* OTA_UPDATE_SCRIPT_PATH = "/solartherm-ota/ota-update.php";                                  // Path to OTA update script
const char* OTA_UPDATE_VERSION = "livingroom";                                                          // Determine correct device version on update

const char* LOCAL_STORAGE_HOST = "http://solartherm.dyndns-what-ever.net";                              // The FQDN or IP address of the local storage device on the network.
const char* LOCAL_STORAGE_HOSTNAME = "livingroom";                                                      // The name of the device as it should appear in the local storage output e.g. webpage.
const char* LOCAL_STORAGE_API_KEY = "MY_VERY_OWN_PASSWORD_HASH";                                        // The API key with write permission post data to a local storage. Generated with PHP $hash = password_hash("S47TnP3pp3R", PASSWORD_DEFAULT);

const char* MAGIC_MIRROR_HOST = "http://192.168.100.234:8080";                                          // The IP address and port number of the MagicMirror device on the network.
const char* MAGIC_MIRROR_SENSORID = "livingroom";                                                       // The name of the device as it should appear in the magic mirror output.

float UPDATE_INTERVAL_SECONDS = 600;                                                                    // The frequency of measuring data, update interval and sending updates to LocalStorage or MagicMirror in seconds.
