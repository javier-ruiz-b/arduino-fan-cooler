#include "config.h"

void Config::loadConfig(uint8_t device) {
  switch (device) {
    case DEV_LIVING_ROOM:
      tempThreshold       = 24.5; // ºC
      tempHysteresisValue =  2.5; // ºC
      break;
    case DEV_BEDROOM:
      tempThreshold       = 24.0; // ºC
      tempHysteresisValue =  2.5; // ºC
      break;
    default:
      Serial.print("ERR Unknown device: ");
      Serial.println(device);
  }
}

uint8_t Config::deviceId() {
  return EEPROM.read(EEPROM_CONFIG_POSITION);
}

void Config::loadConfigFromEeprom() {
  loadConfig(deviceId());
}

void Config::writeDeviceId(uint8_t deviceId) {
  EEPROM.write(EEPROM_CONFIG_POSITION, deviceId);
}

void Config::print() {
  Serial.print("Ver:");
  Serial.print(version);
  Serial.print(" ID:");
  Serial.print(deviceId());
  Serial.print(" threshold:");
  Serial.print(tempThreshold);
  Serial.print(" hysteresis:");
  Serial.print(tempHysteresisValue);
  Serial.print(" sleepTimeSec:");
  Serial.print(sleepTimeSec);
  Serial.print(" fanPin:");
  Serial.print(fanPin);
  Serial.print(" tempPin:");
  Serial.print(tempSensorPin);
}