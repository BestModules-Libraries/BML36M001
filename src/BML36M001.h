/*************************************************
File:             BML36M001.h
Author:           XIAO, BESTMODULES
Description:      Define classes and required variables
History：  
V1.0	 -- initial version；2023-05-15；Arduino IDE : v1.8.15
**************************************************/
#pragma once

#include "Arduino.h"
#include "Wire.h"
#include "utility/BM_RangeSensor.h"
#include "utility/BML36M001_ErrorCodes.h"
#include "utility/BML36M001_Class.h"
class BML36M001
{
  public:
    BML36M001( int intPin = 22,TwoWire *theWire=&Wire1); 
    bool begin(uint8_t  i2c_addr = 0x29); 
    uint8_t getINT();
    void startRanging(); 
    void stopRanging(); 
    bool checkForDataReady();
    uint16_t readDistance();
    void clearInterrupt();  
    void calibrateOffset(uint16_t targetDistanceInMm); 
    void calibrateXTalk(uint16_t targetDistanceInMm);  
    
    int getI2CAddress(); 
    uint16_t getTimingBudgetInMs();
    uint16_t getIntermeasurementPeriod();
    uint8_t getDistanceMode(); 
    uint8_t getInterruptPolarity();
    void setI2CAddress(uint8_t i2c_addr); 
    void setTimingBudgetInMs(uint16_t timingBudget);
    void setIntermeasurementPeriod(uint16_t intermeasurement); 
    void setDistanceModeLong(); 
    void setDistanceModeShort(); 
    void setInterruptPolarityHigh();
    void setInterruptPolarityLow(); 
    
    
  private:
    bool checkID(); 
    VL53L1X_Version_t getSoftwareVersion(); 
    bool checkBootState(); 
    uint16_t getSensorID(); 
    uint16_t getSignalPerSpad(); 
    uint16_t getAmbientPerSpad(); 
    uint16_t getSignalRate(); 
    uint16_t getSpadNb(); 
    uint16_t getAmbientRate();
    uint8_t getRangeStatus(); 
    void setDistanceThreshold(uint16_t lowThresh, uint16_t hiThresh, uint8_t window);
    uint16_t getDistanceThresholdWindow(); 
    uint16_t getDistanceThresholdLow(); 
    uint16_t getDistanceThresholdHigh();
    void setOffset(int16_t offset); 
    int16_t getOffset(); 
    void setXTalk(uint16_t xTalk); 
    uint16_t getXTalk();
    /**Table of Optical Centers**
    *
    * 128,136,144,152,160,168,176,184,  192,200,208,216,224,232,240,248
    * 129,137,145,153,161,169,177,185,  193,201,209,217,225,233,241,249
    * 130,138,146,154,162,170,178,186,  194,202,210,218,226,234,242,250
    * 131,139,147,155,163,171,179,187,  195,203,211,219,227,235,243,251
    * 132,140,148,156,164,172,180,188,  196,204,212,220,228,236,244,252
    * 133,141,149,157,165,173,181,189,  197,205,213,221,229,237,245,253
    * 134,142,150,158,166,174,182,190,  198,206,214,222,230,238,246,254
    * 135,143,151,159,167,175,183,191,  199,207,215,223,231,239,247,255
  
    * 127,119,111,103, 95, 87, 79, 71,  63, 55, 47, 39, 31, 23, 15, 7
    * 126,118,110,102, 94, 86, 78, 70,  62, 54, 46, 38, 30, 22, 14, 6
    * 125,117,109,101, 93, 85, 77, 69,  61, 53, 45, 37, 29, 21, 13, 5
    * 124,116,108,100, 92, 84, 76, 68,  60, 52, 44, 36, 28, 20, 12, 4
    * 123,115,107, 99, 91, 83, 75, 67,  59, 51, 43, 35, 27, 19, 11, 3
    * 122,114,106, 98, 90, 82, 74, 66,  58, 50, 42, 34, 26, 18, 10, 2
    * 121,113,105, 97, 89, 81, 73, 65,  57, 49, 41, 33, 25, 17, 9, 1
    * 120,112,104, 96, 88, 80, 72, 64,  56, 48, 40, 32, 24, 16, 8, 0 Pin 1
    *
    * To set the center, set the pad that is to the right and above the exact center of the region you'd like to measure as your opticalCenter*/
    void setROI(uint8_t x, uint8_t y, uint8_t opticalCenter); 
    uint16_t getROIX(); 
    uint16_t getROIY(); 
    void setSignalThreshold(uint16_t signalThreshold); 
    uint16_t getSignalThreshold(); 
    void setSigmaThreshold(uint16_t sigmaThreshold);
    uint16_t getSigmaThreshold(); 
    void startTemperatureUpdate(); 
    TwoWire *_i2cPort;
    int _interruptPin;
    int _i2cAddress = 0x29;
    VL53L1X* _device;
};
