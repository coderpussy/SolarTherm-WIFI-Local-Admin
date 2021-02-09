#include "bme280.h"

void BME280::init() {
  bool bme_status;
  bme_status = _bme.begin(DEFAULT_ADDRESS);  //address either 0x76 or 0x77
  if (!bme_status) {
    Serial.println("BME280: Could not find a valid BME280 sensor, check wiring!");
    Serial.print("SensorID was: 0x"); Serial.println(_bme.sensorID(),16);
  }
}

float BME280::getTemperature() {
  return _bme.readTemperature();
}

float BME280::getPressure() {
  return _bme.readPressure() / 100.0F;
}

float BME280::getAltitude() {
  return _bme.readAltitude(SEALEVELPRESSURE_HPA);
}

float BME280::getHumidity() {
  return _bme.readHumidity();
}

BME280::Measurement BME280::getMeasuredData() {
  Measurement measurement;

  measurement.temperature = getTemperature();
  measurement.pressure = getPressure();
  measurement.altitude = getAltitude();
  measurement.humidity = getHumidity();

  return measurement;
}
