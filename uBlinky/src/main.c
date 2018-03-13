/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f4xx_hal.h"
#include <stdint.h>

//#include "stm32f4xx.h"
//#include "stm32f4xx_nucleo_144.h"
volatile uint32_t *MODER = 0x40020400;
volatile uint32_t *ODR = 0x40020414;
volatile uint32_t *RCCAHB = 0x40023830;
volatile uint32_t *IWDG_KEY = 0x40003000;
volatile uint32_t *IWDG_RLR = 0x40003008;
volatile uint32_t *WWDG_CR = 0x40002C00;
//volatile GPIO_TypeDef *LED = GPIOB;
//uint32_t * LED = GPIOB;
uint32_t counter = 0;
int main(void)
{
	*RCCAHB = 1<<1;
//	uint32_t add = &(RCC->AHB1ENR);
//	volatile uint32_t tempreg = RCC->AHB1ENR;
//#define __HAL_RCC_GPIOB_CLK_ENABLE()   do { \
//                                        __IO uint32_t tmpreg = 0x00U; \
//                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);\
//                                        /* Delay after an RCC peripheral clock enabling */ \
//                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);\
//                                        UNUSED(tmpreg); \
//                                          } while(0U)
//	__HAL_RCC_GPIOB_CLK_ENABLE();
	*MODER = 1 << 2 * 7;
	*ODR = 1 << 7;
//	LED->MODER = 1 << 2 * 7; //PB7
//	LED->ODR = 1 << 7;
	for(;;)
	{
		 for(counter=0;counter<1024000/4;counter++)
		 {
			 if((*WWDG_CR < 0xD0) && (*WWDG_CR & 0x80))
				*WWDG_CR = 0x000000ff;
			 if(*IWDG_RLR)
				*IWDG_KEY = 0x0000AAAAU;
		 }
			*ODR = 1 << 7;
		 for(counter=0;counter<1024000/4;counter++)
		 {
			 if((*WWDG_CR < 0xD0) && (*WWDG_CR & 0x80))
				*WWDG_CR = 0x000000ff;
			 if(*IWDG_RLR)
				*IWDG_KEY = 0x0000AAAAU;
		 }
			*ODR = 0 << 7;
	}
}
