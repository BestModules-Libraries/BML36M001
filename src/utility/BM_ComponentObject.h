/*************************************************
File:       		BM_ComponentObject.h
Author:            	llxin, BESTMODULE
Description:        This file contains the abstract class describing the interface of a
					generic component
History：			版本修改描述
					V1.0	 -- initial version；2021-12-3；Arduino IDE : ≥v1.8.15

**************************************************/



/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __COMPONENT_OBJECT_CLASS_H
#define __COMPONENT_OBJECT_CLASS_H


/* Includes ------------------------------------------------------------------*/

#include "stdint.h"


/* Classes  ------------------------------------------------------------------*/

/** An abstract class for Generic components.
 */
class ComponentObject
{
public:
    /**
     * @brief     Initializing the component.
     * @param[in] init pointer to device specific initalization structure.
     * @retval    "0" in case of success, an error code otherwise.
     */
    virtual int Init() = 0;

    /**
     * @brief      getting the ID of the component.
     * @param[out] id pointer to an allocated variable to store the ID into.
     * @retval     "0" in case of success, an error code otherwise.
     */
    virtual int ReadID() = 0;
};

#endif /* __COMPONENT_OBJECT_CLASS_H */

