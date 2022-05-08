/*------------------------------------------------------------------------------

  O_GSM.h
  Brief: 
  Description: 
  Author: Oliver Padolina
  URL: 
  Reference: 

  ----------------------------------------------------------------------------*/
  
#ifndef O_GSM_h
#define O_GSM_h

#include "Arduino.h"
#include "O_SERIAL.h"

#define O_GSM_DEBUG
#define O_GSM_TIMEOUT 5000 // Default Timeout (5 Seconds)

/*------------------------------------------------------------------------------
  O_GSM
  ----------------------------------------------------------------------------*/
class O_GSM : public O_SERIAL
{
  public:
    O_GSM(HardwareSerial *serial, const char *tag, const char startMarker,
            const char endMarker);
    O_GSM(SoftwareSerial *serial, const char *tag, const char startMarker,
            const char endMarker);
    void begin(const size_t baud);
    bool init();
    bool sendSMS(const char *number, const char *message);
    void deleteAllSMS();
    bool initGPRS(const char* apn);
    bool checkGPRS();
    void turnOnGPRS();
    void turnOffGPRS();
    bool httpSendData(const char* url);
    bool httpReadData();
    void httpTerminate();
    
  private:
    unsigned long _startTime = 0;
    bool sendAndWait(const char *command, const char *responseTag);
    bool sendAndWait(const char *command, const char *responseTag,
                     unsigned long timeout);
    bool writeAndWait(const char command, const char *responseTag);
    bool writeAndWait(const char command, const char *responseTag,
                     unsigned long timeout);
    bool wait(const char *responseTag,
                     unsigned long timeout);
    void readAndWait();
    void readAndWait(unsigned long timeout);
};
#endif
