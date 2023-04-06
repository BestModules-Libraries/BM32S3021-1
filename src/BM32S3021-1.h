/*****************************************************************
File:             BM32S3021-1.h
Author:           XIAO, BESTMODULES
Description:      Define classes and required variables
History：         
V1.0.0   -- initial version；2023-03-08；Arduino IDE : v1.8.16
******************************************************************/
#ifndef _BM32S3021_1_H_
#define _BM32S3021_1_H_

#include "Arduino.h"
#include <SoftwareSerial.h>

#define   SUCCESS         0
#define   FAIL            1
#define CHECK_OK        0
#define CHECK_ERROR     1
#define TIMEOUT_ERROR   2


class BM32S3021_1
{
  public:
    BM32S3021_1(uint8_t intPin, HardwareSerial *theSerial  = &Serial);
    BM32S3021_1(uint8_t intPin,uint8_t rxPin,uint8_t txPin);
    void begin(uint16_t baud = 9600);
   
    uint8_t getINT();
    uint8_t getIRStatus();
    uint8_t distanceLearning();
    uint8_t getIRGestureNum();
    uint16_t getFWVer();
    uint8_t reset();
   
    uint8_t getIRDebounce();
    uint8_t getIRThreshold();
    uint8_t getIRQTrigerTime();
    uint8_t getIRContinutyGestureTime();
    uint8_t getIRFastestGestureTime();
    uint8_t getIRSlowestGestureTime();
    uint8_t setIRDebounce(uint8_t  debounce = 16);
    uint8_t setIRThreshold(uint8_t  threshold = 7);
    uint8_t setIRQTrigerTime(uint8_t  irqTime = 50);
    uint8_t setIRContinutyGestureTime(uint8_t  irTime = 30);
    uint8_t setIRFastestGestureTime(uint8_t  irTime = 0);
    uint8_t setIRSlowestGestureTime(uint8_t  irTime = 20);
 
  private:
    uint8_t writeVerL(uint8_t  verl);
    uint8_t readIR1Ref();
    uint8_t readIR2Ref();
    uint8_t readIrA2_A5(uint8_t  buff[]);
    uint8_t readIrA6_Ab(uint8_t  buff[]);
    uint8_t getIROPA();
    uint8_t getIR1Current();
    uint8_t getIR2Current();
    uint8_t setIROPA(uint8_t  value = 23); 
    uint8_t setIR1Current(uint8_t  current = 25);
    uint8_t setIR2Current(uint8_t  current = 25);
    void writeBytes(uint8_t wbuf[], uint8_t wlen);
    uint8_t readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeOut = 10);
    uint16_t _intPin;
    uint16_t _rxPin;
    uint16_t _txPin;
    HardwareSerial *_hardSerial = NULL;
    SoftwareSerial *_softSerial = NULL ;   
};

#endif
