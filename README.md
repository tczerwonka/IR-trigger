# IR-trigger
This is an arduino project to use arbitrary IR input and have output control of a 2-channel relay to serve as an externally controllable 12V trigger for audio device usage.

E.g. the Trinnov Altitude32 does not have IR commands for power on/off.  Reports are that WOL is flakey.  It does, however have a 12V trigger input.  

Parts required are any generic arduino board.  I'm using the Arduino Nano because I have spares.  Additionally a 2-channel generic 5V relay board and IR receiver are needed.

12V trigger voltage provided by a simple wall-wart.  Current is limited to approximately 100mA max by a 2W 120 ohm series resistor.

I picked a random device from Sharp for IR commands and the device is used in the universal remote for control.  Device is unimportant as long as it doesn't overlap with something already used in the existing system.

