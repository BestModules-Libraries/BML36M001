/*************************************************
File:             settingDisntanceSensor.ino
Description:      1.The Wire is used for IIC communication with the BML36M001.
                  2.Set:time budget,intermeasurement Period, measurement mode
                  3.get:time budget,intermeasurement Period, measurement mode,and display them on the serial monitor
                  3.Use a hardware serial port (BAUDRATE 115200) to communicate with the serial monitor.
                    When the distance data is ready, the distance data is read and displayed on the serial monitor.
connection method： i2cPort:Wire  intPin:D2          
**************************************************/
#include "BML36M001.h"

BML36M001 BML36(2,&Wire);                            //i2cPort,intPin,Please comment out this line of code if you don't use HW Wire
//BML36M001 BML36(22,&Wire1);                           //Please uncomment out this line of code if you use HW Wire1 on BMduino
//BML36M001 BML36(25,&Wire2);                           //Please uncomment out this line of code if you use HW Wire1 on BMduino
void setup(void)
{
  Serial.begin(115200);                              //Set the communication rate of the serial monitor to 115200 baud
  BML36.begin();                                    //Initialize BML36M001   
  BML36.setTimingBudgetInMs(100);          //Timing Budget(ms)：20、33、50、100、200、500
  BML36.setIntermeasurementPeriod(1000);   //20~1000，Intermeasurement period must be >= timing budget. 
  BML36.setDistanceModeLong();                         //Set the probe mode to be long-distance.  Detection range:40~4000(max)（Unit: mm）                                                
/*BML36.setDistanceModeShort(); */                     //Set the probe mode to be short-range.  Detection range:40~1300（Unit: mm）
  Serial.print("BML36 Timing Budget(ms): ");
  Serial.println(BML36.getTimingBudgetInMs());
  Serial.print("BML36 Intermeasurement Period(ms): ");
  Serial.println(BML36.getIntermeasurementPeriod());
  if (BML36.getDistanceMode() == 1)
  {
    Serial.println("BML36 Distance Mode: Short");
  }
  else if (BML36.getDistanceMode() == 2)
  {
    Serial.println("BML36 Distance Mode: Long");
  }
  Serial.println();
  BML36.startRanging();                                //Start measuring the distance         
}

void loop(void)
{    
  if (BML36.checkForDataReady())                               //Ask if the data is ready  0：data is not ready 1：data is ready
  {
   Serial.print("Distance(mm):");                               //Print string:“Distance(mm):”        
   Serial.println(BML36.readDistance());                        //Gets distance data（Unit: mm）
   BML36.clearInterrupt();                                             //Clear the BML36M001 interrupt flag bit   
  } 
}
