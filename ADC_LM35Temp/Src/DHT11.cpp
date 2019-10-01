#include "DHT11.h"
#include "DHAL_Delay.h"
#include "stdbool.h"


void DHAL::DHT11::DHT11_Load(void)
{
	PinOutput();
	HAL_GPIO_WritePin(_Port, _Pin, GPIO_PIN_RESET);
	DelayMs(18);
	PinInput();
}

void DHAL::DHT11::DHT11_Check(void)
{
	DelayUs(40);
	if (!(HAL_GPIO_ReadPin(_Port, _Pin)))
	{
		DelayUs(80);
	}
	while ((HAL_GPIO_ReadPin(_Port, _Pin)));
}

uint8_t DHAL::DHT11::DHT11_Read(void)
{
	uint8_t Data, Count;
	for (Count = 0; Count < 8; Count++)
	{
		while (HAL_GPIO_ReadPin(_Port, _Pin) == GPIO_PIN_RESET);
		DelayUs(40);
		if(HAL_GPIO_ReadPin(_Port, _Pin) == GPIO_PIN_RESET)
		{
			Data &= ~(1 << (7 - Count));
		}
		else Data |= (1 << (7 - Count));
		while (HAL_GPIO_ReadPin(_Port, _Pin) == GPIO_PIN_SET) ;
	}
	return Data;
}

bool DHAL::DHT11::DHT11_GetVale(void)
{
	DHT11_Load();
	DHT11_Check();
	Humidity = DHT11_Read();
	PercentHumidity = DHT11_Read();
	Temp = DHT11_Read();
	PercentTemp = DHT11_Read();
	Parity = DHT11_Read();
	
	if (Parity == (Humidity + PercentHumidity + Temp + PercentTemp)) return true;
	return false;
}

DHAL::DHT11::DHT11(GPIO_TypeDef *Port, uint16_t Pin)
{
	_Port = Port;
	_Pin = Pin;
}

void DHAL::DHT11::PinInput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = _Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(_Port, &GPIO_InitStruct);
}

void DHAL::DHT11::PinOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = _Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_Port, &GPIO_InitStruct);
}