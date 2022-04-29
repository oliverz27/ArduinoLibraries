# SERIAL
This library allows Arduino or any other board to receive incoming set of characters, words, or bytes via Serial Communication, and split that data.

## Example
A Serial device is sending this data "<<DATA,DATA1,DATA2,DATA3>>" into Arduino, and needs to be splitted via "," character. This library can be used to receive the incoming data "<DATA,DATA1,DATA2,DATA3>" by using getData() API, and then check if the word "DATA" is present using contains() API, and lastly split it using parseData() API.

* [Serial Hardware Example](https://github.com/oliverz27/ArduinoLibraries/tree/main/O_SERIAL/examples/Serial_Hardware)
* [Serial Software Example](https://github.com/oliverz27/ArduinoLibraries/blob/main/O_SERIAL/examples/Serial_Software/Serial_Software.ino)

## How To Use Library
* Go back to main page - [README.md]()

## API
* void begin(const size_t baud);
* char *getData();
* bool hasData();
* bool contains(const char *value);
* void sendData(char *data);
* void writeData(char data);
* void parseData(char **data, const char *parser);
* void listen();
