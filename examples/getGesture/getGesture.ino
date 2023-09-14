/*****************************************************************
File:         getGesture.ino
Description:  1.SoftwareSerial interface (BAUDRATE 9600)is used to communicate with BMS31M002.
              2.hardware Serial (BAUDRATE 9600) is used to communicate with Serial port monitor.
                When there is a gesture,read the proximity sensor status displayed on the serial port monitor.
              3.Slide the left and the serial port monitor prints "Swipe left".
                Swipe right and the serial monitor prints "Swipe right"
connection method： intPin:D3 rxPin:D5 txPin:D4
******************************************************************/
#include "BM32S3021-1.h"
BM32S3021_1     BMS31(3,5,4); //intPin,rxPin,txPin,Please comment out this line of code if you don't use SW Serial
//BM32S3021_1     BMS31(22,&Serial1); //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BM32S3021_1     BMS31(25,&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BM32S3021_1     BMS31(3,&Serial3); //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BM32S3021_1     BMS31(3,&Serial4); //Please uncomment out this line of code if you use HW Serial4 on BMduino

uint8_t irStatus = 0;

void setup() 
{
  BMS31.begin(); 
  Serial.begin(9600);//Set the communication rate between the serial monitor and BMS31M002 to 9600 baud rate
}

void loop() 
{ 
 if(!BMS31.getINT())
   {
    irStatus = BMS31.getIRStatus();  //Gets the proximity sensing state 0:not approach 1:approach
    if(!(irStatus&0x08))          //calibration is completed when BIT3 = 0
     {
      if(irStatus&0x02)  //Determine whether it is true to slide left to right
       {
        Serial.println("Swipe right");   
       }
      else if(irStatus&0x04)//Determine whether it is true to slide right to left 
       {  
        Serial.println("Swipe left");
       }
      }
    }  
}
