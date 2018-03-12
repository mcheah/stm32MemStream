/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "main.h"
//Private variables
UART_HandleTypeDef UartHandle;
//Private Functions
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
static void SystemClock_Config(void);
static void Error_Handler(void);
static void printAllMemory(void);
volatile const char secretPass[] = "THIS IS SECRET TEST DATA THAT YOU SHOULDNT BE ABLE TO SEE";

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
	BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);
	  /*##-1- Configure the UART peripheral ######################################*/
	  /* Put the UART peripheral in the Asynchronous mode (UART Mode) */
	  /* UART configured as follows:
	      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
		                  BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
	      - Stop Bit    = One Stop bit
	      - Parity      = ODD parity
	      - BaudRate    = 9600 baud
	      - Hardware flow control disabled (RTS and CTS signals) */
	  UartHandle.Instance        = UARTx;
	  UartHandle.Init.BaudRate   = 115200;
	  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	  UartHandle.Init.StopBits   = UART_STOPBITS_1;
	  UartHandle.Init.Parity     = UART_PARITY_NONE;
	  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  UartHandle.Init.Mode       = UART_MODE_TX_RX;
	  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&UartHandle) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }
	  /* Output a message on Hyperterminal using printf function */
	  printf("\n\r UART Printf Example: retarget the C library printf function to the UART\n\r");
	  printf("** Test finished successfully. ** \n\r");

	for(;;)
	{
		if(BSP_PB_GetState(BUTTON_KEY)==GPIO_PIN_RESET)
		{
//			printf("Button Pressed\r\n");
			HAL_Delay(500);
			BSP_LED_Toggle(LED2);
		}
		else
		{
			printAllMemory();
		}
	}
}

/**
  * @brief  Retargets the C library printf function to the UART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the UART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 12
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* No HSE Oscillator on Nucleo, Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED2 on */
  BSP_LED_On(LED1);
  while (1)
  {
  }
}
const uint32_t *startAdd = 0x8000000;
uint8_t *buffAdd = 0;
static void printAllMemory(void)
{
	uint32_t bytesToPrint = 128*1024;
	uint32_t buffSize = 1024;
	buffAdd = (uint8_t *) startAdd;
	printf("printing Memory contents starting at %X",startAdd);
//	  while(1)
//	  {
//		  printf("fuck you\r\n");
//		  printf("fuckyou %d\r\n",2);
//		  HAL_Delay(2500);
//	  }
//	printf("fuckme\r\n");
//	while(buffAdd - startAdd < bytesToPrint)
//	{
//		BSP_LED_Toggle(LED1);
//		HAL_UART_Transmit(&UartHandle,(uint8_t *)buffAdd,buffSize,10000);
//		buffAdd += buffSize;
//	}
	while((uint32_t) buffAdd - (uint32_t) startAdd < bytesToPrint)
	{
//		BSP_LED_Toggle(LED1);
//		printf("%02X",*((uint8_t *)buffAdd));
//		HAL_UART_Transmit(&UartHandle,(uint8_t *)buffAdd,buffSize,10000);
		HAL_UART_Transmit(&UartHandle,(uint8_t *)buffAdd,1,10000);
		buffAdd += 1;
	}
}

