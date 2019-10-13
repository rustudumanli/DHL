#ifndef DHAL_DELAY_H
#define DHAL_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "stdbool.h"
	uint32_t DelayInit(void);
	
	void DelayUs(uint32_t Value);
	void DelayMs(uint32_t Value);
	
#ifdef __cplusplus
}
#endif

#endif
