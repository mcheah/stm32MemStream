/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo_144.h"
#include "main.h"
//Private variables
pFunction Jump_To_Application;
uint32_t JumpAddress;

IWDG_HandleTypeDef IwdgHandle;
static WWDG_HandleTypeDef   WwdgHandle;

static __IO uint32_t uwLsiFreq = 0;
uint32_t counter;
volatile uint32_t *IWDG_KEY = 0x40003000;
volatile uint32_t *IWDG_RLR = 0x40003008;
volatile uint32_t *WWDG_CR = 0x40002C00;

static inline void delay(uint8_t refresh)
{
	 for(counter=0;counter<1024000/4;counter++)
	 {
		 if(*WWDG_CR == 0xC0 && *WWDG_CR & 0x80 && refresh)
			 *WWDG_CR = 0x000000ff;
//			 HAL_WWDG_Refresh(&WwdgHandle);
		 if(*IWDG_RLR)
			*IWDG_KEY = 0x0000AAAAU;
//	 for(counter=0;counter<1024000;counter++)
//		*IWDG_KEY = 0x0000AAAAU;
	 }
}

int main(void)
{
	BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
	BSP_LED_Off(LED1);
	BSP_LED_Off(LED2);
	BSP_LED_Off(LED3);
	delay(0);
	delay(0);
	delay(0);
	delay(0);
	BSP_LED_On(LED1);
	BSP_LED_On(LED2);
	BSP_LED_On(LED3);
	/*##-2- Get the LSI frequency: TIM5 is used to measure the LSI frequency ###*/
//	uwLsiFreq = GetLSIFrequency();

	/*##-3- Configure the IWDG peripheral ######################################*/
	/* Set counter reload value to obtain 250ms IWDG TimeOut.
	 IWDG counter clock Frequency = LsiFreq / 32
	 Counter Reload Value = 250ms / IWDG counter clock period
						  = 0.25s / (32/LsiFreq)
						  = LsiFreq / (32 * 4)
						  = LsiFreq / 128 */
	IwdgHandle.Instance       = IWDG;
	IwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
	IwdgHandle.Init.Reload    = 1<<12-1;
	if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
	{
	/* Initialization Error */
//		Error_Handler();
		__NOP;
	}
	/*##-2- Configure the WWDG peripheral ######################################*/
	  /* WWDG clock counter = (PCLK1 (16MHz)/4096)/8) = 976.5625 Hz (~1024 us)
	     WWDG Window value = 200 means that the WWDG counter should be refreshed only
	     when the counter is below 200 (and greater than 64/0x40) otherwise a reset will
	     be generated.
	     WWDG Counter value = 2000, WWDG timeout = ~1024 us * 2000 = 2000 ms */
	  WwdgHandle.Instance       = WWDG;
	  WwdgHandle.Init.Prescaler = WWDG_PRESCALER_8;
	  WwdgHandle.Init.Window    = 127;
	  WwdgHandle.Init.Counter   = 127;
//	  __HAL_RCC_WWDG_CLK_ENABLE();
//	  if (HAL_WWDG_Init(&WwdgHandle) != HAL_OK)
//	  {
//	    /* Initialization Error */
//		  __NOP;
////	    Error_Handler();
//	  }
//	  __HAL_WWDG_ENABLE(&WwdgHandle);
//	volatile uint32_t *RCC_CSR = 0x40023874;
//	*RCC_CSR &= ~1; //Disable LSI
//	IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;
//	IwdgHandle.Init.Reload = 0;
//	HAL_IWDG_Init(&IwdgHandle);
//	HAL_IWDG_Refresh(&IwdgHandle);
//	LL_RCC_LSI_Disable();
//	HAL_IWDG_Refresh(&IwdgHandle);
	while(BSP_PB_GetState(BUTTON_KEY)==GPIO_PIN_RESET)
	{
		delay(1);
//			printf("Button Pressed\r\n");
		BSP_LED_Toggle(LED3);
	}

	JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    /* Jump to user application */
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
    Jump_To_Application();

	for(;;);
}
