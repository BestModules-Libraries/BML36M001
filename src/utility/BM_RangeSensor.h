/*************************************************
File:       		BM_RangeSensor.h
Author:            	llxin, BESTMODULE
Description:        This file contains the abstract class describing in general 
					the interfaces of a range sensor
History：			版本修改描述
					V1.0	 -- initial version；2021-12-3；Arduino IDE : ≥v1.8.15

**************************************************/


/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __RANGE_SENSOR_CLASS_H
#define __RANGE_SENSOR_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include "BM_ComponentObject.h"

/* Classes  ------------------------------------------------------------------*/
/** An abstract class for range sensors
 */
class RangeSensor : public ComponentObject
{
 public:
    /**
     * @brief       get current range [mm]
     * @param[out]  piData Pointer to where to store range to
     * @return      0 in case of success, an error code otherwise
     */
    virtual int getDistance(uint32_t *piData) = 0;
};

#endif /* __RANGE_SENSOR_CLASS_H */
