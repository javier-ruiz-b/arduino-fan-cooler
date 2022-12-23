#pragma once

#define DEV_LIVING_ROOM 0
#define DEV_BEDROOM 1

#define EEPROM_CONFIG_POSITION 0
#include <EEPROM.h>
#include <Arduino.h>

class Config {
public:
  const uint8_t version = 1;
  const uint8_t tempSensorPin = A0;
  const uint8_t fanPin = 6;
  const uint8_t saveEveryXs = 8;
  const int tempOffset = 200;

  int tempThreshold;
  uint8_t tempHysteresisValue = 13;

private:
  void loadConfig(uint8_t device);
  uint8_t deviceId();

public:
  void loadConfigFromEeprom();
  void writeDeviceId(uint8_t deviceId);
  void print();

};