/*------------------------------------------------------------------------------

  Send_SMS.ino
  Brief: 
  Description: 
  Author: Oliver Padolina
  Repository: 
  Circuit:
            Hardware Serial:
            Connect Serial TX -> GSM Module TX
            Connect Serial RX -> GSM Module RX

            Software Serial:
            Connect Arduino Pin 10 -> GSM Module TX
            Connect Arduino Pin 11 -> GSM Module RX

            Software Serial:
            Connect Node MCU Pin 12 -> GSM Module TX
            Connect Node MCU Pin 14 -> GSM Module RX

  ----------------------------------------------------------------------------*/

#include <O_GSM.h>

/*------------------------------------------------------------------------------
  GLOBAL VARIABLES
  ----------------------------------------------------------------------------*/

// Number
const char *number = "09XXXXXXXXX";

// Serial
SoftwareSerial gsmSS(11, 10); // Arduino Pins
//SoftwareSerial gsmSS(14, 12); // Node MCU

// GSM
//O_GSM gsm(&Serial, "GSM", '\n', '\r');  // Hardware Serial
O_GSM gsm(&gsmSS, "GSM", '\n', '\r');  // Software Serial

/*------------------------------------------------------------------------------
  SETUP
  ----------------------------------------------------------------------------*/
void setup() {
  Serial.begin(9600);
  //gsm.begin(9600);    // SIM800L
  gsm.begin(115200);  // SIM900A

  if (!gsm.init()) {
    Serial.println(F("GSM: NO SIM"));
  }

  if (!gsm.sendSMS(number, "Test")) {
    Serial.println(F("GSM: NO SIM or NO BALANCE"));
  }

  Serial.println("Loop start");
}
/*------------------------------------------------------------------------------
  LOOP
  ----------------------------------------------------------------------------*/
void loop() {
  char *gsmData = gsm.getData();
}
