#include "fan.h"
#include "config.h"
#include "history.h"
#include <LowPower.h>
#include <DHT.h>

const uint8_t dhtSensorPin = 2;
const uint8_t dhtType = DHT11;
DHT dht(dhtSensorPin, dhtType);
Fan fan;
Config config;
// History history(config.sleepTimeSec, config.tempOffset);

void setup() {
  //following line sets the ID of the device in EEPROM for persistent identification. Do this only once!
  //config.writeDeviceId(DEV_LIVING_ROOM); 

  Serial.begin(9600);
  Serial.println("setup() start");

  config.loadConfigFromEeprom();
  config.print();
  
  fan.init(config);
  fan.setState(true);

  // history.print();

  dht.begin();

  Serial.println("setup() done!");

  sleep();
}

void loop() {
  // history.save(tempSensorValue);
  float tempCelsius = readTempCelsius();
  bool fanValue = calcFanState(tempCelsius);

  fan.setState(fanValue);

  sleep();
}

/**********************************/

float readTempCelsius() {
  float analogSensorTemp = analogSensorTempCelsius();
  float dhtSensorTemp = dhtTempCelsius();

  if (isnan(dhtSensorTemp)) {
    return analogSensorTemp;
  } 
  return dhtSensorTemp;
}

float analogSensorTempCelsius() {
  int analogSensorValue = 0;
  const uint8_t samplesCount = 4;
  for (int i = 0; i < samplesCount; i++) {
    analogSensorValue += analogRead(config.tempSensorPin);
  }
  analogSensorValue /= samplesCount;

  // Define the two data points
  const int analogLow = 351;
  const int analogHigh = 605;
  const float tempLow = 24.4;
  const float tempHigh = 45.3;

  // Calculate the slope (m) and y-intercept (b)
  float slope = (tempHigh - tempLow) / (analogHigh - analogLow);
  float intercept = tempLow - slope * analogLow;

  // Use the formula for a straight line to convert the analog reading to temperature
  float tempCelsius = slope * analogSensorValue + intercept;

  Serial.print("Analog Reading: ");
  Serial.print(analogSensorValue);
  Serial.print(" Temp: ");

  Serial.print(tempCelsius);
  Serial.print("ºC ");

  return tempCelsius; 
}

float dhtTempCelsius() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  bool force = true;
  float h = dht.readHumidity(force);
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature(false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    return t;
  }

  // // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("DHT Humidity: "));
  Serial.print(h);
  Serial.print(F("% Temp: "));
  Serial.print(t);
  // Serial.print(F("°C Heat index: "));
  // Serial.print(hic);
  Serial.print(F("°C"));

  return t;
}

period_t lowPowerPeriodValue() {
  if (config.sleepTimeSec >= 8) {
    return SLEEP_8S;
  }
  if (config.sleepTimeSec >= 4) {
    return SLEEP_4S;
  }
  if (config.sleepTimeSec >= 2) {
    return SLEEP_2S;
  }
  if (config.sleepTimeSec >= 1) {
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

bool calcFanState(float temperatureValue) {
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