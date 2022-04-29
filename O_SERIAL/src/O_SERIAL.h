/*------------------------------------------------------------------------------

  O_SERIAL.h
  Brief: Allows to receive/parse data from any serial capable device.
  Description: This library allows Arduino or any other board to receive
               incoming set of characters, words, or bytes via Serial
               Communication, and split that data.
  Author: Oliver Padolina
  URL: https://github.com/oliverz27/ArduinoLibraries/blob/main/O_SERIAL/
  Reference: https://forum.arduino.cc/t/serial-input-basics-updated/382007/2

  ----------------------------------------------------------------------------*/

#ifndef O_SERIAL_h
#define O_SERIAL_h

#include "SoftwareSerial.h"

#if defined(ESP32)
#include "BluetoothSerial.h"
#endif

#include "Arduino.h"

#define O_SERIAL_DEBUG
#define O_SERIAL_MAXCHARS 128

/*------------------------------------------------------------------------------
  O_SERIAL
  ----------------------------------------------------------------------------*/
class O_SERIAL
{
  public:
    O_SERIAL(HardwareSerial *serial, const char *tag, const char startMarker,
            const char endMarker);
#if defined (ESP32)
    O_SERIAL(BluetoothSerial *serial, const char *tag, const char startMarker,
            const char endMarker);
#endif
    O_SERIAL(SoftwareSerial *serial, const char *tag, const char startMarker,
            const char endMarker);
    void begin(const size_t baud);
#if defined (ESP32)
    void begin(const char *btName);
#endif
    char *getData();
    bool hasData();
    bool contains(const char *value);
    void sendData(char *data);
    void writeData(char data);
    void parseData(char **data, const char *parser);
    void listen();

  private:
    bool _hwSerial = false;
#if defined (ESP32)
    bool _btSerial = false;
#endif
    Stream *_port;
    char _receivedChars[O_SERIAL_MAXCHARS];
    char _receivedCharsCopy[O_SERIAL_MAXCHARS];
    const char *_tag;
    char _startMarker;
    char _endMarker;
    bool _hasData;
    void receiveData();
};
#endif
