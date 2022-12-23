/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   https://forum.arduino.cc/index.php?topic=16612#msg121031
 */
#ifdef ARDUINO_AVR_NANO
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

#elif ARDUINO_attiny
void setPwmFrequency(int pin, int divisor) {
  /*
  Control Register A for Timer/Counter-0 (Timer/Counter-0 is configured using two registers: A and B)
  TCCR0A is 8 bits: [COM0A1:COM0A0:COM0B1:COM0B0:unused:unused:WGM01:WGM00]
  2<<COM0A0: sets bits COM0A0 and COM0A1, which (in Fast PWM mode) clears OC0A on compare-match, and sets OC0A at BOTTOM
  2<<COM0B0: sets bits COM0B0 and COM0B1, which (in Fast PWM mode) clears OC0B on compare-match, and sets OC0B at BOTTOM
  3<<WGM00: sets bits WGM00 and WGM01, which (when combined with WGM02 from TCCR0B below) enables Fast PWM mode
  */
  TCCR0A = 2 << COM0A0 | 2 << COM0B0 | 3 << WGM00;

  /*
  Control Register B for Timer/Counter-0 (Timer/Counter-0 is configured using two registers: A and B)
  TCCR0B is 8 bits: [FOC0A:FOC0B:unused:unused:WGM02:CS02:CS01:CS00]
  0<<WGM02: bit WGM02 remains clear, which (when combined with WGM00 and WGM01 from TCCR0A above) enables Fast PWM mode
  1<<CS00: sets bits CS01 (leaving CS01 and CS02 clear), which tells Timer/Counter-0 to not use a prescalar
  */
  TCCR0B = 0 << WGM02 | 1 << CS00;

  /*
  Control Register for Timer/Counter-1 (Timer/Counter-1 is configured with just one register: this one)
  TCCR1 is 8 bits: [CTC1:PWM1A:COM1A1:COM1A0:CS13:CS12:CS11:CS10]
  0<<PWM1A: bit PWM1A remains clear, which prevents Timer/Counter-1 from using pin OC1A (which is shared with OC0B)
  0<<COM1A0: bits COM1A0 and COM1A1 remain clear, which also prevents Timer/Counter-1 from using pin OC1A (see PWM1A above)
  1<<CS10: sets bit CS11 which tells Timer/Counter-1  to not use a prescalar
  */
  TCCR1 = 0 << PWM1A | 0 << COM1A0 | 1 << CS10;

  /*
  General Control Register for Timer/Counter-1 (this is for Timer/Counter-1 and is a poorly named register)
  GTCCR is 8 bits: [TSM:PWM1B:COM1B1:COM1B0:FOC1B:FOC1A:PSR1:PSR0]
  1<<PWM1B: sets bit PWM1B which enables the use of OC1B (since we disabled using OC1A in TCCR1)
  2<<COM1B0: sets bit COM1B1 and leaves COM1B0 clear, which (when in PWM mode) clears OC1B on compare-match, and sets at BOTTOM
  */
  GTCCR = 1 << PWM1B | 2 << COM1B0;
}

#else
#error Arduino not supported!
#endif

void setFastPwm(int pin) {
#ifdef ARDUINO_AVR_NANO
  // Set pin's PWM frequency to 62500 Hz (62500/1 = 62500)
  // Note that the base frequency for pins 5 and 6 is 62500 Hz
  setPwmFrequency(pin, 1);
#elif ARDUINO_attiny
#endif
}