/*****************************************************************
File:        BM32S3021-1.cpp
Author:      XIAO, BESTMODULES
Description: UART communication with the BM32S3021_1 and obtain the corresponding value  
History:         
V1.0.1   -- initial version；2023-03-08；Arduino IDE : v1.8.16
******************************************************************/
#include "BM32S3021-1.h"
/**********************************************************
Description: Constructor
Parameters:  intPin: INT Output pin connection with Arduino, the INT will be pulled down when an object approaches
             *theSerial: Wire object if your board has more than one UART interface      
                         parameter range:
                                         BMduino UNO: &Serial、&Serial1、&Serial2、&Serial3、&Serial4
                                         Arduino UNO：&Serial
Return:          
Others:     
**********************************************************/
BM32S3021_1::BM32S3021_1(uint8_t intPin, HardwareSerial *theSerial)
{
     _softSerial = NULL;
     _intPin = intPin;
     _hardSerial = theSerial;
}
/**********************************************************
Description: Constructor
Parameters:  intPin: INT Output pin connection with Arduino, 
                     the INT will be pulled down when an object approaches
             rxPin: Receiver pin of the UART
             txPin: Send signal pin of UART         
Return:          
Others:   
**********************************************************/
BM32S3021_1::BM32S3021_1(uint8_t intPin,uint8_t rxPin,uint8_t txPin)
{
    _hardSerial = NULL;
    _intPin = intPin;
    _rxPin = rxPin;
    _txPin = txPin;
    _softSerial = new SoftwareSerial(_rxPin,_txPin);
}

/**********************************************************
Description: Module Initial
Parameters:  baudRate: Module communication baud rate(Unique 9600bps)        
Return:          
Others:   If the hardware UART is initialized, the _softSerial 
          pointer is null, otherwise it is non-null       
**********************************************************/
void BM32S3021_1::begin(uint16_t baud)
{
    if(_softSerial != NULL)
    {
        pinMode(_intPin,INPUT);
        _softSerial->begin(baud); 
    }
    else
    {
        pinMode(_intPin,INPUT);
        _hardSerial->begin(baud);
    }
}

/**********************************************************
Description: Get INT Status
Parameters:          
Return:      INT Status：
                        0:INT output low level 
                        1:INT output high level  
Others:     
**********************************************************/
uint8_t BM32S3021_1::getINT()
{
     return (digitalRead(_intPin));
}

/**********************************************************
Description: Get IR Induction state
Parameters:   
Return:      irStatus&0x08==0: calibration not approach 
             irStatus&0x08==1:
                                irStatus&0x04== : 
                                                 1 : B to A 
                                                 0 : not B to A 
                                irStatus&0x02== : 
                                                 1 : A to B
                                                 0 : not A to B
                                irStatus&0x01== : 
                                                 1 : approach   
                                                 0 : not approach      
Others:      
**********************************************************/
uint8_t BM32S3021_1::getIRStatus()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x02, 0x01, 0xD8};
    uint8_t buff[6] = {0};
    uint8_t  irStatus = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     irStatus= buff[4];
    }
    delay(10);
    return irStatus;
}

/**********************************************************
Description: Distance Learning
Parameters:         
Return:      Communication status  0:Success 1:Fail   
Others:      Place the object to be measured to the distance 
             you want to learn, and wait for 2s to complete 
             the learning
**********************************************************/
uint8_t BM32S3021_1::distanceLearning()
{
    uint8_t sendBuf[3] = {0x55, 0x19, 0x6E};
    uint8_t buff[3] = {0};
    uint8_t value = 0;
    writeBytes(sendBuf,3);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(2000);
       return SUCCESS;
     }
    }
    delay(2000);
    return FAIL ;
}

/**********************************************************
Description: Get number of left & right sliding
Parameters:       
Return:      number of left & right sliding: -127~128   
Others:      
**********************************************************/
uint8_t BM32S3021_1::getIRGestureNum()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x03, 0x01, 0xD9};
    uint8_t buff[6] = {0};
    uint8_t  num = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     num= buff[4];
    }
    delay(10);
    return num;
}

/**********************************************************
Description: Get the version information 
Parameters:
Return:      version number: (default 0b00000001)  
Others:      version number = ver_highByte+ver_lowByte
**********************************************************/
uint16_t BM32S3021_1::getFWVer()
{ 
    uint8_t sendBuf1[5] = {0x55, 0x80, 0x00, 0x01,0xD6};
    uint8_t sendBuf2[5] = {0x55, 0x80, 0x01, 0x01,0xD7};
    uint8_t buff[6] = {0};
    uint8_t verh,verl = 0;
    uint8_t temp = 0;
    uint16_t ver = 0;
    writeBytes(sendBuf1,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
    verl= buff[4];
    }
    delay(10);
    writeBytes(sendBuf2,5);
   if(readBytes(buff,6)== CHECK_OK)
   {
    verh= buff[4];
   }  
    ver = verl+ verh;
   delay(10);
   return ver;
}

/**********************************************************
Description: Module reset
Parameters:         
Return:      Communication status  0:Success 1:Fail        
Others:      
**********************************************************/
uint8_t BM32S3021_1::reset()
{
    uint8_t sendBuf[3] = {0x55, 0x10, 0x65};
    uint8_t buff[3] = {0};
    uint8_t value = 0;
    writeBytes(sendBuf,3);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Get IR debounce value
Parameters:    
Return:     debounce value: 0~255   
Others:      
**********************************************************/
uint8_t BM32S3021_1::getIRDebounce()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x06, 0x01, 0xDC};
    uint8_t buff[6] = {0};
    uint8_t  debounce = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     debounce= buff[4];
    }
    delay(10);
    return debounce;
}

/**********************************************************
Description: Get IR trigger threshold
Parameters: 
Return:      trigger threshold: 10~200   
Others:      
**********************************************************/
uint8_t BM32S3021_1::getIRThreshold()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x07, 0x01, 0xDD};
    uint8_t buff[6] = {0};
    uint8_t threshold = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     threshold = buff[4];
    }
    delay(10);
    return threshold;
}

/**********************************************************
Description: Get IRQ triger time
Parameters:
Return:      IRQ Triger Time:0~255  
Others:      The calculation formula of IRQ trigger time is as follows:
             irqTime×4ms, (default 50×4ms=200ms)
**********************************************************/
uint8_t BM32S3021_1::getIRQTrigerTime()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x08, 0x01, 0xDE};
    uint8_t buff[6] = {0};
    uint8_t irqTime = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     irqTime = buff[4];
    }
    delay(10);
    return irqTime;
}

/**********************************************************
Description: Get IR Cumulative continuous sliding time
Parameters:  
Return:      Cumulative continuous sliding time: 0~127   
Others:      The calculation formula of IRQ trigger time is as follows:
             irTime×64ms, (default 30×64ms=1.92s)
**********************************************************/
uint8_t BM32S3021_1::getIRContinutyGestureTime()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x09, 0x01, 0xDF};
    uint8_t buff[6] = {0};
    uint8_t irqTime = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     irqTime = buff[4];
    }
    delay(10);
    return irqTime;
}

/**********************************************************
Description: Get IR fastest gesture time
Parameters:  
Return:      fastest gesture time: 0~200   
Others:      The calculation formula of IRQ trigger time is as follows:
             20+irTime×4ms, (Default 20+0×4ms=20ms)
**********************************************************/
uint8_t BM32S3021_1::getIRFastestGestureTime()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x0A, 0x01, 0xE0};
    uint8_t buff[6] = {0};
    uint8_t irqTime = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     irqTime = buff[4];
    }
    delay(10);
    return irqTime;
}

/**********************************************************
Description: Get IR  slowest  gesture time
Parameters:        
Return:      slowest  gesture time: 0~200   
Others:      The calculation formula of IRQ trigger time is as follows:
             irTime×64ms, (default 20×64ms=1.28s)
**********************************************************/
uint8_t BM32S3021_1::getIRSlowestGestureTime()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x0B, 0x01, 0xE1};
    uint8_t buff[6] = {0};
    uint8_t irqTime = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     irqTime = buff[4];
    }
    delay(10);
    return irqTime;
}

/**********************************************************
Description: Set IR debounce value
Parameters:  debounce: Gesture trigger to shake times
                       parameter range: 0~255 (Default 7) 
                       unit: times
Return:      Communication status  0:Success 1:Fail   
Others:      
**********************************************************/
uint8_t BM32S3021_1::setIRDebounce(uint8_t  debounce)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x06, 0x01, 0x00, 0x00};
    sendBuf[4] =  debounce;
    sendBuf[5] =  debounce+28;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Set IR trigger threshold
Parameters:  threshold: Gesture trigger threshold
                        parameter range: 10~200 (Default 16)          
Return:      Communication status  0:Success 1:Fail   
Others:      
**********************************************************/
uint8_t BM32S3021_1::setIRThreshold(uint8_t  threshold)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x07, 0x01, 0x00, 0x00};
    sendBuf[4] =  threshold;
    sendBuf[5] =  threshold+29;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}



/**********************************************************
Description: Set IRQ triger time
Parameters:  irqTime: IRQ trigger time when gesture is valid
                      parameter range: 0~255 (Default 50)       
Return:      Communication status  0:Success 1:Fail   
Others:      The calculation formula of IRQ trigger time is as follows:
             irqTime×4ms, (default 50×4ms=200ms)
**********************************************************/
uint8_t BM32S3021_1::setIRQTrigerTime(uint8_t  irqTime)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x08, 0x01, 0x00, 0x00};
    sendBuf[4] =  irqTime;
    sendBuf[5] =  irqTime+30;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Set IR Cumulative continuous sliding time
Parameters:  irqTime: Cumulative continuous sliding duration
                      parameter range: 0 ~ 255 (Default 30)                                  
Return:      Communication status  0:Success 1:Fail   
Others:      The calculation formula of IRQ trigger time is as follows:
             irTime×64ms, (default 30×64ms=1.92s)
**********************************************************/
uint8_t BM32S3021_1::setIRContinutyGestureTime(uint8_t  irTime)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x09, 0x01, 0x00, 0x00};
    sendBuf[4] =  irTime;
    sendBuf[5] =  irTime+31;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Set IR fastest gesture time
Parameters:  irqTime: Fastest gesture to judge time
                      parameter range: 0 ~ 200 (Default 0)         
Return:      Communication status  0:Success 1:Fail   
Others:      The calculation formula of IRQ trigger time is as follows:
             20+irTime×4ms, (Default 20+0×4ms=20ms)
**********************************************************/
uint8_t BM32S3021_1::setIRFastestGestureTime(uint8_t  irTime)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x0A, 0x01, 0x00, 0x00};
    sendBuf[4] =  irTime;
    sendBuf[5] =  irTime+32;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Set IR Slowest gesture time
Parameters:  irqTime: Slowest gesture to judge time
                      parameter range: 0 ~ 200 (Default 20)     
Output:         
Return:      Communication status  0:Success 1:Fail   
Others:      The calculation formula of IRQ trigger time is as follows:
             irTime×64ms, (default 20×64ms=1.28s)
**********************************************************/
uint8_t BM32S3021_1::setIRSlowestGestureTime(uint8_t  irTime)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x0B, 0x01, 0x00, 0x00};
    sendBuf[4] =  irTime;
    sendBuf[5] =  irTime+33;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Write the version information low byte
Parameters:  verl: version information low byte
                   parameter range: 0X00~0XFF
     
Return:      Communication status  0:Success 1:Fail   
Others:      If the low byte is set to 0XAA, you can set the Current and OPA
**********************************************************/
uint8_t BM32S3021_1::writeVerL(uint8_t  verl)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x00, 0x01, 0x00, 0X00};
    uint8_t temp = 0;
    uint8_t buff[3] = {0};
    sendBuf[4] = verl;
    sendBuf[5] = 22+verl;
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Read IR1 reference
Parameters: 
Return:      IR1 reference   
Others:      
**********************************************************/
uint8_t BM32S3021_1::readIR1Ref()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x04, 0x01, 0xDA};
     uint8_t buff[6] = {0};
    uint8_t ref = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     ref= buff[4];
    }
    delay(10);
    return ref;
}
/**********************************************************
Description: Read IR2 reference
Parameters:         
Return:     IR2 reference 
Others:      
**********************************************************/
uint8_t BM32S3021_1::readIR2Ref()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x05, 0x01, 0xDB};
    uint8_t buff[6] = {0};
    uint8_t ref = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     ref= buff[4];
    }
    delay(10);
    return ref;
}

/**********************************************************
Description: Read IRStatus, IRGerstureNum, IR1Ref, IR2Ref value
Parameters:  buff[]: Stores the IR partial state parameter
                     parameter range: The minimum array length is 4 bytes                            
Return:      
             0:Success 
             1:Fail   
Others:      buff[0] : IR Status  
             buff[1] : IR Gersture Num
             buff[2] : IR1 Ref 
             buff[3] : IR2 Ref
**********************************************************/
uint8_t BM32S3021_1::readIrA2_A5(uint8_t  buff[])
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x02, 0x04, 0xDB};
    uint8_t value = 0;
    uint8_t rbuf[9] ={0};
    uint8_t Cnt = 0;
    writeBytes(sendBuf,5);
    if(readBytes(rbuf,9)== CHECK_OK)
    {
     delay(10);
     buff[0] = rbuf[4];
     buff[1] = rbuf[5];
     buff[2] = rbuf[6];
     buff[3] = rbuf[7];
     return SUCCESS;
    }
    return FAIL;
}

/**********************************************************
Description: Read IRDebounce,IRThreshold,IRQTrigerTime,
             ContinutyGerstureTime,FastestGerstureTime,
             SlowestGerstureTime value
Parameters:  buff[]:Stores the IR partial state parameter    
                    parameter range: The minimum array length is 6 bytes    
Return:     
             0:Success 
             1:Fail   
Others:      buff[0] : IR Debounce  
             buff[1] : IR Threshold
             buff[2] : IRQ Triger Time 
             buff[3] : Continuty Gersture Time
             buff[4] : Fastest Gersture Time 
             buff[5] : Slowest Gersture Time  
**********************************************************/
uint8_t BM32S3021_1::readIrA6_Ab(uint8_t  buff[])
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x06, 0x06, 0xE1};
    uint8_t value = {0};
    uint8_t rbuf[11] ={0};
    uint8_t Cnt = 0;
    writeBytes(sendBuf,5);
    if(readBytes(rbuf,11)== CHECK_OK)
    {
     delay(10);
     buff[0] = rbuf[4];
     buff[1] = rbuf[5];
     buff[2] = rbuf[6];
     buff[3] = rbuf[7];
     buff[4] = rbuf[8];
     buff[5] = rbuf[9];
     return SUCCESS;
    }
    return FAIL;
}

/**********************************************************
Description: Get IR OPA 
Parameters:        
Return:      OPA:Current amplification  
Others:      
**********************************************************/
uint8_t BM32S3021_1::getIROPA()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x21, 0x01, 0xF7};
    uint8_t buff[6] = {0};
    uint8_t opa = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     opa = buff[4];
    }
    delay(10);
    return opa;
}


/**********************************************************
Description: Get IR1 emission current value
Parameters:       
Return:      IR1 emission current value :0~31  
Others:      
**********************************************************/
uint8_t BM32S3021_1::getIR1Current()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x22, 0x01, 0xF8};
    uint8_t buff[6] = {0};
    uint8_t  current = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     current = buff[4];
    }
    delay(10);
    return current;
}

/**********************************************************
Description: Get IR2 emission current value
Parameters: 
Return:      IR2 emission current value :0~31   
Others:        
**********************************************************/
uint8_t BM32S3021_1::getIR2Current()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x23, 0x01, 0xF9};
    uint8_t buff[6] = {0};
    uint8_t  current = 0;
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     current = buff[4];
    }
    delay(10);
    return current;
}

/**********************************************************
Description: Set IR OPA
Parameters:  opa: Current magnification
             parameter range:(fixed 23)          
Return:      Communication status  0:Success 1:Fail   
Others:      Version information Low bytes You can set the 
             OPA value only when this parameter is set to 0XAA
**********************************************************/
uint8_t BM32S3021_1::setIROPA(uint8_t  opa)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x21, 0x01, 0x00, 0x00};
    sendBuf[4] =  opa;
    sendBuf[5] =  opa+55;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Set IR1 emission current value
Parameters:  current: IR1 emission current value
                      parameter range: 0~31 (Default 25),emission current = range x 1 + 1
                      unit: mA                  
Return:      Communication status  0:Success 1:Fail   
Others:      Version information Low bytes You can set the 
             Current value only when this parameter is set to 0XAA
**********************************************************/
uint8_t BM32S3021_1::setIR1Current(uint8_t  current)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x22, 0x01, 0x00, 0x00};
    sendBuf[4] =  current;
    sendBuf[5] =  current+56;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Set IR2 emission current value
Parameters:  current: IR2 emission current value
                      parameter range: 0~31 (Default 25),emission current = range x 1 + 1
                      unit: mA                  
Return:      Communication status  0:Success 1:Fail   
Others:      Version information Low bytes You can set the 
             Current value only when this parameter is set to 0XAA
**********************************************************/
uint8_t BM32S3021_1::setIR2Current(uint8_t  current)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x23, 0x01, 0x00, 0x00};
    sendBuf[4] =  current;
    sendBuf[5] =  current+57;
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: writeBytes
Parameters:  wbuf[]:Variables for storing Data to be sent
             wlen:Length of data sent  
Return:   
Others:
**********************************************************/
void BM32S3021_1::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
  /* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
  }
  /* Select HardwareSerial Interface */
  else
  {
    while (_hardSerial->available() > 0)
    {
      _hardSerial->read();
    }
    _hardSerial->write(wbuf, wlen);
  }
}

/**********************************************************
Description: readBytes
Parameters:  rbuf[]:Variables for storing Data to be obtained
             rlen:Length of data to be obtained
Return:   
Others:
**********************************************************/
uint8_t BM32S3021_1::readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeOut)
{
  uint8_t i = 0, delayCnt = 0, checkSum = 0;
/* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_softSerial->available() == 0)
      {
        if (delayCnt > timeOut)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _softSerial->read();
    }
  }
/* Select HardwareSerial Interface */
  else
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_hardSerial->available() == 0)
      {
        if (delayCnt > timeOut)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _hardSerial->read();
    }
  }

  /* check Sum */
  for (i = 0; i < (rlen - 1); i++)
  {
    checkSum += rbuf[i];
  }
  if (checkSum == rbuf[rlen - 1])
  {
    return CHECK_OK; // Check correct
  }
  else
  {
    return CHECK_ERROR; // Check error
  }
}
