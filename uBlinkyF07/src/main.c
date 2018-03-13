/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include <stdint.h>

//Private Constants
#define APPLICATION_ADDRESS     (uint32_t)0x08002000

__IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));


//volatile uint32_t *MODER = 0x40020400;
//volatile uint32_t *ODR = 0x40020414;
//volatile uint32_t *RCCAHB = 0x40023830;
volatile uint32_t *MODER = &(GPIOB->MODER);
volatile uint32_t *ODR = &(GPIOB->ODR);
volatile uint32_t *RCCAHB = &(RCC->APB1ENR);
volatile uint32_t *IWDG_KR = &(IWDG->KR);
volatile uint32_t *IWDG_RLR = &(IWDG->RLR);
volatile uint32_t *WWDG_CR = &(WWDG->CR);
uint32_t counter = 0;
int main(void)
{
	  uint32_t i = 0;
		__disable_irq();
	  /*!< At this stage the microcontroller clock setting is already configured,
	       this is done through SystemInit() function which is called from startup
	       file (startup_stm32f0xx.s) before to branch to application main.
	       To reconfigure the default setting of SystemInit() function, refer to
	       system_stm32f0xx.c file
	     */

	/* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/

	  /* Copy the vector table from the Flash (mapped at the base of the application
	     load address 0x08003000) to the base address of the SRAM at 0x20000000. */
	for(i = 0; i < 48; i++)
	{
		 if((*WWDG_CR < 0xD0) && (*WWDG_CR & 0x80))
			*WWDG_CR = 0x000000ff;
		 if(*IWDG_RLR)
			*IWDG_KR = IWDG_KEY_RELOAD;
	    VectorTable[i] = *(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
	}
//	for(i = 0;32;i++)
//	{
//		 if((*WWDG_CR < 0xD0) && (*WWDG_CR & 0x80))
//			*WWDG_CR = 0x000000ff;
//		 if(*IWDG_RLR)
//			*IWDG_KR = IWDG_KEY_RELOAD;
//		 NVIC_DisableIRQ(i);
//	}
	  /* Enable the SYSCFG peripheral clock*/
	__HAL_RCC_APB2_FORCE_RESET();
	  /* Remap SRAM at 0x00000000 */
	__HAL_SYSCFG_REMAPMEMORY_SRAM();

	*RCCAHB = 1<<1;

	*MODER = 1 << 2 * 8 | 1<< 2*9 | 1<<2*15;
//	*ODR = 1 << 8 | 1<< 15;
	*ODR = 0 << 9 | 1 <<8 | 1<<15;
	//		BLUE	GREEN	RED
	for(;;)
	{
		 for(counter=0;counter<1024000/20;counter++)
		 {
			 if((*WWDG_CR < 0xD0) && (*WWDG_CR & 0x80))
				*WWDG_CR = 0x000000ff;
			 if(*IWDG_RLR)
				*IWDG_KR = IWDG_KEY_RELOAD;
		 }
			*ODR = 1 << 9 | 0 <<8 | 1<<15;
		 for(counter=0;counter<1024000/20;counter++)
		 {
			 if((*WWDG_CR < 0xD0) && (*WWDG_CR & 0x80))
				*WWDG_CR = 0x000000ff;
			 if(*IWDG_RLR)
				*IWDG_KR = IWDG_KEY_RELOAD;
		 }
			*ODR = 1 << 9 | 1 <<8 | 0<<15;
			 for(counter=0;counter<1024000/20;counter++)
			 {
				 if((*WWDG_CR < 0xD0) && (*WWDG_CR & 0x80))
					*WWDG_CR = 0x000000ff;
				 if(*IWDG_RLR)
					*IWDG_KR = IWDG_KEY_RELOAD;
			 }
				*ODR = 0 << 9 | 1 <<8 | 1<<15;
	}
}
