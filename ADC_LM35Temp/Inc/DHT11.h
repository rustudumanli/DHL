#ifndef DHT11_H
#define DHT11_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"
#include "stdbool.h"

	
	namespace DHAL
	{
		class DHT11
		{
		public:
			void DHT11_Load(void);
			void DHT11_Check(void);
			uint8_t DHT11_Read(void);
			bool DHT11_GetVale(void);
			DHT11(GPIO_TypeDef *Port, uint16_t Pin);
			uint8_t Temp, PercentTemp, Humidity, PercentHumidity, Parity;
			bool Error = false;
			
		private:
			void PinInput(void);
			void PinOutput(void);
			GPIO_TypeDef *_Port;
			uint16_t  _Pin;
		};
	}
	



#ifdef __cplusplus
}
#endif

#endif
