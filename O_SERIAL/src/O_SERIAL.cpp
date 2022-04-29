/*------------------------------------------------------------------------------

  O_SERIAL.cpp
  Brief: Allows to receive/parse data from any serial capable device.
  Description: This library allows Arduino or any other board to receive
               incoming set of characters, words, or bytes via Serial
               Communication, and split that data.
  Author: Oliver Padolina
  URL: https://github.com/oliverz27/ArduinoLibraries/O_SERIAL
  Reference: https://forum.arduino.cc/t/serial-input-basics-updated/382007/2

  ----------------------------------------------------------------------------*/

#include "Arduino.h"
#include "O_SERIAL.h"
#if defined (ESP32)
#include "BluetoothSerial.h"
#endif

/*------------------------------------------------------------------------------
  CONSTRUCTOR
  ----------------------------------------------------------------------------*/
O_SERIAL::O_SERIAL(HardwareSerial *serial, const char *tag,
                   const char startMarker, const char endMarker)
{
  _hwSerial = true;
  _port = serial;
  _tag = tag;
  _startMarker = startMarker;
  _endMarker = endMarker;
}
#if defined (ESP32)
O_SERIAL::O_SERIAL(BluetoothSerial *serial, const char *tag,
                   const char startMarker, const char endMarker)
{
  _btSerial = true;
  _port = serial;
  _tag = tag;
  _startMarker = startMarker;
  _endMarker = endMarker;
}
#endif
O_SERIAL::O_SERIAL(SoftwareSerial *serial, const char *tag,
                   const char startMarker, const char endMarker)
{
  _port = serial;
  _tag = tag;
  _startMarker = startMarker;
  _endMarker = endMarker;
}
/*------------------------------------------------------------------------------
  PUBLIC FUNCTIONS
  ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
  BEGIN
  ----------------------------------------------------------------------------*/
void O_SERIAL::begin(const size_t baud)
{
  if (_hwSerial) {
    static_cast<HardwareSerial*>(_port)->begin(baud);
  }
  else {
    static_cast<SoftwareSerial*>(_port)->begin(baud);
  }
}
#if defined (ESP32)
void O_SERIAL::begin(const char *btName)
{
  if (_btSerial) {
    static_cast<BluetoothSerial*>(_port)->begin(btName);
  }
}
#endif
/*------------------------------------------------------------------------------
  GET DATA
  ----------------------------------------------------------------------------*/
char *O_SERIAL::getData() {
  receiveData();
  if (_hasData) {
    _hasData = false;
    strcpy(_receivedCharsCopy, _receivedChars);
#ifdef O_SERIAL_DEBUG
    Serial.print(_tag);
    Serial.print(F(": "));
    Serial.println(_receivedChars);
    Serial.print(_tag);
    Serial.print(F(": C->"));
    Serial.println(_receivedCharsCopy);
#endif
    return _receivedChars;
  }
  return "\0";
}
/*------------------------------------------------------------------------------
  HAS DATA
  ----------------------------------------------------------------------------*/
bool O_SERIAL::hasData() {
  return _hasData;
}
/*------------------------------------------------------------------------------
  CONTAINS
  ----------------------------------------------------------------------------*/
bool O_SERIAL::contains(const char *value) {
  if (strstr(_receivedChars, value) != NULL) {
    return true;
  }
  return false;
}
/*------------------------------------------------------------------------------
  SEND DATA
  ----------------------------------------------------------------------------*/
void O_SERIAL::sendData(char *data)
{
  _port->print(data);
#ifdef O_SERIAL_DEBUG
  Serial.print(_tag);
  Serial.print(F("-> "));
  Serial.println(data);
#endif
}
/*------------------------------------------------------------------------------
  SEND DATA
  ----------------------------------------------------------------------------*/
void O_SERIAL::writeData(char data)
{
  _port->write(data);
#ifdef O_SERIAL_DEBUG
  Serial.print(_tag);
  Serial.print(F("-> "));
  Serial.println(data);
#endif
}
/*------------------------------------------------------------------------------
  PARSE DATA
  ----------------------------------------------------------------------------*/
void O_SERIAL::parseData(char **data, const char *parser) {
  char *index;
  uint8_t i = 0;

  index = strtok(_receivedCharsCopy, parser);
  while (index != NULL) {
    data[i] = index;
#ifdef O_SERIAL_DEBUG
    Serial.print(_tag);
    Serial.print(F(": "));
    Serial.print(i);
    Serial.print(F("->"));
    Serial.println(index);
#endif
    index = strtok(NULL, parser);
    i++;
  }
}
/*------------------------------------------------------------------------------
  LISTEN
  ----------------------------------------------------------------------------*/
void O_SERIAL::listen()
{
  if (!_hwSerial) {
    static_cast<SoftwareSerial*>(_port)->listen();
  }
}
/*------------------------------------------------------------------------------
  PRIVATE FUNCTIONS
  ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
  RECEIVE DATA
  ----------------------------------------------------------------------------*/
void O_SERIAL::receiveData() {
  static boolean receiving = false;
  static uint8_t index = 0;
  char receivedChar;

  while (_port->available() > 0 && _hasData == false) {
    receivedChar = _port->read();

    if (receiving) {
      if (receivedChar != _endMarker) {
        _receivedChars[index] = receivedChar;
        index++;
        if (index >= O_SERIAL_MAXCHARS) {
          index = O_SERIAL_MAXCHARS - 1;
        }
      }
      else {
        _receivedChars[index] = '\0';
        receiving = false;
        index = 0;
        _hasData = true;
      }
    }
    else if (receivedChar == _startMarker) {
      receiving = true;
    }
  }
}
