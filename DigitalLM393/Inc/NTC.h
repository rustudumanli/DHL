#ifndef NTC_H
#define NTC_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"
	
#ifndef NTC_OHM
#define NTC_OHM 10000
#endif
	
#ifndef ADCBIT
#define ADCBIT 12				  
#endif

	
	namespace DHAL
	{
		class NTC
		{
		public:
			float GetTemp(uint32_t ADCValue);
			
			NTC(float VRef);
			
		private:
			float _VRef;
		};
	}
	



#ifdef __cplusplus
}
#endif

#endif
