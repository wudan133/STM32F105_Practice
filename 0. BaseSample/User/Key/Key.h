/**
******************************************************************************
* @file    Key.h
* @author  dwu
* @version V1.0.0
* @date    29-Nov-2017
* @brief   Key driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __KEY_H__
#define	__KEY_H__
#include "stm32f10x.h"



typedef enum _KEY_MAP_
{
    KEY_MAP_SHORT0 = 0,
    KEY_MAP_SHORT1,
    KEY_MAP_SHORT2,
    KEY_MAP_SHORT3,
    KEY_MAP_SHORT4,
    KEY_MAP_SHORT5,
    KEY_MAP_SHORT6,
    KEY_MAP_SHORT7,
    KEY_MAP_CONTINUITY0,
    KEY_MAP_CONTINUITY1,
    KEY_MAP_CONTINUITY2,
    KEY_MAP_CONTINUITY3,
    KEY_MAP_CONTINUITY4,
    KEY_MAP_CONTINUITY5,
    KEY_MAP_CONTINUITY6,
    KEY_MAP_CONTINUITY7,
    KEY_MAP_LONG0,
    KEY_MAP_LONG1,
    KEY_MAP_LONG2,
    KEY_MAP_LONG3,
    KEY_MAP_LONG4,
    KEY_MAP_LONG5,
    KEY_MAP_LONG6,
    KEY_MAP_LONG7,

    KEY_NONE = 0xFF
} KEY_MAP_E;

void Key_GPIO_Config(void);
u8   Key_Detect(void);

#endif
