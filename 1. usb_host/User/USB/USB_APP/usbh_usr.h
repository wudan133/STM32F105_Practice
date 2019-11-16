/**
  ******************************************************************************
  * @file    usbh_usr.h
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file is the header file for usb usr file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USH_USR_H__
#define __USH_USR_H__


/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
//#include "usbh_hid_core.h"
#include "usb_conf.h"
#include <stdio.h>

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_DEMO_USER_CALLBACKS
* @{
*/
  
/** @defgroup USBH_USR 
  * @brief This file is the header file for user action
  * @{
  */ 


/** @defgroup USBH_CORE_Exported_Variables
  * @{
  */ 



extern  USBH_Usr_cb_TypeDef USR_Callbacks;

extern USBH_Class_cb_TypeDef  MS2109_cb;

//表示USB连接状态
//0,没有连接;
//1,已经连接;
extern vu8 bDeviceState;        //默认没有连接  


/**
  * @}
  */ 


/** @defgroup USBH_CORE_Exported_FunctionsPrototype
  * @{
  */ 


/* Structure for HID process */

typedef struct _MS2109_Process
{
    uint8_t             buff[256];
    uint8_t             MS2109_State; 
    uint8_t             MS2109_Step; 
    uint8_t             MS2109_valid;   // 有效性判断
    uint16_t            MS2109_addr;   
    uint8_t             MS2109_data; 
    uint8_t             hc_num_in; 
    uint16_t            length;
    uint8_t             poll; 
    __IO uint16_t       timer; 
    uint16_t            audio_high;     // audio 高电平数据
    uint16_t            audio_low;      // audio 低电平数据
    uint16_t            audio_counter;  // audio 低电平数据
}
MS2109_machine_TypeDef;

typedef enum
{
    STATE_IDLE= 0,
    STATE_SFR_reg_read ,        //1
    STATE_xdata_reg_read ,      //2
    STATE_SFR_reg_write,        //3
    STATE_xdata_reg_write,      //4
    STATE_set_interface,        //5
    STATE_GET_DATA,
    STATE_POLL,
    STATE_ERROR,
}
MS2109_State;


void USBH_USR_ApplicationSelected(void);
void USBH_USR_Init(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed); 
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);


//自行添加的函数
uint8_t MS2109_read_data(uint8_t data_type,uint16_t addr);
void MS2109_write_data(uint8_t data_type,uint16_t addr,uint8_t value);
void usb_setup(uint8_t value);
void MS2109_audio_iso_receive(_Bool audio_enable);
uint8_t MS2109_int_audio_test(void);
uint8_t MS2109_ext_audio_test(void);

//u8 USBH_Check_HIDCommDead(USB_OTG_CORE_HANDLE *pcore,HID_Machine_TypeDef *phidm);

u8 HID_ReadByte(u16 u16_index);

void HID_WriteByte(u16 u16_index, u8 u8_value);

void HID_ModBits(u16 u16_index, u8 u8_mask, u8 u8_value);

void HID_ClrBits(u16 u16_index, u8 u8_mask);

void HID_SetBits(u16 u16_index, u8 u8_mask);

void HID_ToggleBits(u16 u16_index, u8 u8_mask, u8 u8_set);

/**
  * @}
  */ 

#endif /* __USBH_USR_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
