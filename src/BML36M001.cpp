/*************************************************
File:       	   BML36M001.cpp
Author:            BEST MODULES CORP.
Description:       Define classes and required variables
Version：          V1.0.4	 --2025-03-13
**************************************************/
#include "BML36M001.h"
/**********************************************************
Description: Constructor
Parameters:  intPin: INT Output p*in connection with Arduino, 
                     the INT will be pulled down when an object approaches
             *theSerial: Wire object if your board has more than one IIC interface      
                         parameter range:&wire、&wire1、&wire2
Return:       none     
Others:       none      
**********************************************************/
BML36M001::BML36M001(int intPin,TwoWire *theWire)
{
  _i2cPort = theWire;
  _interruptPin = intPin;
  _device = new VL53L1X(_i2cPort, -1, intPin);
}

/**********************************************************
Description: Initialize mode
Parameters:  i2c_addr:Address of the IIC communication device(0x00~0xff)   
Return:      Initialization result
                             true： successful
                             false: fail  
Others:       none            
**********************************************************/
bool BML36M001::begin(uint8_t  i2c_addr)
{
	bool status;
	pinMode(_interruptPin,INPUT);
	if (checkID() == false)
		{
		  return (VL53L1_ERROR_PLATFORM_SPECIFIC_START);
		}
	status = _device->VL53L1X_SensorInit();
  delay(80);
  setI2CAddress(i2c_addr);
  return status;
}

/**********************************************************
Description: Get INT Status
Parameters:   none           
Return:      INT Status：
                        0:INT output low level 
                        1:INT output high level  
Others:        none      
**********************************************************/
uint8_t BML36M001::getINT()
{
     return (digitalRead(_interruptPin));
}

/**********************************************************
Description:  Start the distance measurement
Parameters:    none           
Return:           void     
Others:       This function needs to be called before the probe
**********************************************************/
void BML36M001::startRanging()
{
  _device->VL53L1X_StartRanging();
}

/**********************************************************
Description:  Stop measuring distance
Parameters:    none           
Return:           void     
Others:       none     
**********************************************************/
void BML36M001::stopRanging()
{
  _device->VL53L1X_StopRanging();
}

/**********************************************************
Description:   Check if the distance data is ready
Parameters:    none         
Return:        Whether the measurement data can be read  
                       true: The measurements are done and the data is ready  
                       false: Measurement not completed, data not ready  
Others:       none       
**********************************************************/
bool BML36M001::checkForDataReady()
{
  uint8_t dataReady;
  _device->VL53L1X_CheckForDataReady(&dataReady);
  return (bool)dataReady;
}

/**********************************************************
Description:  Get the ranging results
Parameters:    none         
Return:       ranging results(Unit: mm)    
Others:       none        
**********************************************************/
uint16_t BML36M001::readDistance()
{
  uint16_t distance;
  _device->VL53L1X_getDistance(&distance);
  return (int)distance;
}

/**********************************************************
Description:  Clear the interrupt flag
Parameters:    none   
Return:       void 
Others:       After reading the data, the interrupt flag bit must be cleared.
              If the interrupt flag bit is not cleared, the data ready state flag will not be updated,
              and the external interrupt pin will not be cleared.
**********************************************************/
void BML36M001::clearInterrupt()
{
  _device->VL53L1X_ClearInterrupt();
  delay(50);
}

/**********************************************************
Description:  Automatic calibration of offsets
Parameters:   targetDistanceInMm: The distance between the module and the calibration point, Radius:40~4000(unit:mm)      
Return:       void 
Others:       Automatically calibrates the offset by placing a target
              with a gray value of 17% at a known distance from the sensor
              and passing this known distance into the function.  
**********************************************************/
void BML36M001::calibrateOffset(uint16_t targetDistanceInMm)
{
  int16_t offset = getOffset();
  _device->VL53L1X_CalibrateOffset(targetDistanceInMm, &offset);
}

/**********************************************************
Description:  Automatic calibration of crosstalk values
Parameters:   targetDistanceInMm:The distance between the module and the calibration point, Radius:40~4000(unit:mm)      
Return:       void 
Others:       Crosstalk is automatically calibrated by placing a target
            with a gray value of 17% at a known distance from the sensor 
            and passing this known distance into the function.   
**********************************************************/
void BML36M001::calibrateXTalk(uint16_t targetDistanceInMm)
{
  uint16_t xTalk = getXTalk();
  _device->VL53L1X_CalibrateXtalk(targetDistanceInMm, &xTalk);
};

/**********************************************************
Description:  Get the IIC address of the BML36M001
Parameters:    none  
Return:       IIC address of the BML36M001 
Others:       none
**********************************************************/
int BML36M001::getI2CAddress()
{
  return _i2cAddress;
}

/**********************************************************
Description:  Get the timing budget for a measurement
Parameters:    none   
Return:       timing budget for a measurement: 20、33、50、100、200、500(Unit: ms)
Others:           none        
**********************************************************/
uint16_t BML36M001::getTimingBudgetInMs()
{
  uint16_t timingBudget;
  _device->VL53L1X_getTimingBudgetInMs(&timingBudget);
  return timingBudget;
}

/**********************************************************
Description:  Get the measurement interval
Parameters:   none
Return:       measurement interval:20~1000(Unit: ms)    
Others:       none 
**********************************************************/
uint16_t BML36M001::getIntermeasurementPeriod()
{
  uint16_t intermeasurement;
  _device->VL53L1X_getInterMeasurementInMs(&intermeasurement);
  return intermeasurement;
}

/**********************************************************
Description:  Get distance Mode
Parameters:   none
Return:       distance Mode
                           1: short range mode  
                           2: long range mode   
Others:       none 
**********************************************************/
uint8_t BML36M001::getDistanceMode()
{
  uint16_t distanceMode;
  _device->VL53L1X_getDistanceMode(&distanceMode);
  return distanceMode;
}

/**********************************************************
Description:   Gets the level of the interrupt pin when triggered
Parameters:     none
Return:        level of the interrupt pin when triggered
                 1: The interrupt pin is high when triggered
                 0: The interrupt pin is low when triggered 
Others:        none
**********************************************************/
uint8_t BML36M001::getInterruptPolarity()
{
  uint8_t tmp;
  _device->VL53L1X_getInterruptPolarity(&tmp);
  return tmp;
}

/**********************************************************
Description:  Set the IIC address of the BML36M001
Parameters:   i2c_addr:BML36M001 Device address,Radius: 0x00~0x7f (Default 0x29)      
Return:       void 
Others:       none 
**********************************************************/
void BML36M001::setI2CAddress(uint8_t i2c_addr)
{
  _i2cAddress = i2c_addr;
  i2c_addr = ((i2c_addr << 1) & 0xFE);
  _device->VL53L1X_setI2CAddress(i2c_addr);
}

/**********************************************************
Description:  Set a single measurement duration
Parameters:   timingBudget: single measurement duration,Radius:20、33、50、100、200、500(Unit: ms)     
Return:       void 
Others:       The interval time should be greater than or equal to the time budget   
**********************************************************/
void BML36M001::setTimingBudgetInMs(uint16_t timingBudget)
{
  _device->VL53L1X_setTimingBudgetInMs(timingBudget);
}

/**********************************************************
Description:  Set the measurement interval
Parameters:   intermeasurement:measurement interval,Radius:20~1000(Unit: ms)     
Return:       void 
Others:       The time budget should be less than or equal to the interval time  
**********************************************************/
void BML36M001::setIntermeasurementPeriod(uint16_t intermeasurement)
{
  _device->VL53L1X_setInterMeasurementInMs(intermeasurement);
}

/**********************************************************
Description:  Set ranging mode to long range mode
Parameters:   none       
Return:       void 
Others:       Measuring range: 40~4000(Unit: mm)
              Environmental conditions: Ambient light darkness     
**********************************************************/
void BML36M001::setDistanceModeLong()
{
  _device->VL53L1X_setDistanceMode(2);
}

/**********************************************************
Description:  Set ranging mode to short range mode
Parameters:   none     
Return:       void 
Others:       Measuring range: 40~1300(Unit: mm)
              Environmental conditions: Ambient light darkness     
**********************************************************/
void BML36M001::setDistanceModeShort()
{
  _device->VL53L1X_setDistanceMode(1);
}

/**********************************************************
Description:  Set the interrupt pin to high when triggered
Parameters:   none      
Return:       void 
Others:       Interrupt pin is high level: The distance data is ready 
              Interrupt pin is low level:  Distance data not ready    
**********************************************************/
void BML36M001::setInterruptPolarityHigh()
{
  _device->VL53L1X_setInterruptPolarity(1);
}

/**********************************************************
Description:  Set the interrupt pin to low when triggered
Parameters:   none   
Return:       void 
Others:       Interrupt pin is high level: Distance data not ready
              Interrupt pin is low level: The distance data is ready      
**********************************************************/
void BML36M001::setInterruptPolarityLow()
{
  _device->VL53L1X_setInterruptPolarity(0);
}

/**********************************************************
Description:   Checking the sensor ID
Parameters:     none
Return:         whether ID is correct
                                  true: ID is correct  
                                   false: ID is incorrect           
Others:         Sensor default ID: 0xEACC        
**********************************************************/
bool BML36M001::checkID()
{
	uint16_t sensorId;
	_device->VL53L1X_getSensorId(&sensorId);
	if (sensorId == 0xEACC)
 {
	return true;
 }
	return false;
}

/**********************************************************
Description:   Get the current ST software version
Parameters:     none
Return:        ST software version:
                         tempVersion.major: 1
                         tempVersion.minor: 0  
                         tempVersion.build: 1
                         tempVersion.revision: 0             
Others:         none    
**********************************************************/
VL53L1X_Version_t BML36M001::getSoftwareVersion()
{
	VL53L1X_Version_t tempVersion;
	_device->VL53L1X_getSWVersion(&tempVersion);
	return tempVersion;
}

/**********************************************************
Description:   Check if the VL53L1X has been initialized
Parameters:     none
Return:       Initialization condition 
                       true: initialized 
                       false: Not initialized              
Others:        none     
**********************************************************/
bool BML36M001::checkBootState()
{
	uint8_t bootState;
	_device->VL53L1X_BootState(&bootState);
	return (bool)bootState;
}

/**********************************************************
Description:   Get the sensor ID
Parameters:     none
Return:        sensor ID(Default 0xEACC)               
Others:         none      
**********************************************************/
uint16_t BML36M001::getSensorID()
{
	uint16_t id;
	_device->VL53L1X_getSensorId(&id);
	return id;
}

/**********************************************************
Description:    Get the average signal rate per SPAD 
Parameters:      none
Return:         average signal rate per SPAD(Unit: kcps/SPAD)                 
Others:             none            
**********************************************************/
uint16_t BML36M001::getSignalPerSpad()
{
	uint16_t temp;
	_device->VL53L1X_getSignalPerSpad(&temp);
	return temp;
}

/**********************************************************
Description:   Get ambient noise when not measured
Parameters:      none 
Return:         ambient noise when not measured(Unit: kcps/SPAD)                 
Others:             none               
**********************************************************/
uint16_t BML36M001::getAmbientPerSpad()
{
	uint16_t temp;
	_device->VL53L1X_getAmbientPerSpad(&temp);
	return temp;
}

/**********************************************************
Description:   Gets the total signal rate
Parameters:      none
Return:         total signal rate(Unit: kcps)                  
Others:             none               
**********************************************************/
uint16_t BML36M001::getSignalRate()
{
	uint16_t temp;
	_device->VL53L1X_getSignalRate(&temp);
	return temp;
}

/**********************************************************
Description:   Gets the number of enabled Spad
Parameters:     none
Return:        number of enabled Spad                   
Others:        none     
**********************************************************/
uint16_t BML36M001::getSpadNb()
{
	uint16_t temp;
	_device->VL53L1X_getSpadNb(&temp);
	return temp;
}

/**********************************************************
Description:   Gets Ambient Rate
Parameters:     none
Return:        Ambient Rate(Unit: kcps)                      
Others:        none     
**********************************************************/
uint16_t BML36M001::getAmbientRate()
{
	uint16_t temp;
	_device->VL53L1X_getAmbientRate(&temp);
	return temp;
}

/**********************************************************
Description:   Get range state
Parameters:     none
Return:        State of the measured data
                    0: Ranging measurements are valid
                    1: Sigma estimator check is above the internal defined threshold
                   2: Signal value is below the internal defined threshold
                   4: phase is out of bounds
                    5: HW or VCSEL failure
                   7: Wrapped target, not matching phases
                   8: Internal algorithm underflow or overflow
                   14: The reported range is invalid                         
Others:        none  
**********************************************************/
uint8_t BML36M001::getRangeStatus()
{
	uint8_t temp;
	_device->VL53L1X_getRangeStatus(&temp);
	return temp;
}

/**********************************************************
Description:   Set a threshold-based distance detection mode
Parameters:    lowThresh: The lower limit of the detection distance(40~4000 unit: mm)
               hiThresh: The upper limit of the detection distance  (40~4000 unit: mm)
               window: Distance detection mode(0、1、2、3)
                    window = 0 : 
                                1.Object distance > lowThresh:  No report
                                2.Object distance < lowThresh:  report
                    window = 1 :
                                1.Object distance < hiThresh:  No report
                                2.Object distance > hiThresh:  report
                    window = 2 :
                                1.lowThresh <Object distance < hiThresh:  No report
                                2.lowThresh >Object distance > hiThresh:  report
                    window = 3 :
                                1.lowThresh >Object distance > hiThresh:  No report
                                2.lowThresh <Object distance < hiThresh:  report
Return:             void               
Others:             hiThresh >= lowThresh
**********************************************************/
void BML36M001::setDistanceThreshold(uint16_t lowThresh, uint16_t hiThresh, uint8_t window)
{
  _device->VL53L1X_setDistanceThreshold(lowThresh, hiThresh, window, 1);
}

/**********************************************************
Description:   Get Distance Threshold Window
Parameters:     none
Return:        Distance Threshold Window
                    0 : 
                        1.Object distance > lowThresh:  No report
                        2.Object distance < lowThresh:  report
                     1 :
                        1.Object distance < hiThresh:  No report
                        2.Object distance > hiThresh:  report
                    2 :
                       1.lowThresh <Object distance < hiThresh:  No report
                        2.lowThresh >Object distance > hiThresh:  report
                    3 :
                        1.lowThresh >Object distance > hiThresh:  No report
                       2.lowThresh <Object distance < hiThresh:  report                    
Others:        none     
**********************************************************/
uint16_t BML36M001::getDistanceThresholdWindow()
{
  uint16_t temp;
  _device->VL53L1X_getDistanceThresholdWindow(&temp);
  return temp;
}

/**********************************************************
Description:   Gets the lower limit of the detection distance
Parameters:     none
Return:         The lower limit of the detection distance(unit: mm)                        
Others:        none     
**********************************************************/
uint16_t BML36M001::getDistanceThresholdLow()
{
  uint16_t temp;
  _device->VL53L1X_getDistanceThresholdLow(&temp);
  return temp;
}

/**********************************************************
Description:   Gets the upper limit of the detection distance
Parameters:     none
Return:        The upper limit of the detection distance(unit: mm)                          
Others:        none     
**********************************************************/
uint16_t BML36M001::getDistanceThresholdHigh()
{
  uint16_t temp;
  _device->VL53L1X_getDistanceThresholdHigh(&temp);
  return temp;
}

/**********************************************************
Description:   Set offset
Parameters:    offset:
Return:        void                          
Others:        none     
**********************************************************/
void BML36M001::setOffset(int16_t offset)
{
	_device->VL53L1X_setOffset(offset);
}

/**********************************************************
Description:   Get offset
Parameters:     none
Return:        temp:The offset                            
Others:         none      
**********************************************************/
int16_t BML36M001::getOffset()
{
	int16_t temp;
	_device->VL53L1X_getOffset(&temp);
	return temp;
}

/**********************************************************
Description:   Set the value of crosstalk in counts per second 
Parameters:    xTalk:
Return:        void                         
Others:             none          
**********************************************************/
void BML36M001::setXTalk(uint16_t xTalk)
{
	_device->VL53L1X_setXtalk(xTalk);
}

/**********************************************************
Description:   Get the value of crosstalk in counts per second 
Parameters:     none
Return:        temp: The value of crosstalk in counts per second                          
Others:        none     
**********************************************************/
uint16_t BML36M001::getXTalk()
{
	uint16_t temp;
	_device->VL53L1X_getXtalk(&temp);
	return temp;
}

/**********************************************************
Description:   Set up ROI in SPADs
Parameters:    x: The width of the ROI 
               y: The height of the ROI
               opticalCenter: Optical center
Return:        void              
Others:      x>=4,y>=4          
**********************************************************/
void BML36M001::setROI(uint8_t x, uint8_t y, uint8_t opticalCenter)
{
	_device->VL53L1X_setROI(x, y, opticalCenter);
}

/**********************************************************
Description:   Get the width of the ROI
Parameters:    none
Return:        tempX               
Others:        none       
**********************************************************/
uint16_t BML36M001::getROIX()
{
	uint16_t tempX;
	uint16_t tempY;
	_device->VL53L1X_getROI_XY(&tempX, &tempY);
	return tempX;
}

/**********************************************************
Description:   Get the height of the ROI
Parameters:    none
Return:        tempY              
Others:        none       
**********************************************************/
uint16_t BML36M001::getROIY()
{
	uint16_t tempX;
	uint16_t tempY;
	_device->VL53L1X_getROI_XY(&tempX, &tempY);
	return tempY;
}

/**********************************************************
Description:   Set the signal threshold
Parameters:    signalthreshold:   
Return:        void         
Others:        none       
**********************************************************/
void BML36M001::setSignalThreshold(uint16_t signalThreshold)
{
	_device->VL53L1X_setSignalThreshold(signalThreshold);
}

/**********************************************************
Description:   Get the signal threshold
Parameters:    none  
Return:        temp: signal threshold           
Others:         none        
**********************************************************/
uint16_t BML36M001::getSignalThreshold()
{
	uint16_t temp;
	_device->VL53L1X_getSignalThreshold(&temp);
	return temp;
}

/**********************************************************
Description:   Set the Sigma threshold
Parameters:    sigmaThreshold:  
Return:        void     
Others:        none       
**********************************************************/
void BML36M001::setSigmaThreshold(uint16_t sigmaThreshold)
{
	_device->VL53L1X_setSigmaThreshold(sigmaThreshold);
}

/**********************************************************
Description:   Get the Sigma threshold
Parameters:    none 
Return:        temp: Sigma threshold       
Others:        none       
**********************************************************/
uint16_t BML36M001::getSigmaThreshold()
{
	uint16_t temp;
	_device->VL53L1X_getSigmaThreshold(&temp);
	return temp;
}

/**********************************************************
Description:   Recalibrates the sensor for temperature changes
Parameters:    none 
Return:        void    
Others:        Run this any time the temperature has changed by more than 8°C          
**********************************************************/
void BML36M001::startTemperatureUpdate()
{
	_device->VL53L1X_StartTemperatureUpdate();
}
