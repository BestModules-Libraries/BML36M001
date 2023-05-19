 
/*************************************************
File:             calibration.ino
Description:      1.The offset and crosstalk values are calibrated before leaving the factory
                  2.Calibration is generally not required unless the device is soldered on 
                  the customer board or a glass cover is added
                  3.The detected target is placed 140 mm away from the sensor
connection method： i2cPort:Wire  intPin:D2              
**************************************************/
#include "BML36M001.h"

BML36M001 BML36(2,&Wire);                            //i2cPort,intPin,Please comment out this line of code if you don't use HW Wire
//BML36M001 BML36(22,&Wire1);                           //Please uncomment out this line of code if you use HW Wire1 on BMduino
//BML36M001 BML36(25,&Wire2);                           //Please uncomment out this line of code if you use HW Wire1 on BMduino

void setup(void)
{
  BML36.begin();                //Initialize BML36M001 
  BML36.calibrateOffset(140);   //The detected target is placed 140 mm away from the sensor and the offset is automatically calibrated
  BML36.calibrateXTalk(140);    //The detected target is placed 140 mm away from the sensor and the crosstalk value is automatically calibrated
}

void loop(void)
{
}
