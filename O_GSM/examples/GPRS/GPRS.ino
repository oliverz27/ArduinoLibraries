/*------------------------------------------------------------------------------

  GPRS.ino
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

// APN
const char *apn = "internet"; // Smart
//const char *apn = "internet.globe.com.ph"; // Globe
// Server URL
const char* serverURL = "www.google.com";

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

  Serial.println(F("GSM: Initializing..."));
  delay(10000);

  if (!gsm.init()) {
    Serial.println(F("GSM: NO SIM"));
  }

  if (!gsm.initGPRS((const char*)apn)) {
    Serial.println(F("GSM: GPRS Error"));
  }

  if (!gsm.checkGPRS()) {
    gsm.turnOnGPRS();
  }

  gsm.httpSendData(serverURL);
  if (gsm.httpReadData()) {
    while(true) {
      char *gsmData = gsm.getData();
      if (strlen(gsmData) > 0) {
        // Do the parsing here
        break;
      }
      delay(1);
    }
  }
  gsm.httpTerminate();
  gsm.turnOffGPRS();
  

  Serial.println(F("Loop start"));
}
/*------------------------------------------------------------------------------
  LOOP
  ----------------------------------------------------------------------------*/
void loop() {
  while (Serial.available()) {
    gsm.writeData(Serial.read());
  }
  char *gsmData = gsm.getData();
}
