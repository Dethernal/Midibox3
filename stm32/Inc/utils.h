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

#ifdef __cplusplus
 extern "C" {
#endif

void delay_ms(int ms);

#ifdef __cplusplus
}
#endif

#endif
