#include "main.h"
#include "OneWire.h"
#include "DHAL_Delay.h"

uint8_t DHAL::OneWire::OneWire_Load(void)
{
	PinOutput();      
	HAL_GPIO_WritePin(_Port, _Pin, GPIO_PIN_RESET);    
	DelayUs(480);       

	PinInput();       
	DelayUs(80);      

	if(!(HAL_GPIO_ReadPin(_Port, _Pin)))
	{
		DelayUs(400);
		return 0;
	}

	else
	{
		DelayUs(400);
		return 1;
	}
}
void DHAL::OneWire::OneWire_Write(uint8_t data)
{
	PinOutput();   

	for(int i = 0 ; i < 8 ; i++)
	{

		if ((data & (1 << i)) != 0) 
		{

			PinOutput();
			HAL_GPIO_WritePin(_Port, _Pin, GPIO_PIN_RESET);
			DelayUs(1);

			PinInput();
			DelayUs(60);
		}

		else
		{
			PinOutput();
			HAL_GPIO_WritePin(_Port, _Pin, GPIO_PIN_RESET);
			DelayUs(60);
			PinInput();
		}
	}

}
uint8_t DHAL::OneWire::OneWire_Read(void)
{
	uint8_t value = 0;
	PinInput();

	for (int i = 0; i < 8; i++)
	{
		PinOutput();

		HAL_GPIO_WritePin(_Port, _Pin, GPIO_PIN_RESET);
		DelayUs(2);

		PinInput();
		if(HAL_GPIO_ReadPin(_Port, _Pin))
		{
			value |= 1 << i;
		}
		DelayUs(60); 
	}
	return value;
}

DHAL::OneWire::OneWire(GPIO_TypeDef *Port, uint16_t Pin)
{
	_Port = Port;
	_Pin = Pin;
}

void DHAL::OneWire::PinInput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = _Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(_Port, &GPIO_InitStruct);
}

void DHAL::OneWire::PinOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = _Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(_Port, &GPIO_InitStruct);
}