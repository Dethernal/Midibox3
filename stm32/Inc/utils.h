#ifndef UTILS_H
#define UTILS_H

#include "stdint.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"

/* SOFTMPU: Additional defines, typedefs etc. for C */
typedef unsigned long Bit32u;
typedef int Bits;

#define SYSEX_SIZE 1024		// sysex buffer for delay calculation

void delay_ms(uint32_t ms);


#endif
