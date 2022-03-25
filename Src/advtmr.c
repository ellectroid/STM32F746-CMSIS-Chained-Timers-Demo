#include "advtmr.h"

void advanced_timer_setup_master(TIM_TypeDef *TMR) {
	/*
	 * CR1 reset value = 0x00000000
	 * ARR is not buffered
	 * upcounter (default)
	 * update request source: only overflow/underflow
	 * One pulse mode (fire once)
	 *
	 * */
	TMR->CR1 = 0x00; //reset
	TMR->CR1 |= TIM_CR1_URS | TIM_CR1_OPM;


	/*
	 * CR2 reset value = 0x00000000
	 * Master Mode Selection 1: Enable. CNT_EN Triggers TRGO to start another timer
	 * Master Mode Selection 2: reset
	 *
	 * */
	TMR->CR2 = 0x00; //reset
	TMR->CR2 |= (0x01 << TIM_CR2_MMS_Pos);

	TMR->PSC = (216000U / 4U) - 1U; //APB2 is 108MHz, timer is 2x APB1 Freq, so now the timer is at 4kHz; 16-bit value!!! 65535 max!
	TMR->ARR = 12000U - 1U; //12000 ticks at 4kHz is 3s
	TMR->CNT = 0x00; //start value
	TMR->EGR |= TIM_EGR_UG; //force update event to make sure all values are loaded
	TMR->DIER |= TIM_DIER_UIE; //update event interrupt enable


	//enable interrupt
	TMR->SR = ~TIM_SR_UIF; //make sure the update flag is cleared
	NVIC_ClearPendingIRQ(TIM1_UP_TIM10_IRQn); //clear NVIC pending interrupt
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 4U);
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

}

void TIM1_UP_TIM10_IRQHandler(void) {
	TIM1->SR = ~TIM_SR_UIF; //make sure the update flag is cleared
	toggle_led1();
}
