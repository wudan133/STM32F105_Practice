#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED1_GPIO_Port      GPIOC                       /* GPIO端口 */
#define LED1_GPIO_CLK       RCC_APB2Periph_GPIOC        /* GPIO端口时钟 */
#define LED1_Pin            GPIO_Pin_0                  /* 连接到SCL时钟线的GPIO */

/** the ro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a) if (a)  \
                    GPIO_SetBits(LED1_GPIO_Port,LED1_Pin);\
                    else    \
                    GPIO_ResetBits(LED1_GPIO_Port,LED1_Pin)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)          {p->BSRR=i;}            //设置为高电平
#define digitalLo(p,i)          {p->BRR=i;}             //输出低电平
#define digitalToggle(p,i)      {p->ODR ^=i;}           //输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE     digitalToggle(LED1_GPIO_Port,LED1_Pin)
#define LED1_OFF        digitalHi(LED1_GPIO_Port,LED1_Pin)
#define LED1_ON         digitalLo(LED1_GPIO_Port,LED1_Pin)



void LED_GPIO_Config(void);

#endif /* __LED_H */
