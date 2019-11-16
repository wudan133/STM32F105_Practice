#include "usbh_usr.h"
#include "usbh_ioreq.h"
#include "usb_hcd_int.h"
#include "SysTick.h"
#include "string.h"  
#include "usbh_hcs.h"
//////////////////////////////////////////////////////////////////////////////////   
//*******************************************************************************
//�޸���Ϣ
//��
//////////////////////////////////////////////////////////////////////////////////     
 
//��ʾUSB����״̬
//0,û������;
//1,�Ѿ�����;
vu8 bDeviceState=0;     //Ĭ��û������  

MS2109_machine_TypeDef MS2109_machine;
__IO uint8_t start_toggle = 0;

extern USB_OTG_CORE_HANDLE USB_OTG_Core_dev;
u8 USB_FIRST_PLUGIN_FLAG=0; //USB��һ�β����־,���Ϊ1,˵���ǵ�һ�β���

//USB OTG �жϷ�����
//��������USB�ж�
void OTG_FS_IRQHandler(void)
{ 
    USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}  

//USB HOST �û��ص�����.
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
 
//USB HOST ��ʼ�� 
void USBH_USR_Init(void)
{
    printf("USB OTG FS MSC Host\r\n");
    printf("> USB Host library started.\r\n");
    printf("  USB Host Library v2.1.0\r\n\r\n");
}

//��⵽U�̲���
void USBH_USR_DeviceAttached(void)//U�̲���
{ 
    printf("��⵽USB�豸����!\r\n");
}

//��⵽U�̰γ�
void USBH_USR_DeviceDisconnected (void)//U���Ƴ�
{ 
    printf("USB�豸�γ�!\r\n");
    bDeviceState=0; //USB�豸�γ���
}

//��λ�ӻ�
void USBH_USR_ResetDevice(void)
{
    printf("��λ�豸...\r\n");
}

//��⵽�ӻ��ٶ�
//DeviceSpeed:�ӻ��ٶ�(0,1,2 / ����)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
    if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
    {
        printf("����(HS)USB�豸!\r\n");
    }  
    else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
    {
        printf("ȫ��(FS)USB�豸!\r\n"); 
    }
    else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
    {
        printf("����(LS)USB�豸!\r\n");  
    }
    else
    {
        printf("�豸����!\r\n");  
    }
}

//��⵽�ӻ���������
//DeviceDesc:�豸������ָ��
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

//�ӻ���ַ����ɹ�
void USBH_USR_DeviceAddressAssigned(void)
{
    printf("�ӻ���ַ����ɹ�!\r\n");   
}

//��������������Ч
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
//              printf("���ƶ��洢���豸!\r\n"); 
//          }else if((*id).bInterfaceClass==0x03)
//          {
//              printf("HID �豸!\r\n"); 
//          }
//          else if((*id).bInterfaceClass==0x0e)
//          {
//              printf("USB VIDEO �豸!\r\n"); 
//          }
//        else if((*id).bInterfaceClass==0x01)
//          {
//              printf("USB Audio �豸!\r\n"); 
//          }
//          else if((*id).bInterfaceClass==0xEF)
//          {
//              printf("Miscellaneous �豸!\r\n"); 
//          }   
//          else
//          {
//              printf("Unknown �豸!\r\n"); 
//          }
//      }

//  }
}
//��ȡ���豸Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
    printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//��ȡ���豸Product String 
void USBH_USR_Product_String(void *ProductString)
{
    printf("Product: %s\r\n",(char *)ProductString);  
}
//��ȡ���豸SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
    printf("Serial Number: %s\r\n",(char *)SerialNumString);    
} 
//�豸USBö�����
void USBH_USR_EnumerationDone(void)
{ 
    printf("�豸ö�����!\r\n\r\n");    
} 
//�޷�ʶ���USB�豸
void USBH_USR_DeviceNotSupported(void)
{ 
    printf("�޷�ʶ���USB�豸!\r\n\r\n");    
}  
//�ȴ��û����밴��,ִ����һ������
USBH_USR_Status USBH_USR_UserInput(void)
{ 
    //printf("�����û�ȷ�ϲ���!\r\n");
    bDeviceState=1;//USB�豸�Ѿ����ӳɹ�
    return USBH_USR_RESP_OK;
} 
//USB�ӿڵ�������
void USBH_USR_OverCurrentDetected (void)
{
    printf("�˿ڵ�������!!!\r\n");
}  
//���³�ʼ��
void USBH_USR_DeInit(void)
{
    printf("���³�ʼ��!!!\r\n");
}
//�޷��ָ��Ĵ���!!  
void USBH_USR_UnrecoveredError (void)
{
    printf("�޷��ָ��Ĵ���!!!\r\n\r\n");    
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
    MS2109_machine.buff[1] = MS2109_machine.MS2109_addr>>8; //��λ��ַ
    MS2109_machine.buff[2] = MS2109_machine.MS2109_addr; //��λ��ַ
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
    
    if(MS2109_machine.MS2109_valid == 1)  // ��Ч��MS2109 �豸������VID/PID�ж�
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
             MS2109_machine.MS2109_State = STATE_IDLE;   //usb �ָ�idle �����TVD REG д
        }
        break;
    case STATE_xdata_reg_write:
        if(MS2109_xdata_reg_write(pdev,  pphost)    == USBH_OK)
        {
             MS2109_machine.MS2109_State = STATE_IDLE;   //usb �ָ�idle �����XDATA REG д
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
            
                printf("audio L ����ֵ��%4d ,\t audio R ����ֵ��%4d \r\n",i,j);
            
                 if(MS2109_machine.audio_counter==5)  //  ����ǰ32 �β������Ե�33��Ϊ��ʼֵ
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


// data_type  = 1 TVD ���ݶ� data_type = 2�� xdata ���ݶ�


/**
  * @brief  MS2109_read_data
  *         ��ȡ MS2109 ������ ���߼Ĵ���
  * @param  data_type: 1 ��ȡTVD �Ĵ���  2 ����ȡXDATA����
  * @param  addr: Ҫ��ȡ�����ݵ�ַ��ʵ��TVD�����ݵ�ַֻ��Ҫ��λ
  * @retval ����ʵ�ʶ�ȡ��������
  */
uint8_t MS2109_read_data(uint8_t data_type,uint16_t addr)
{
    uint16_t i = 0;
      if(MS2109_machine.MS2109_valid == 1)  // ����VID �ж��Ƿ���Ч���豸
        {
            MS2109_machine.MS2109_addr = addr;
            MS2109_machine.MS2109_Step = 1;
            MS2109_machine.MS2109_State = data_type ; //1 ����һ�� TVD ���ݶ�  2 ����һ��XDATA���ݶ�    
            while(MS2109_machine.MS2109_State != 0) //�ȴ�������� ,ʵ��USB ���ݴ�����Timer3 �жϵ���USB���ݴ�����ɵ�  
            {
                 i++;
                if(i>15)
                {
                    MS2109_machine.MS2109_data  = 0xff;   // �����ȡ����ʱ�䳬ʱ������0xff ��Ч������
                    break;
                }
                Delay_ms(1);
            }
          //printf("MS2109_read_dataָ���ʱ��%4d ms\r\n",i);
            
          //return MS2109_machine.buff[0]; // ���ض�ȡ������ֵ  
          return MS2109_machine.MS2109_data;
        }

     return 0 ;
}

/**
  * @brief  MS2109_write_data
  *         ��ָ����ַдMS2109������
  * @param  data_type: 3  дTVD �Ĵ���  4 ��дXDATA����
  * @param  addr: Ҫд������ݵ�ַ��ʵ��TVD�����ݵ�ַֻ��Ҫ��λ
  * @param  value: Ҫд�������ֵ
  * @retval ���� ��
  */
void MS2109_write_data(uint8_t data_type,uint16_t addr,uint8_t value)
{
        uint16_t i = 0;
      if(MS2109_machine.MS2109_valid == 1)  //����VID �ж��Ƿ���Ч���豸
        {
            MS2109_machine.MS2109_State = data_type ; //3 ����һ�� TVD ���ݶ�  4 ����һ��XDATA���ݶ�
            MS2109_machine.MS2109_addr = addr;
            MS2109_machine.MS2109_data = value;             
            while(MS2109_machine.MS2109_State != 0) //�ȴ�������� ,ʵ��USB ���ݴ�����Timer3 �жϵ���USB���ݴ�����ɵ�  
            {
                 i++;
                if(i>10)
                {
                    break;
                }
                Delay_ms(1);
            }
//          printf("MS2109_write_dataָ���ʱ��%4d ms\r\n",i);
        }
}


/**
  * @brief  MS2109_write_data
  *         ��ָ����ַдMS2109������
  * @param  value: 1 ����Ƶͨ��   3  ����Ƶͨ�� 
  * @retval ���� ��
  */
void MS2109_setup(uint8_t value)
{
      uint16_t i;
      if(MS2109_machine.MS2109_valid == 1)  // 
        {
            MS2109_machine.MS2109_State = 5 ; // 
            MS2109_machine.MS2109_data = value;
            i = 0;
            while(MS2109_machine.MS2109_State != 0) //�ȴ�������� ,ʵ��USB ���ݴ�����Timer3 �жϵ���USB���ݴ�����ɵ�  
            {
                 i++;
                if(i>10)
                {
                    break;
                }
                Delay_ms(1);
            }
//          printf("setupָ���ʱ��%4d ms\r\n",i);
        }
}

/**
  * @brief  MS2109_audio_iso_receive
  *         �����˵�3 ISO ���ݴ��䣨ʵ���������жϴ��䣩
  * @param  ��
  * @retval ��
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
  *         �ڲ�Audio adc ����
  * @param  ��
  * @retval 0������ͨ���� 11  ����Fail
  */
uint8_t MS2109_int_audio_test(void)
{
    uint16_t i;
    MS2109_machine.audio_high = 0;
    MS2109_machine.audio_low = 0;
    MS2109_machine.audio_counter = 0;
    //MS2109_setup(3);   // �Ѿ���USB ��ʼ����ɺ���
    MS2109_audio_iso_receive(TRUE);
    i=0;
    while(MS2109_machine.audio_counter<10)  //  �ӵ�4��ISO ���俪ʼȡ���ݣ����ֵ-��Сֵ����800 ��Ϊ����Ч�ģ�
    {
         i++;
        if(i>120)
        {
            break;
        }
        Delay_ms(1);
    }
    printf("��Ƶ������ʱ��%4d ms\r\n",i);
    
    MS2109_audio_iso_receive(FALSE);
    printf("audio �ߵ�ƽADC ����ֵ��%4d ,\t ����ֵ��2520\r\n",MS2109_machine.audio_high);
    printf("audio �͵�ƽADC ����ֵ��%4d ,\t ����ֵ��1480\r\n",MS2109_machine.audio_low);
    if((MS2109_machine.audio_high>2400) && (MS2109_machine.audio_high<2650))  // �ߵ�ƽ����2400��С��2650
    {
        if((MS2109_machine.audio_low>1350) && (MS2109_machine.audio_low<1600))  // �͵�ƽ����1350��С��1600
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
