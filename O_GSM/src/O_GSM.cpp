/*------------------------------------------------------------------------------

  O_GSM.cpp
  Brief: 
  Description: 
  Author: Oliver Padolina
  URL: 
  Reference: 

  ----------------------------------------------------------------------------*/
  
#include "Arduino.h"
#include "O_SERIAL.h"
#include "O_GSM.h"

/*------------------------------------------------------------------------------
  CONSTRUCTOR
  ----------------------------------------------------------------------------*/
O_GSM::O_GSM(HardwareSerial *serial, const char *tag,
                     const char startMarker, const char endMarker):
                     O_SERIAL(serial, tag, startMarker, endMarker)
{
}
O_GSM::O_GSM(SoftwareSerial *serial, const char *tag,
                     const char startMarker, const char endMarker):
                     O_SERIAL(serial, tag, startMarker, endMarker)
{
}
/*------------------------------------------------------------------------------
  PUBLIC FUNCTIONS
  ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
  BEGIN
  ----------------------------------------------------------------------------*/

void O_GSM::begin(const size_t baud)
{
  O_SERIAL::begin(baud);
}
/*------------------------------------------------------------------------------
  INIT
  ----------------------------------------------------------------------------*/
bool O_GSM::init()
{
  if (!sendAndWait("AT\r\n", "OK")) {
#ifdef O_GSM_DEBUG
    Serial.println(F("GSM: AT Error"));
#endif
    return false;
  }
  if (!sendAndWait("AT+CCID\r\n", "OK")) {
#ifdef O_GSM_DEBUG
    Serial.println(F("GSM: AT+CCID Error"));
#endif
    return false;
  }
  if (!sendAndWait("AT+CREG?\r\n", "CREG: 0,1")) {
#ifdef O_GSM_DEBUG
    Serial.println(F("GSM: AT+CREG Error"));
#endif
    return false;
  }
  return true;
}
/*------------------------------------------------------------------------------
  SEND SMS
  ----------------------------------------------------------------------------*/
bool O_GSM::sendSMS(const char *number, const char *message)
{
  if (!sendAndWait("AT+CMGF=1\r\n", "OK")) {  
#ifdef O_GSM_DEBUG
    Serial.println(F("GSM: AT+CMGF Error"));
#endif
    return false;
  }

  sendData("AT+CMGS=\"");
  sendData((char *)number);
  sendData("\"\r\n");
  delay(100);
 
  sendData((char *)message);
  if (!writeAndWait((const char)26, "+CMGS:", 60000)) {
#ifdef O_GSM_DEBUG
    Serial.println(F("GSM: AT+CMGS Error"));
#endif
    return false;
  }
  
  return true;
}
/*------------------------------------------------------------------------------
  DELETE ALL SMS
  ----------------------------------------------------------------------------*/
void O_GSM::deleteAllSMS()
{
  if (!sendAndWait("AT+CMGD=1,4\r\n", "OK")) {
#ifdef O_GSM_DEBUG
    Serial.println(F("GSM: AT+CMGD Error"));
#endif
  }
}
/*------------------------------------------------------------------------------
  PRIVATE FUNCTIONS
  ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
  SEND AND WAIT
  ----------------------------------------------------------------------------*/
bool O_GSM::sendAndWait(const char *command, const char *responseTag)
{
  return sendAndWait(command, responseTag, O_GSM_TIMEOUT);
}
/*------------------------------------------------------------------------------
  SEND AND WAIT
  ----------------------------------------------------------------------------*/
bool O_GSM::sendAndWait(const char *command, const char *responseTag,
                            unsigned long timeout)
{
  sendData((char *)command);
  return wait(responseTag, timeout);
}
/*------------------------------------------------------------------------------
  WRITE AND WAIT
  ----------------------------------------------------------------------------*/
bool O_GSM::writeAndWait(const char command, const char *responseTag)
{
  return writeAndWait(command, responseTag, O_GSM_TIMEOUT);
}
/*------------------------------------------------------------------------------
  WRITE AND WAIT
  ----------------------------------------------------------------------------*/
bool O_GSM::writeAndWait(const char command, const char *responseTag,
                            unsigned long timeout)
{
  writeData((char)command);
  return wait(responseTag, timeout);
}
/*------------------------------------------------------------------------------
  WAIT
  ----------------------------------------------------------------------------*/
bool O_GSM::wait(const char *responseTag,
                            unsigned long timeout)
{
  _startTime = millis();
  while (millis() - _startTime < timeout) {
    char *response = getData();
    if (response) {
      if (contains(responseTag)) {
        delay(100);
        return true;
      }
    }
    delay(1);
  }
  return false;
}
/*------------------------------------------------------------------------------
  SEND AND READ
  ----------------------------------------------------------------------------*/
void O_GSM::readAndWait()
{ 
  readAndWait(O_GSM_TIMEOUT);
}
/*------------------------------------------------------------------------------
  SEND AND READ
  ----------------------------------------------------------------------------*/
void O_GSM::readAndWait(unsigned long timeout)
{   
  _startTime = millis();
  while (millis() - _startTime < timeout) {
    getData();
    delay(1);
  }
}
