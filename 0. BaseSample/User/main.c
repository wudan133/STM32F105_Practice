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

#define SLT_FIRMWARE_VERSION ("STM32F105RBT6_BaseSample_V1.0.0_20191012\r\n")


static void NVIC_Configuration(void);
static void system_init(void);
static void key_service(void);

u8 step4_usb_test(void);

/**
  * @brief  
  * @param  
  * @retval 
  */
int main(void)
{
    system_init();
    
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
    NVIC_Configuration();
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
/***********************************END OF FILE********************************/
