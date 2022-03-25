#ifndef GPTMR_H_
#define GPTMR_H_

#include "stm32f746xx.h"
#include "gpio.h"

void general_purpose_timer_setup_slave1(TIM_TypeDef *TMR);
void general_purpose_timer_setup_slave2(TIM_TypeDef *TMR);
void general_purpose_timer_setup_slave3(TIM_TypeDef *TMR);


#endif /* ADVTMR_H_ */
