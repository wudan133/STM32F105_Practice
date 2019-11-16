#include "usbh_usr.h"
#include "usbh_ioreq.h"
#include "usb_hcd_int.h"
#include "SysTick.h"
#include "string.h"  
#include "usbh_hcs.h"
//////////////////////////////////////////////////////////////////////////////////   
//*******************************************************************************
//修改信息
//无
//////////////////////////////////////////////////////////////////////////////////     
 
//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 bDeviceState=0;     //默认没有连接  

MS2109_machine_TypeDef MS2109_machine;
__IO uint8_t start_toggle = 0;

extern USB_OTG_CORE_HANDLE USB_OTG_Core_dev;
u8 USB_FIRST_PLUGIN_FLAG=0; //USB第一次插入标志,如果为1,说明是第一次插入

//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{ 
    USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}  

//USB HOST 用户回调函数.
USBH_Usr_cb_TypeDef USR_Callbacks =
{
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    NULL,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError
};
 
//USB HOST 初始化 
void USBH_USR_Init(void)
{
    printf("USB OTG FS MSC Host\r\n");
    printf("> USB Host library started.\r\n");
    printf("  USB Host Library v2.1.0\r\n\r\n");
}

//检测到U盘插入
void USBH_USR_DeviceAttached(void)//U盘插入
{ 
    printf("检测到USB设备插入!\r\n");
}

//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void)//U盘移除
{ 
    printf("USB设备拔出!\r\n");
    bDeviceState=0; //USB设备拔出了
}

//复位从机
void USBH_USR_ResetDevice(void)
{
    printf("复位设备...\r\n");
}

//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
    {
        printf("高速(HS)USB设备!\r\n");
    }  
    else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
    {
        printf("全速(FS)USB设备!\r\n"); 
    }
    else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
    {
        printf("低速(LS)USB设备!\r\n");  
    }
    else
    {
        printf("设备错误!\r\n");  
    }
}

//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
    USBH_DevDesc_TypeDef *hs;
    hs=DeviceDesc;   
    printf("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
    printf("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct); 
    
    if((*hs).idVendor == 0x534D)
    {
        MS2109_machine.MS2109_valid = 1;
    }
    else
    {
        MS2109_machine.MS2109_valid = 0;
    }
}

//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void)
{
    printf("从机地址分配成功!\r\n");   
}

//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
//  USBH_InterfaceDesc_TypeDef *id; 
//  u8 i ,j;
//  i = cfgDesc->bNumInterfaces;
//  
//  for(j=0;j<i;j++)
//  {
//          id = itfDesc;   
//          id = id+j;
//    if(id->bNumEndpoints>0)
//      {
//          if((*id).bInterfaceClass==0x08)
//          {
//              printf("可移动存储器设备!\r\n"); 
//          }else if((*id).bInterfaceClass==0x03)
//          {
//              printf("HID 设备!\r\n"); 
//          }
//          else if((*id).bInterfaceClass==0x0e)
//          {
//              printf("USB VIDEO 设备!\r\n"); 
//          }
//        else if((*id).bInterfaceClass==0x01)
//          {
//              printf("USB Audio 设备!\r\n"); 
//          }
//          else if((*id).bInterfaceClass==0xEF)
//          {
//              printf("Miscellaneous 设备!\r\n"); 
//          }   
//          else
//          {
//              printf("Unknown 设备!\r\n"); 
//          }
//      }

//  }
}
//获取到设备Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
    printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//获取到设备Product String 
void USBH_USR_Product_String(void *ProductString)
{
    printf("Product: %s\r\n",(char *)ProductString);  
}
//获取到设备SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
    printf("Serial Number: %s\r\n",(char *)SerialNumString);    
} 
//设备USB枚举完成
void USBH_USR_EnumerationDone(void)
{ 
    printf("设备枚举完成!\r\n\r\n");    
} 
//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{ 
    printf("无法识别的USB设备!\r\n\r\n");    
}  
//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{ 
    //printf("跳过用户确认步骤!\r\n");
    bDeviceState=1;//USB设备已经连接成功
    return USBH_USR_RESP_OK;
} 
//USB接口电流过载
void USBH_USR_OverCurrentDetected (void)
{
    printf("端口电流过大!!!\r\n");
}  
//重新初始化
void USBH_USR_DeInit(void)
{
    printf("重新初始化!!!\r\n");
}
//无法恢复的错误!!  
void USBH_USR_UnrecoveredError (void)
{
    printf("无法恢复的错误!!!\r\n\r\n");    
}

// 01 0b 01 00 03 00 00 00
USBH_Status MS2109_set_interface(USB_OTG_CORE_HANDLE *pdev, 
                                 USBH_HOST *phost)
{
  phost->Control.setup.b.bmRequestType = 0x01; 
  phost->Control.setup.b.bRequest = 0x0b;  
  phost->Control.setup.b.wValue.w = 0x0001; 
  phost->Control.setup.b.wIndex.w = MS2109_machine.MS2109_data;  // 01 Video 03 Audio
  phost->Control.setup.b.wLength.w = 0;

  return USBH_CtlReq(pdev, phost,MS2109_machine.buff , 0 );
}


USBH_Status MS2109_SFR_reg_write(USB_OTG_CORE_HANDLE *pdev, 
                                 USBH_HOST *phost)
{
    phost->Control.setup.b.bmRequestType = 0x21; 
    phost->Control.setup.b.bRequest = 0x09;  
    phost->Control.setup.b.wValue.w = 0x0300;

    phost->Control.setup.b.wIndex.w = 0x0004;
    phost->Control.setup.b.wLength.w = 8;
    MS2109_machine.buff[0] = 0xC6;
    MS2109_machine.buff[1] = MS2109_machine.MS2109_addr;
    MS2109_machine.buff[2] = MS2109_machine.MS2109_data; //data 
    return USBH_CtlReq(pdev, phost,MS2109_machine.buff , 8 );
}

USBH_Status MS2109_xdata_reg_write(USB_OTG_CORE_HANDLE *pdev, 
                                   USBH_HOST *phost)
{
    phost->Control.setup.b.bmRequestType = 0x21; 
    phost->Control.setup.b.bRequest = 0x09;  
    phost->Control.setup.b.wValue.w = 0x0300; 
    phost->Control.setup.b.wIndex.w = 0x0004;
    phost->Control.setup.b.wLength.w = 8;
    MS2109_machine.buff[0] = 0xb6;
    MS2109_machine.buff[1] = MS2109_machine.MS2109_addr>>8; //高位地址
    MS2109_machine.buff[2] = MS2109_machine.MS2109_addr; //低位地址
    MS2109_machine.buff[3] = MS2109_machine.MS2109_data; //data 
    return USBH_CtlReq(pdev, phost,MS2109_machine.buff , 8 );
}


USBH_Status MS2109_SFR_read_cmd_step1(USB_OTG_CORE_HANDLE *pdev, 
                                      USBH_HOST *phost)
{
    phost->Control.setup.b.bmRequestType = 0x21; 
    phost->Control.setup.b.bRequest = 0x09;  
    phost->Control.setup.b.wValue.w = 0x0300;

    phost->Control.setup.b.wIndex.w = 0x0004;
    phost->Control.setup.b.wLength.w = 8;
    MS2109_machine.buff[0] = 0xC5;
    MS2109_machine.buff[1] = MS2109_machine.MS2109_addr; 
    return USBH_CtlReq(pdev, phost,MS2109_machine.buff , 8);
}


USBH_Status MS2109_read_cmd_step2(USB_OTG_CORE_HANDLE *pdev, 
                                  USBH_HOST *phost)
{
    phost->Control.setup.b.bmRequestType = 0xa1; 
    phost->Control.setup.b.bRequest = 0x01;  
    phost->Control.setup.b.wValue.w = 0x0300;

    phost->Control.setup.b.wIndex.w = 0x0004;
    phost->Control.setup.b.wLength.w = 8;
    return USBH_CtlReq(pdev, phost,MS2109_machine.buff, 8);
}

//uint8_t MS2109_tvd_flag = 0;
uint8_t MS2109_read_SFR_data(USB_OTG_CORE_HANDLE *pdev, 
                             USBH_HOST *phost)
{
    if(MS2109_machine.MS2109_Step == 1)
    {
        if(MS2109_SFR_read_cmd_step1(pdev,phost) ==USBH_OK) 
        {
            MS2109_machine.MS2109_Step = 2;
        }
    }
    else if(MS2109_machine.MS2109_Step == 2)
    {
        if(MS2109_read_cmd_step2(pdev,phost)==USBH_OK)
        {
            MS2109_machine.MS2109_data = MS2109_machine.buff[2];
            MS2109_machine.MS2109_Step = 0;
             return 1;  
        }
    }           
    return 0;   
}

USBH_Status MS2109_xdata_read_cmd_step1(USB_OTG_CORE_HANDLE *pdev, 
                                        USBH_HOST *phost)
{
    phost->Control.setup.b.bmRequestType = 0x21; 
    phost->Control.setup.b.bRequest = 0x09;  
    phost->Control.setup.b.wValue.w = 0x0300;

    phost->Control.setup.b.wIndex.w = 0x0004;
    phost->Control.setup.b.wLength.w = 8;
    
    MS2109_machine.buff[0] = 0xb5;
    MS2109_machine.buff[1] =  MS2109_machine.MS2109_addr>>8;
    MS2109_machine.buff[2] = MS2109_machine.MS2109_addr; 
    return USBH_CtlReq(pdev, phost,MS2109_machine.buff , 8);
}

//uint8_t MS2109_tvd_flag = 0;
uint8_t MS2109_read_xdata_data(USB_OTG_CORE_HANDLE *pdev, 
                               USBH_HOST *phost)
{
    if(MS2109_machine.MS2109_Step == 1)
    {
        if(MS2109_xdata_read_cmd_step1(pdev,phost) ==USBH_OK)   
        {
            MS2109_machine.MS2109_Step = 2;
        }
    }
    else if(MS2109_machine.MS2109_Step == 2)
    {
        if(MS2109_read_cmd_step2(pdev,phost)==USBH_OK)
        {
            MS2109_machine.MS2109_Step = 0;
            MS2109_machine.MS2109_data = MS2109_machine.buff[3];
             return 1;  
        }
    }
    return 0;
}

USBH_Status MS2109_test_Init(USB_OTG_CORE_HANDLE *pdev , void *phost)
{
    USBH_HOST *pphost = phost;
    MS2109_machine.MS2109_State = 0;
    MS2109_machine.MS2109_Step = 0;
    
    if(MS2109_machine.MS2109_valid == 1)  // 有效的MS2109 设备，根据VID/PID判断
    {
        MS2109_machine.hc_num_in = USBH_Alloc_Channel(pdev, pphost->device_prop.Ep_Desc[3][0].bEndpointAddress);  
        MS2109_machine.length  = pphost->device_prop.Ep_Desc[3][0].wMaxPacketSize;
        MS2109_machine.poll      = pphost->device_prop.Ep_Desc[3][0].bInterval ;
        /* Open channel for IN endpoint */
        USBH_Open_Channel  (pdev,
                            MS2109_machine.hc_num_in,
                            pphost->device_prop.address,
                            pphost->device_prop.speed,
                            EP_TYPE_INTR,
                            MS2109_machine.length);
    }
    //printf("hid_test_Init\r\n");
    return USBH_OK;
}

void MS2109_test_DeInit(USB_OTG_CORE_HANDLE *pdev , void *phost)
{
//  printf("hid_test_DeInit\r\n");
}

USBH_Status  MS2109_test_Requests(USB_OTG_CORE_HANDLE *pdev , void *phost)
{
//  printf("hid_test_Requests\r\n");
    return USBH_OK;
}

 

USBH_Status  MS2109_test_Machine(USB_OTG_CORE_HANDLE *pdev , void *phost)
{
     USBH_HOST *pphost = phost; 
    uint16_t i ,j;
    
    switch(MS2109_machine.MS2109_State)
    {
    case STATE_IDLE:
        break;
    case STATE_SFR_reg_read:
        if(MS2109_read_SFR_data(pdev,  pphost)==1)//
        {
            MS2109_machine.MS2109_State = STATE_IDLE;
        }
        break;
    case STATE_xdata_reg_read:
        if(MS2109_read_xdata_data(pdev,  pphost)==1)//
        {
             MS2109_machine.MS2109_State = STATE_IDLE  ;
        }       
        break;
    case STATE_SFR_reg_write:
        if(MS2109_SFR_reg_write(pdev,  pphost)  == USBH_OK)
        {
             MS2109_machine.MS2109_State = STATE_IDLE;   //usb 恢复idle 已完成TVD REG 写
        }
        break;
    case STATE_xdata_reg_write:
        if(MS2109_xdata_reg_write(pdev,  pphost)    == USBH_OK)
        {
             MS2109_machine.MS2109_State = STATE_IDLE;   //usb 恢复idle 已完成XDATA REG 写
        }
        break;
    case STATE_set_interface:
        if(MS2109_set_interface(pdev,  pphost)  == USBH_OK)
        {
             MS2109_machine.MS2109_State = STATE_IDLE;   // 
        }
        break;  
    case STATE_GET_DATA:

//        USBH_IsocReceiveData(pdev, 
//                             MS2109_machine.buff,
//                             MS2109_machine.length,
//                             MS2109_machine.hc_num_in);
        USBH_InterruptReceiveData(pdev, 
                                  MS2109_machine.buff,
                                  MS2109_machine.length,
                                  MS2109_machine.hc_num_in);
        start_toggle = 1;
        
        MS2109_machine.MS2109_State = STATE_POLL;
        MS2109_machine.timer = HCD_GetCurrentFrame(pdev);
        break;
    case STATE_POLL:
        if(( HCD_GetCurrentFrame(pdev) - MS2109_machine.timer) >= MS2109_machine.poll)
        {
            MS2109_machine.MS2109_State = STATE_GET_DATA;
        }
        else if(HCD_GetURB_State(pdev , MS2109_machine.hc_num_in) == URB_DONE)
        {
            if(start_toggle == 1) /* handle data once */
            {
                start_toggle = 0;
                MS2109_machine.audio_counter++;
                i = MS2109_machine.buff[0]|MS2109_machine.buff[1]<<8;
                i = i>>3;
                j = MS2109_machine.buff[2]|MS2109_machine.buff[3]<<8;
                j = j>>3;
            
                printf("audio L 采样值：%4d ,\t audio R 采样值：%4d \r\n",i,j);
            
                 if(MS2109_machine.audio_counter==5)  //  忽略前32 次采样，以第33次为初始值
                 {
                      if(i>j)
                        {
                            MS2109_machine.audio_high = i;   
                            MS2109_machine.audio_low = j;
                        }
                        else{
                            MS2109_machine.audio_high = j;   
                            MS2109_machine.audio_low = i;
                        }                       

                 }
                 if(MS2109_machine.audio_counter>4)  //  
                 {
                    if(i>j)
                    {
                         if((i-j)>800)
                         {
                                MS2109_machine.audio_high = i;
                                MS2109_machine.audio_low = j;
                         }
                    }
                    else
                    {
                         if((j-i)>800)
                         {
                                MS2109_machine.audio_high = j;
                                MS2109_machine.audio_low = i;
                         }
                    }
                }
            }
        }
        else if(HCD_GetURB_State(pdev, MS2109_machine.hc_num_in) == URB_STALL) /* IN Endpoint Stalled */
        {
          printf("iso_URB_STALL");
          /* Issue Clear Feature on interrupt IN endpoint */ 
    //      if( (USBH_ClrFeature(pdev, 
    //                           pphost,
    //                           HID_Machine.ep_addr,
    //                           HID_Machine.hc_num_in)) == USBH_OK)
    //      {
    //        /* Change state to issue next IN token */
    //        HID_Machine.state = HID_GET_DATA;
    //        
    //      }
        }
        break;
    default:
        break;
    }
    return USBH_OK;
}


USBH_Class_cb_TypeDef  MS2109_cb = 
{
  MS2109_test_Init,
  MS2109_test_DeInit,
  MS2109_test_Requests,
  MS2109_test_Machine
};


// data_type  = 1 TVD 数据读 data_type = 2， xdata 数据读


/**
  * @brief  MS2109_read_data
  *         读取 MS2109 的数据 或者寄存器
  * @param  data_type: 1 读取TVD 寄存器  2 ：读取XDATA数据
  * @param  addr: 要读取的数据地址，实际TVD的数据地址只需要低位
  * @retval 返回实际读取到的数据
  */
uint8_t MS2109_read_data(uint8_t data_type,uint16_t addr)
{
    uint16_t i = 0;
      if(MS2109_machine.MS2109_valid == 1)  // 根据VID 判断是否有效的设备
        {
            MS2109_machine.MS2109_addr = addr;
            MS2109_machine.MS2109_Step = 1;
            MS2109_machine.MS2109_State = data_type ; //1 启动一次 TVD 数据读  2 启动一次XDATA数据读    
            while(MS2109_machine.MS2109_State != 0) //等待传输完成 ,实际USB 数据传输在Timer3 中断调用USB数据处理完成的  
            {
                 i++;
                if(i>15)
                {
                    MS2109_machine.MS2109_data  = 0xff;   // 如果读取数据时间超时，返回0xff 无效的数据
                    break;
                }
                Delay_ms(1);
            }
          //printf("MS2109_read_data指令耗时：%4d ms\r\n",i);
            
          //return MS2109_machine.buff[0]; // 返回读取的数据值  
          return MS2109_machine.MS2109_data;
        }

     return 0 ;
}

/**
  * @brief  MS2109_write_data
  *         向指定地址写MS2109的数据
  * @param  data_type: 3  写TVD 寄存器  4 ：写XDATA数据
  * @param  addr: 要写入的数据地址，实际TVD的数据地址只需要低位
  * @param  value: 要写入的数据值
  * @retval 返回 无
  */
void MS2109_write_data(uint8_t data_type,uint16_t addr,uint8_t value)
{
        uint16_t i = 0;
      if(MS2109_machine.MS2109_valid == 1)  //根据VID 判断是否有效的设备
        {
            MS2109_machine.MS2109_State = data_type ; //3 启动一次 TVD 数据读  4 启动一次XDATA数据读
            MS2109_machine.MS2109_addr = addr;
            MS2109_machine.MS2109_data = value;             
            while(MS2109_machine.MS2109_State != 0) //等待传输完成 ,实际USB 数据传输在Timer3 中断调用USB数据处理完成的  
            {
                 i++;
                if(i>10)
                {
                    break;
                }
                Delay_ms(1);
            }
//          printf("MS2109_write_data指令耗时：%4d ms\r\n",i);
        }
}


/**
  * @brief  MS2109_write_data
  *         向指定地址写MS2109的数据
  * @param  value: 1 打开视频通道   3  打开音频通道 
  * @retval 返回 无
  */
void MS2109_setup(uint8_t value)
{
      uint16_t i;
      if(MS2109_machine.MS2109_valid == 1)  // 
        {
            MS2109_machine.MS2109_State = 5 ; // 
            MS2109_machine.MS2109_data = value;
            i = 0;
            while(MS2109_machine.MS2109_State != 0) //等待传输完成 ,实际USB 数据传输在Timer3 中断调用USB数据处理完成的  
            {
                 i++;
                if(i>10)
                {
                    break;
                }
                Delay_ms(1);
            }
//          printf("setup指令耗时：%4d ms\r\n",i);
        }
}

/**
  * @brief  MS2109_audio_iso_receive
  *         启动端点3 ISO 数据传输（实际是启动中断传输）
  * @param  无
  * @retval 无
  */
void MS2109_audio_iso_receive(_Bool audio_enable)
{
    if(audio_enable)  // 
    {
        MS2109_machine.MS2109_State = STATE_GET_DATA ; //
    }
    else
    {
        MS2109_machine.MS2109_State = STATE_IDLE ; //   
    }
}


/**
  * @brief  MS2109_int_audio_test
  *         内部Audio adc 测试
  * @param  无
  * @retval 0，测试通过， 11  测试Fail
  */
uint8_t MS2109_int_audio_test(void)
{
    uint16_t i;
    MS2109_machine.audio_high = 0;
    MS2109_machine.audio_low = 0;
    MS2109_machine.audio_counter = 0;
    //MS2109_setup(3);   // 已经在USB 初始化完成后处理
    MS2109_audio_iso_receive(TRUE);
    i=0;
    while(MS2109_machine.audio_counter<10)  //  从第4次ISO 传输开始取数据，最大值-最小值大于800 认为是有效的，
    {
         i++;
        if(i>120)
        {
            break;
        }
        Delay_ms(1);
    }
    printf("音频采样耗时：%4d ms\r\n",i);
    
    MS2109_audio_iso_receive(FALSE);
    printf("audio 高电平ADC 采样值：%4d ,\t 理论值：2520\r\n",MS2109_machine.audio_high);
    printf("audio 低电平ADC 采样值：%4d ,\t 理论值：1480\r\n",MS2109_machine.audio_low);
    if((MS2109_machine.audio_high>2400) && (MS2109_machine.audio_high<2650))  // 高电平大于2400，小于2650
    {
        if((MS2109_machine.audio_low>1350) && (MS2109_machine.audio_low<1600))  // 低电平大于1350，小于1600
        {
             return 0 ;
        }
    }
    return 11 ;//Fail 11
}


u8 HID_ReadByte(u16 u16_index)
{
    return MS2109_read_data(2, u16_index);
}

void HID_WriteByte(u16 u16_index, u8 u8_value)
{
    MS2109_write_data(4, u16_index, u8_value);
}

void HID_ModBits(u16 u16_index, u8 u8_mask, u8 u8_value)
{
    u8 tmp;
    tmp  = HID_ReadByte(u16_index);
    tmp &= ~u8_mask;
    tmp |= (u8_value & u8_mask);
    HID_WriteByte(u16_index, tmp); 
}

void HID_ClrBits(u16 u16_index, u8 u8_mask)
{
    HID_ModBits(u16_index, u8_mask, 0x00);  
}

void HID_SetBits(u16 u16_index, u8 u8_mask)
{
    HID_ModBits(u16_index, u8_mask, u8_mask); 
}

void HID_ToggleBits(u16 u16_index, u8 u8_mask, u8 u8_set)
{
    (u8_set) ? HID_SetBits(u16_index, u8_mask) : HID_ClrBits(u16_index, u8_mask);
}
