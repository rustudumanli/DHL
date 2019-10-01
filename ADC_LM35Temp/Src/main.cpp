#include "main.h"
#include <stdio.h>
#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "fonts.h"
#include "ssd1306.h"
#include "DHAL_Delay.h"
#include "OneWire.h"
#include "DHT11.h"

using namespace DHAL;

void SystemClock_Config(void);

uint32_t ADCValue;
double RefVol = 3.3;
double Temp;
char SBuffer[20];
OneWire OWire = OneWire(GPIOA, GPIO_PIN_1);
DHT11 Dht11Sensor = DHT11(GPIOA, GPIO_PIN_4);
uint8_t temp_l, temp_h;
uint16_t DS18B20_Temp;
float Temperature;
int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_ADC1_Init();

	MX_I2C1_Init();
	SSD1306_Init();
	
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_UpdateScreen();

	
	while (1)
	{
		//LM35 Sensör
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_ADC_ERROR_NONE)
		{
			ADCValue = HAL_ADC_GetValue(&hadc1);		  
		}
		HAL_ADC_Stop(&hadc1);
		Temp = ((((RefVol / 4095) * ADCValue) * 1000) / 10);
		sprintf(SBuffer, "TEMP : %4.2f", Temp);
		SSD1306_GotoXY(0, 0);
		SSD1306_Puts(SBuffer, &Font_11x18, SSD1306_COLOR_WHITE);
		SSD1306_UpdateScreen();
		
		//DS18B20 Sensör
		OWire.OneWire_Load();
		OWire.OneWire_Write(0xCC);
		OWire.OneWire_Write(0x44);
		DelayMs(800);
		
		OWire.OneWire_Load();
		OWire.OneWire_Write(0xCC);
		OWire.OneWire_Write(0xBE);
		
		temp_l = OWire.OneWire_Read();
		temp_h = OWire.OneWire_Read();
		DS18B20_Temp = (temp_h << 8) | temp_l;
		Temperature = (float)DS18B20_Temp / 16;
		sprintf(SBuffer, "TEMP : %4.2f", Temperature);
		SSD1306_GotoXY(0, 19);
		SSD1306_Puts(SBuffer, &Font_11x18, SSD1306_COLOR_WHITE);
		SSD1306_UpdateScreen();
		
		//DHT11 Sensör
		Dht11Sensor.DHT11_GetVale();
		sprintf(SBuffer, "TEMP : %d.%d", Dht11Sensor.Temp, Dht11Sensor.PercentTemp);
		SSD1306_GotoXY(0, 37);
		SSD1306_Puts(SBuffer, &Font_11x18, SSD1306_COLOR_WHITE);
		SSD1306_UpdateScreen();
		DelayMs(2000);
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
