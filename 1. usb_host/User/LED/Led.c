/**
  ******************************************************************************
  * @file    Led.c
  * @author  Dan Wu
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   
  *            
  ******************************************************************************
  */
#include "Led.h"


void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( LED1_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = LED1_Pin;
    GPIO_Init(LED1_GPIO_Port, &GPIO_InitStructure);
    
    GPIO_SetBits(LED1_GPIO_Port, LED1_Pin);
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
