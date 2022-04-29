/*------------------------------------------------------------------------------

  Serial_Hardware.ino
  Brief: Allows to receive/parse data from any serial capable device.
  Description: This library allows Arduino or any other board to receive
               incoming bytes from Hardware Serial and convert it to character
               array that can be parsed.
  Author: Oliver Padolina
  Repository: https://github.com/oliverz27/ArduinoLibraries/O_SERIAL
  Circuit:
            Connect Arduino Mega Serial1 TX -> Other Board TX
            Connect Arduino Mega Serial1 RX -> Other Board RX

  ----------------------------------------------------------------------------*/

#include <O_SERIAL.h>

/*------------------------------------------------------------------------------
  GLOBAL VARIABLES
  ----------------------------------------------------------------------------*/

// SERIAL
O_SERIAL other(&Serial1, "OTHER", '<', '>');

unsigned long startTime = 0;

/*------------------------------------------------------------------------------
  SETUP
  ----------------------------------------------------------------------------*/
void setup() {
  Serial.begin(9600);
  other.begin(9600);
}
/*------------------------------------------------------------------------------
  LOOP
  ----------------------------------------------------------------------------*/
void loop() {

  /*----------------------------------------------------------------------------
    SERIAL RECEIVE CODE
    --------------------------------------------------------------------------*/
  // Store the incoming data from serial to otherData char array
  char *otherData = other.getData();

  // Check if there is a data on otherData
  if (strlen(otherData) > 0) {

    // Check if the otherData contains the word "DATA,"
    if (other.contains("DATA,")) {
      /*------------------------------------------------------------------------
        SERIAL PARSE CODE
        ----------------------------------------------------------------------*/
      /*
        Parse the data behind the scene on "," symbol
        and store it as array of char array
      */
      char *parsedData[4]; // DATA,DATA1,DATA2,DATA3
      other.parseData(parsedData, ",");
      Serial.println(parsedData[0]);
      Serial.println(parsedData[1]);
      Serial.println(parsedData[2]);
    }
  }

  /*----------------------------------------------------------------------------
    SERIAL SEND CODE
    --------------------------------------------------------------------------*/
  // Send data every 5 seconds
  if (millis() - startTime > 5000) {
    startTime = millis();
    other.sendData("<DATA,DATA1,DATA2,DATA3>");
  }
}
