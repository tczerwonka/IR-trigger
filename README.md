# IR-trigger
This is an arduino project to use arbitrary IR input and have output control of a 2-channel relay to serve as an externally 
controllable 12V trigger as typically found in audio device power applications.

E.g. the Trinnov Altitude32 does not have IR commands for power on/off.  Reports are that wake-on-LAN is flakey.  This unit does have a 12V trigger input.  

Minimal parts parts required.  Almost any generic Arduino board should be suitable.  
I'm using the Arduino Nano only because I have a supply on-hand, nothing special is needed.  
Additionally a 2-channel generic 5V relay board is used, along with a voltage buck converter and IR receiver.

12V DC trigger voltage provided by a generic wall-wart. Trigger current is limited to approximately 100mA max by a 2W 120 ohm series resistor.

I picked a random non-conflicting device's IR commands for this unit.  That device is then programmed in the universal remote for control.  The device selected is unimportant as long as the remote codes are unique and do not overlap with something already used in the existing system.

![unit internal](./images/ir-to-trigger.png)

Wiring is simple: 

- the 12V wall wart directly powers the arduino Vin and buck converter.  Each 12V trigger output is fed by a 2W 120 ohm series
resistor to limit trigger output current.  The buck converter is set for 5V out and only powers the relays.  The reason
for using the buck converter is to reduce the amount of current on the Arduino regulator. 

- on the Arduino Nano, 
    - the IR input is on pin 2
    - Relay output is pin 3 and 4, for relay 1 and 2
    - LED indicators can be placed on pins 5 and 6.

Operation:

- The first trigger output is expected to power the primary system.  There are separate remote buttons that
are decoded for 'on' and 'off' functions to ensure that intended state is known when a button is pushed.

- The second trigger output could be an amplifier or other accessory.  Again, there are separate remote
buttons that are decoded for 'on' and 'off'.  However this trigger will not activate unless the first
trigger output is ON, and will automatically turn off when the first output turns OFF.  
