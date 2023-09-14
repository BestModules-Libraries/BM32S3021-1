
/*****************************************************************
File:         distanceLearning.ino
Description:  1.SoftwareSerial interface (BAUDRATE 9600)is used to communicate with BM32S3021_1.
              2.hardware Serial (BAUDRATE 9600) is used to communicate with Serial port monitor.
              3.Distance learning mode (20cm) : Please place the paper 20cm away from the module and wait 
              for two seconds.When the module finishes learning, the calibration result will be printed
              4.Within 20CM of BM32S3021_1. Slide the left and the serial port monitor prints 
                "Swipe left". Swipe right and the serial monitor prints "Swipe Right".
connection method： intPin:D3 rxPin:D5 txPin:D4
******************************************************************/
#include "BM32S3021-1.h"
BM32S3021_1     BMS31(3,5,4); //intPin,rxPin,txPin,Please comment out this line of code if you don't use SW Serial
//BM32S3021_1     BMS31(22,&Serial1); //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BM32S3021_1     BMS31(25,&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BM32S3021_1     BMS31(3,&Serial3); //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BM32S3021_1     BMS31(3,&Serial4); //Please uncomment out this line of code if you use HW Serial4 on BMduino
uint8_t irStatus = 0;
uint8_t FastestGerstureTime= 0;
uint8_t IRSlowestGerstureTime= 0;
void setup() 
{
    BMS31.begin();
    Serial.begin(9600);        //Set the communication rate between the serial monitor and BM32S3021_1 to 9600 baud rate
    BMS31.distanceLearning();  //Start distance learn
    irStatus = BMS31.getIRStatus();
    if(!(irStatus&0x08))          //Calibration is completed when BIT3 = 0
      {
       Serial.println("Distance learning success");
      }
     else
     {
       Serial.println("Distance learning fail");
     }
}

void loop() 
{ 
   if(!BMS31.getINT()) //Gets the proximity sensing state 0: approach 1:not approach 
   {
     irStatus = BMS31.getIRStatus();  //Read near inductive state
     if(!(irStatus&0x08))          //Calibration is completed when BIT3 = 0
      {
        if(irStatus&0x02)         //Swipe right gesture works
         {
          Serial.println("Swipe right");
         }
        else if(irStatus&0x04)    //Swipe lift gesture works
         {
          Serial.println("Swipe left");
         }
       }
    }
}
