#include "config.h"

void Config::loadConfig(uint8_t device) {
  switch (device) {
    case DEV_LIVING_ROOM:
      tempThreshold = 340; // 347-354 ambient
      tempHysteresisValue = 13;
      break;
    case DEV_BEDROOM:
      tempThreshold = 330; // 310-324 ambient; before: 357
      tempHysteresisValue = 18;
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
  Serial.print(" tempOffset:");
  Serial.print(tempOffset);
  Serial.print(" threshold:");
  Serial.print(tempThreshold);
  Serial.print(" hysteresis:");
  Serial.print(tempHysteresisValue);
  Serial.print(" fanPin:");
  Serial.print(fanPin);
  Serial.print(" tempPin:");
  Serial.println(tempSensorPin);

}