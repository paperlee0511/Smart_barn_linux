/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rgb.h"
#include "DHT11.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "i2c_lcd.h"
#include "delay.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#ifdef __GNUC__
/* With GCC small printf (option LD Linker->Libraries->Small printf
 * set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int  __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int  fputc(int ch, FILE *f)
#endif /* __GNUC__*/

/** @brief Retargets the C library printf function to the USART.
 *  @param None
 *  @retval None
 */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop
     until the end of transmission */
  if(ch == '\n')


    HAL_UART_Transmit(&huart6, (uint8_t*) "\r", 1, 0xFFFF);
    HAL_UART_Transmit(&huart2, (uint8_t*) "\r", 1, 0xFFFF);


  HAL_UART_Transmit(&huart6, (uint8_t*) &ch, 1, 0xFFFF);
  HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, 0xFFFF);
  return ch;
}




/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern TIM_HandleTypeDef htim10;  // TIM10 핸들
extern TIM_HandleTypeDef htim11;  // TIM11 핸들
extern TIM_HandleTypeDef htim3;		// TIM3 핸들

DHT10 dht_in;
DHT11 dht_out;
char msg[100];
char lcd_line1[16];
char lcd_line2[16];


uint32_t last_sensor_time = 0;

uint8_t current_pattern[8];
uint8_t use_fan_animation = 0;
uint8_t use_flame_animation =0;
uint8_t use_smile_animation =0;
uint8_t use_humidification_animation =0;
uint8_t use_dehumidification_animation =0;
uint8_t use_fan_and_humidification_animation =0;
uint8_t use_flame_and_dehumidification_animation =0;
uint8_t use_fan_and_dehumidification_animation =0;
uint8_t use_flame_and_humidification_animation =0;
uint8_t reset_all_animations =0;




extern uint8_t smile[8];
extern uint8_t sad[8];
extern uint8_t blank[8];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */



  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM11_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM10_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  dht10Init(&dht_in, GPIOD, GPIO_PIN_2, &htim10);
  dht11Init(&dht_out, GPIOC, GPIO_PIN_4, &htim11);

  i2c_lcd_init();

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  TIM3 ->CCR1 = 0;

  void reset_all_animations(void)
  {
    use_fan_animation = 0;
    use_flame_animation = 0;
    use_smile_animation = 0;
    use_humidification_animation = 0;
    use_dehumidification_animation = 0;
    use_fan_and_humidification_animation = 0;
    use_flame_and_dehumidification_animation = 0;
    use_fan_and_dehumidification_animation = 0;
    use_flame_and_humidification_animation = 0;
  };


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
	  if (HAL_GetTick() - last_sensor_time >= 2000)
	  {
		  if (dht10Read(&dht_in))
		  {
			  sprintf(msg, "IN: %d,%d\n", dht_in.temperature_in, dht_in.humidity_in);
			  HAL_UART_Transmit(&huart6, (uint8_t *)msg, strlen(msg), 1000);
			  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);
			  sprintf(lcd_line1, "IN =T:%dC H:%d%%", dht_in.temperature_in, dht_in.humidity_in);

			  if(dht_in.humidity_in > 40 && dht_in.humidity_in <= 55 && dht_in.temperature_in >= 25)
			  {
				  TIM3->CCR1 = 99;
				  LED_R_OFF(); LED_G_OFF(); LED_B_ON();

				    reset_all_animations();
				    use_fan_animation = 1;

				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			  }

			  else if (dht_in.humidity_in > 55 && dht_in.temperature_in >= 25)
			  {
				  TIM3->CCR1 = 99;
				  LED_R_OFF(); LED_G_ON(); LED_B_ON();
				  reset_all_animations();
				  use_fan_and_dehumidification_animation = 1;
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			  }

			  else if (dht_in.humidity_in > 55 && dht_in.temperature_in > 20 &&dht_in.temperature_in < 25)
			  {
				  TIM3->CCR1 = 0;
				  LED_R_OFF(); LED_G_ON(); LED_B_OFF();
				    reset_all_animations();
				  use_dehumidification_animation =1;
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			  }
			  // 히터 가습기
			  else if(dht_in.humidity_in <= 40 && dht_in.temperature_in <= 20)
			  {
				  TIM3->CCR1 = 0;
				  LED_R_ON(); LED_G_OFF(); LED_B_OFF();
				    reset_all_animations();
				  use_flame_and_humidification_animation =1;
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			  }

			  else if (dht_in.humidity_in >40 && dht_in.humidity_in <= 55 && dht_in.temperature_in < 25 && dht_in.temperature_in > 20 )
			  {
				  TIM3->CCR1 = 0;
				  LED_R_ON(); LED_G_ON(); LED_B_ON();
				    reset_all_animations();
  				  use_smile_animation = 1;
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);

			  }

			  else if (dht_in.humidity_in <= 40 && dht_in.temperature_in < 25 && dht_in.temperature_in > 20 )
			  {
				  TIM3->CCR1 = 0;
				  LED_R_OFF(); LED_G_OFF(); LED_B_OFF();
				    reset_all_animations();
  				  use_humidification_animation=1;
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

			  }

			  else if ( dht_in.humidity_in < 55 &&dht_in.humidity_in >= 40 && dht_in.temperature_in <= 20)
			  {
				  TIM3->CCR1 = 0;
				  LED_R_ON(); LED_G_OFF(); LED_B_OFF();
				  reset_all_animations();
				  use_flame_animation = 1;
				  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);

			  }


			  else if (dht_in.humidity_in < 40 && dht_in.temperature_in >= 25)
			  {
			      TIM3->CCR1 = 99;
			      LED_R_OFF(); LED_G_OFF(); LED_B_ON();
			      reset_all_animations();
  			    use_fan_and_humidification_animation = 1;
			      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			  }
			  else if (dht_in.humidity_in > 55 && dht_in.temperature_in <= 20)
			  {

			      TIM3->CCR1 = 0;
			      LED_R_ON(); LED_G_ON(); LED_B_OFF();
			      reset_all_animations();
			      use_flame_and_dehumidification_animation=1;
			      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			  }


		  }
		  else
		  {
			   HAL_UART_Transmit(&huart6, (uint8_t *)"IN_ERR\n", 7, 1000);
			   sprintf(lcd_line1, "IN=ERROR");
			   reset_all_animations();
			   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		  }

		  if (dht11Read(&dht_out))
		  {
			  sprintf(msg, "OUT: %d,%d\n", dht_out.temperature_out, dht_out.humidity_out);
			  HAL_UART_Transmit(&huart6, (uint8_t *)msg, strlen(msg), 1000);
			  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);
			  sprintf(lcd_line2, "OUT=T:%dC H:%d%%", dht_out.temperature_out, dht_out.humidity_out);
		  }
		  else
		  {
			  HAL_UART_Transmit(&huart6, (uint8_t *)"OUT_ERR\n", 8, 1000);
			  sprintf(lcd_line2, "OUT=ERROR");
		  }

		  move_cursor(0, 0); lcd_string(lcd_line1);
		  move_cursor(1, 0); lcd_string(lcd_line2);

		  last_sensor_time = HAL_GetTick();
	  }



	  if (use_fan_animation)
	  {
		  animate_fan_pattern_rowwise();
	  }


	  else if(use_flame_animation)
	  {
		  animate_flame_pattern_rowwise();
	  }

	  else if(use_humidification_animation)
	  {
		  animate_humidification_pattern_rowwise();
	  }

	  else if(use_dehumidification_animation)
	  {
		  animate_dehumidification_pattern_rowwise();
	  }

	  else if(use_smile_animation)
	  {
	  	  animate_smile_pattern_rowwise();
	  }

	  else if(use_fan_and_humidification_animation)
	  {
		  animate_fan_and_humidification_pattern_rowwise();
	  }

	  else if(use_flame_and_dehumidification_animation)
	  {
		  animate_flame_and_dehumidification_pattern_rowwise();
	  }

	  else if(use_fan_and_dehumidification_animation)
	  {
		  animate_fan_and_dehumidification_pattern_rowwise();
	  }

	  else if(use_flame_and_humidification_animation)
	  {
		  void animate_flame_and_humidification_pattern_rowwise();
	  }
	  else
	  {
		  display_pattern_rowwise(current_pattern);
	  }

	 // HAL_Delay(1);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
