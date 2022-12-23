#pragma once

class History {

private:
  int counter = 0;
  int eepromIndex = 0;
  int saveEvery; //we want 1 sample every 30min 
  uint8_t sleepTime; //we want 1 sample every 30min 
  int historyOffset;
  const int eepromOffset = 8;
  const int eepromCount = 500;
  const uint8_t savesEveryXMinInEeprom = 60;

public:
  History(uint8_t sleepTime, int tempOffset) {
    this->sleepTime = sleepTime;
    this->historyOffset = tempOffset;
    saveEvery = ((int)savesEveryXMinInEeprom*60)/(int)sleepTime;
  }

  void save(int value) {
    counter++;
    if (counter < saveEvery) {
      return;
    }

    counter = 0;
    int eepromPosition = eepromOffset + eepromIndex;
    uint8_t eepromValue = (value - historyOffset);
    EEPROM.write(eepromPosition, eepromValue);

    Serial.print("Saved on ");
    Serial.print(eepromPosition);
    Serial.print(": ");
    Serial.println(eepromValue);
    
    eepromIndex = (eepromIndex + 1) % eepromCount;
  }

  void print() {
    Serial.print("History: ");
    for(int i = eepromOffset; i < eepromCount; i++) {
      Serial.print(EEPROM.read(i));
      Serial.print(" ");
    }
    Serial.println("");
    Serial.print("Save every ");
    Serial.print((saveEvery*(int)sleepTime)/60);
    Serial.print("min, every ");
    Serial.print(saveEvery);
    Serial.println(" iterations");
  };

};