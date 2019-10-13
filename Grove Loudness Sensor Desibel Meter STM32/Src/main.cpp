#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "gpio.h"
#include "DHAL_Delay.h"
#include "fonts.h"
#include "ssd1306.h"
#include <stdio.h>
#include "math.h"

#define ref_SPL			 94 
#define sensitivity    3.3
uint32_t ADCValue = 0;
char SBuffer[20];
float dB_max = 0.0;
float dB_min = 100.0;
float dB_avg = 0.0;             
float dB_current = 0.0;   
void SystemClock_Config(void);
void ADCGetValue()
{
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
		ADCValue = HAL_ADC_GetValue(&hadc1);
	}
	HAL_ADC_Stop(&hadc1);
}

//double map(double value, double in_min, double in_max, double out_min, double out_max) {
//	return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
//}

float GetRMS()
{
	uint16_t samples = 16;
	long tmp = 0;
	float rms = 0.0;          
                      
	while (samples > 0)
	{
		ADCGetValue();
		rms = ADCValue;  
		tmp += (rms * rms);      
		samples--;      
	}                     
	tmp >>= 4;                           
	rms = (sqrt(tmp));     
	rms *= 0.004875; 
    
	if (rms <= 0)
	{
		rms = 0.004875;
	}
    
	return rms;
}

void GetDB() 
{
	dB_current = GetRMS();
	dB_current = (ref_SPL + 20 * log10(dB_current / sensitivity)); 
	if (dB_current <= 40)    {        dB_current = 40; }    if (dB_current >= 99)
	{
		dB_current = 99;
	}

	
	if (dB_current > dB_max)      
	{     
		dB_max = dB_current;  
	}              
	if (dB_current < dB_min)      
	{                    
		dB_min = dB_current;
	}                   
	dB_avg = ((dB_max + dB_min) * 0.5);   
} 

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_I2C1_Init();
	DelayInit();
	SSD1306_Init();
	
	SSD1306_Clear();
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_UpdateScreen();
	
	while (1)
	{
		
		GetDB();
		ADCGetValue();
		sprintf(SBuffer, "DB : %4.0f", dB_current);
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts(SBuffer, &Font_11x18, SSD1306_COLOR_WHITE);
		SSD1306_UpdateScreen();
		
		sprintf(SBuffer, "Mn:%2.0f Mx:%2.0f Ag:%2.0f", dB_min, dB_max, dB_avg);
		SSD1306_GotoXY(0, 19);
		SSD1306_Puts(SBuffer, &Font_11x18, SSD1306_COLOR_WHITE);
		SSD1306_UpdateScreen();
		
				
		sprintf(SBuffer, "AVG:%2.0f", dB_avg);
		SSD1306_GotoXY(0, 38);
		SSD1306_Puts(SBuffer, &Font_11x18, SSD1306_COLOR_WHITE);
		SSD1306_UpdateScreen();
		
		DelayMs(400);

	}
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
