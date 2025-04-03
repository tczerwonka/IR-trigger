/*
   modified version of SimpleReceiver.cpp
   This file was part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.

   This program decodes four separate button presses from a random Sharp device
   and sets one of either relay 1 or 2 to on or off.

 ************************************************************************************
   MIT License

   Copyright (c) 2020-2025 Armin Joachimsmeyer

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is furnished
   to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
   PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
   OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 ************************************************************************************
*/

#include <Arduino.h>

//relay pins used
int RY1 = 3;
int RY2 = 4;
int LED1 = 5;
int LED2 = 6;
bool ON1 = false;

/*
   Specify which protocol(s) should be used for decoding.
   If no protocol is defined, all protocols (except Bang&Olufsen) are active.
   This must be done before the #include <IRremote.hpp>
*/
//#define DECODE_DENON  // Includes Sharp
//#define DECODE_JVC
//#define DECODE_KASEIKYO
//#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
//#define DECODE_LG
//#define DECODE_NEC          // Includes Apple and Onkyo. To enable all protocols , just comment/disable this line.
#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER
//#define DECODE_FAST

//#define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
//#define DECODE_HASH         // special decoder for all protocols

//#define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!

//#define DEBUG               // Activate this for lots of lovely debug output from the decoders.

//#define RAW_BUFFER_LENGTH  750 // For air condition remotes it may require up to 750. Default is 200.

/*
   This include defines the actual pin number for pins like IR_RECEIVE_PIN, IR_SEND_PIN for many different boards and architectures
*/
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>  // include the library

void setup() {
  Serial.begin(115200);

  pinMode(RY1, OUTPUT);
  pinMode(RY2, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(RY1, LOW);
  digitalWrite(RY2, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));
}

void loop() {
  /*
       Check if received data is available and if yes, try to decode it.
       Decoded result is in the IrReceiver.decodedIRData structure.

       E.g. command is in IrReceiver.decodedIRData.command
       address is in command is in IrReceiver.decodedIRData.address
       and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
  */
  if (IrReceiver.decode()) {

    /*
           Print a summary of received data
    */
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print extended info
      //IrReceiver.printIRResultRawFormatted(&Serial, true);

      IrReceiver.resume();  // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
    } else {
      IrReceiver.resume();  // Early enable receiving of the next IR frame

      IrReceiver.printIRResultShort(&Serial);
      //IrReceiver.printIRSendUsage(&Serial);
    }
    //Serial.println();



    //Using the Sharp HT-SB602 remote commands
    //really weird -- on the other sofabaton this is the Kaseikyo/Sharp and the numbers don't exist -- so reprogram
    // for this particular application, RY2 should never turn on unless RY1 is on
    // RY2 should turn off after RY1 is turned off

    if (IrReceiver.decodedIRData.command == 0x4B) {
      Serial.println(F("Received command 0x4B - RY1 ON"));
      digitalWrite(RY1, HIGH);
      digitalWrite(LED1, HIGH);
      ON1 = true;
    } else if (IrReceiver.decodedIRData.command == 0x41) {
      Serial.println(F("Received command 0x41 - RY1 OFF"));
      digitalWrite(RY1, LOW);
      digitalWrite(LED1, LOW);
      ON1 = false;
      delay(1000);  //actually stop for 1s
      digitalWrite(RY2, LOW);
      digitalWrite(LED2, LOW);
    } else if (IrReceiver.decodedIRData.command == 0x42) {
      Serial.println(F("Received command 0x42 - RY2 command"));
      if (ON1) {
        //in this case, relay 1 is ON so we can turn on
        Serial.println(F("RY1 currently on so - RY2 ON"));
        digitalWrite(RY2, HIGH);
        digitalWrite(LED2, HIGH);
      }
    } else if (IrReceiver.decodedIRData.command == 0x43) {
      Serial.println(F("Received command 0x43 - RY2 OFF"));
      digitalWrite(RY2, LOW);
      digitalWrite(LED2, LOW);
    }
  }
}
