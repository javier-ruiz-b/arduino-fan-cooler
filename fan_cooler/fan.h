#include "pwm.h"
#include "Config.h"

class Fan {
private:
  Config *m_config;

public:
  Fan() {}

  void init(const Config &config) {
    pinMode(config.fanPin, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    m_config = &config;
  }

  void setState(bool on) {
    Serial.print(" Fan: ");
    Serial.println(on ? "on": "off");
    digitalWrite(m_config->fanPin, on);
    digitalWrite(LED_BUILTIN, on);
  }
  
};