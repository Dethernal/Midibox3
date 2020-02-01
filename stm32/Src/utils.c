#include "utils.h"

void delay_ms(uint32_t ms) {
    LL_mDelay(ms);
}
void delay_us(void) {
    __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
    /* Add this code to indicate that local variable is not used */
    ((void)tmp);


    while (Delay)
    {
      if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
      {
        Delay--;
      }
    }
}
