/**
******************************************************************************
* @file    TiMbase.c
* @author  dwu
* @version V1.0.0
* @date    29-Nov-2017
* @brief   TiMbase driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "TiMbase.h"


u16 g_u16_sys_timer = 0;
u16 g_u16_key_timer = 0;

void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    // 开启TIMx_CLK,x[2,3,4,5],即内部时钟CK_INT=48M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);

    // 自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Period=10000-1;

    // 累计 TIM_Period个频率后产生一个更新或者中断
    // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (47+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 72-1;

    // 时钟分频因子 ，没有用到，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 

    // 重复计数器的值，通用定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

    // 初始化定时器TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    // 清除计数器中断标志位
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);

    // 开启计数器中断
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);

    // 使能计数器
    TIM_Cmd(TIM6, ENABLE);

    // 暂时关闭TIMx,x[2,3,4,5]的时钟，等待使用
    //RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, DISABLE);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<1; //TIM3时钟使能
    TIM3->ARR=arr;      //设定计数器自动重装值 
    TIM3->PSC=psc;      //预分频器设置
    TIM3->DIER|=1<<0;   //允许更新中断
    TIM3->CR1|=0x01;    //使能定时器3
    //MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2
}
/*********************************************END OF FILE**********************/
