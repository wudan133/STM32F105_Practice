/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0
  * @date    2018-07-xx
  * @brief   
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "SysTick.h"
#include "Led.h"
#include "usart.h"
#include "TiMbase.h"
#include "Key.h"
#include "usb_core.h"
#include "usbh_usr.h"

#define SLT_FIRMWARE_VERSION ("STM32F105RBT6_BaseSample_V1.0.0_20191012\r\n")

USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;

USBH_HOST  USB_Host;

static void NVIC_Configuration(void);
static void system_init(void);
static void key_service(void);

u8 step4_usb_test(void);
u8 step14_audio_in_test(void);

/**
  * @brief  
  * @param  
  * @retval 
  */
int main(void)
{
    system_init();
    
    step4_usb_test();
    step14_audio_in_test();
    
    while(1)
    {
        if (g_u16_sys_timer >= SYS_TIMEOUT_500MS)
        {
            LED1_TOGGLE;
            g_u16_sys_timer = 0; 
        }
        
        if (g_u16_key_timer >= 2)
        {
            key_service();
            g_u16_key_timer = 0;
        }
    }
}

//help
/***************************************************************
* Function name:  NVIC_Configuration()
* Description:    
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    /* Configure the NVIC Preemption Priority Bits */  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void system_init(void)
{
    SysTick_Init(72);
    LED_GPIO_Config();
    USARTx_Config();
    TIM6_Configuration();
    
    Key_GPIO_Config();
    
    printf(SLT_FIRMWARE_VERSION);
    Delay_ms(1);
    
    //初始化USB主机
    USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&MS2109_cb,&USR_Callbacks);
    TIM3_Int_Init(3,7200);  //72M/7200 = 10K = 0.1ms   USB 相关的数据处理全部在Timer3中断中完成，每0.4ms调用一次  
    
    NVIC_Configuration();
    Delay_ms(400);          // 等待USB 枚举完成 
}

static void key_service(void)
{
    uint8_t u8key = Key_Detect();

    switch (u8key)
    {
    case KEY_MAP_SHORT0:
        printf("Key 1\r\n");
        
        break;
    
    case KEY_MAP_SHORT1:
        printf("Key 2\r\n");
        break;
    default:
        break;
    }
}

// 利用定时器每 ms 调用一次USB 数据处理
void USB_Process(void)
{
    USBH_Process(&USB_OTG_Core_dev, &USB_Host);
}

u8 step4_usb_test(void)  // 2s内USB 枚举不通过，报错4
{
    u16 time_limit_counter = 0;

    while(time_limit_counter < 200)  //200ms  实际已经完成 USB 枚举
    {
        if(bDeviceState == 1) // 设备已经连接
        {
            if(HID_ReadByte(0XF800) == 0xA7)  //F000~F003 =  A7109A 
            {
                printf("USB Test Pass!\r\n");
                return 0;
            }
            else
            {
                printf("USB Test Fail!\r\n");
                return 1;  //USB fail
            }
        }
        time_limit_counter++;
        Delay_ms(10);
    }
    printf("USB 2s内USB 枚举不通过!\r\n");
    return 1; 
}

u8 step14_audio_in_test(void)  // 报错14
{
    u16 time_limit_counter = 0;

    while(time_limit_counter < 200)  //200ms  实际已经完成 USB 枚举
    {
        if(bDeviceState == 1) // 设备已经连接
        {
            if(MS2109_int_audio_test() == 0)
            {
                printf("USB audio Pass!\r\n");
                return 0;
            }
            else
            {
                printf("USB audio Fail!\r\n");
                return 1;  //USB fail
            }
        }
        time_limit_counter++;
        Delay_ms(10);
    }
    printf("USB 2s内USB 枚举不通过!\r\n");
    
    return 0; 
}
/***********************************END OF FILE********************************/
