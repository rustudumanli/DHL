#ifndef ONEWIRE_H
#define ONEWIRE_H

#ifdef __cplusplus
extern "C" {
#endif
	
	#include "main.h"

	
	namespace DHAL
	{
		class OneWire
		{
		public:
			uint8_t OneWire_Load(void);
			void OneWire_Write(uint8_t data);
			uint8_t OneWire_Read(void);
			OneWire(GPIO_TypeDef *Port, uint16_t Pin);
			
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
