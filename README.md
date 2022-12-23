# arduino-fan-cooler
Small arduino project which runs a fan when a temperature sensor reaches a certain threshold.

I'm using 5V fans controlled by a MOSFET transistor and a thermistor. I'd recommend googling
around to find good schematics to achieve this circuit.

Tested with Arduino IDE 2.0.3 and Arduino Nano (ATmega 168).


## Required libraries

Install libraries through Arduino's library manager:
* LowPower_LowPowerLab - https://github.com/LowPowerLab/LowPower

This library allows to set the AVR chip into power save mode and save some energy while it's idle. 


## PWM / FastPWM

PWM has been deactivated because the fans I use are not PWM compatible. Trying PWM fans also caused 
annoying noises, where I decided to remove PWM code. 