#include "fan.h"
#include "config.h"
#include "history.h"
#include <LowPower.h>

Fan fan;
Config config;
History history(config.saveEveryXs, config.tempOffset);

void setup() {
  //following line sets the ID of the device in EEPROM for persistent identification. Do this only once!
  //config.writeDeviceId(DEV_LIVING_ROOM); 

  Serial.begin(9600);
  Serial.println("setup() start");

  history.print();
  config.loadConfigFromEeprom();
  config.print();
  
  fan.init(config);

  Serial.println("setup() done!");
}

void loop() {
  int tempSensorValue = analogRead(config.tempSensorPin);
  history.save(tempSensorValue);
  bool fanValue = calcFanState(tempSensorValue);

  Serial.print("temp:");
  Serial.print(tempSensorValue - config.tempOffset);
  Serial.print(" ");

  fan.setState(fanValue);

  sleep();
}

/**********************************/


period_t lowPowerPeriodValue() {
  if (config.saveEveryXs >= 8) {
    return SLEEP_8S;
  }
  if (config.saveEveryXs >= 4) {
    return SLEEP_4S;
  }
  if (config.saveEveryXs >= 2) {
    return SLEEP_2S;
  }
  if (config.saveEveryXs >= 1) {
    return SLEEP_1S;
  }
  return SLEEP_500MS;
}

void ensureSerialDataIsSent() {
  delay(80);
}

void sleep() {
  ensureSerialDataIsSent();
  LowPower.powerSave(lowPowerPeriodValue(), ADC_OFF, BOD_OFF, TIMER2_OFF);
}

bool calcFanState(int temperatureValue) {
  static bool lastResult = true;
  if (temperatureValue < config.tempThreshold) {
    lastResult = false;
    return lastResult;
  }

  if (!lastResult && temperatureValue < config.tempThreshold + config.tempHysteresisValue) {
    return lastResult;
  }

  lastResult = true;

  return lastResult;
}