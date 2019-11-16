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

    // ����TIMx_CLK,x[2,3,4,5],���ڲ�ʱ��CK_INT=48M
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);

    // �Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Period=10000-1;

    // �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж�
    // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (47+1)=1M
    TIM_TimeBaseStructure.TIM_Prescaler= 72-1;

    // ʱ�ӷ�Ƶ���� ��û���õ������ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 

    // �ظ���������ֵ��ͨ�ö�ʱ��û�У����ù�
    //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

    // ��ʼ����ʱ��TIMx, x[2,3,4,5]
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
    TIM_Cmd(TIM6, ENABLE);

    // ��ʱ�ر�TIMx,x[2,3,4,5]��ʱ�ӣ��ȴ�ʹ��
    //RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, DISABLE);
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<1; //TIM3ʱ��ʹ��
    TIM3->ARR=arr;      //�趨�������Զ���װֵ 
    TIM3->PSC=psc;      //Ԥ��Ƶ������
    TIM3->DIER|=1<<0;   //��������ж�
    TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
    //MY_NVIC_Init(1,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2
}
/*********************************************END OF FILE**********************/
