#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define LED1_GPIO_Port      GPIOC                       /* GPIO�˿� */
#define LED1_GPIO_CLK       RCC_APB2Periph_GPIOC        /* GPIO�˿�ʱ�� */
#define LED1_Pin            GPIO_Pin_0                  /* ���ӵ�SCLʱ���ߵ�GPIO */

/** the ro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a) if (a)  \
                    GPIO_SetBits(LED1_GPIO_Port,LED1_Pin);\
                    else    \
                    GPIO_ResetBits(LED1_GPIO_Port,LED1_Pin)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)          {p->BSRR=i;}            //����Ϊ�ߵ�ƽ
#define digitalLo(p,i)          {p->BRR=i;}             //����͵�ƽ
#define digitalToggle(p,i)      {p->ODR ^=i;}           //�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE     digitalToggle(LED1_GPIO_Port,LED1_Pin)
#define LED1_OFF        digitalHi(LED1_GPIO_Port,LED1_Pin)
#define LED1_ON         digitalLo(LED1_GPIO_Port,LED1_Pin)



void LED_GPIO_Config(void);

#endif /* __LED_H */
