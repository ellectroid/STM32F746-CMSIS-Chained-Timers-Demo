#include "gptmr.h"

void general_purpose_timer_setup_slave1(TIM_TypeDef *TMR) {
	//TIMER 3 channel 1 PB4 ARD D3
	//TIM3,4 16-bit ARR, TIM2,5 32-bit

	/*
	 * CR1 reset value = 0x00000000
	 * ARR is not buffered
	 * upcounter (default)
	 * update request source: only overflow/underflow
	 *
	 * */
	TMR->CR1 = 0x00; //reset
	TMR->CR1 |= TIM_CR1_URS;

	/*
	 * 108MHz Clock, 64000 prescaler -> 2kHz Timer Clock
	 * Reload value 6, Period = 6/2000s = 3ms
	 * */
	TMR->PSC = (108000U / 2U) - 1U; //APB1 is 54MHz, timer is 2x APB1 Freq, so now the timer is at 2kHz; 16-bit value!!! 65535 max!
	TMR->ARR = 6U - 1U; //6 ticks at 2kHz is 3ms
	TMR->CNT = 0x00; //start value
	TMR->EGR |= TIM_EGR_UG; //force update event and load all values into registers
	TMR->SR &= ~TIM_SR_UIF; //force clear update event

	/*
	 * SMCR Slave Mode Control Register reset value = 0x00000000
	 * Trigger Selection - ITR0 (TIM1 as Master for TIM3)
	 * Slave Mode Selection - Trigger Mode - The counter starts at a rising edge of the trigger TRGI (but it is not
	 *	reset). Only the start of the counter is controlled (0b0110)
	 */
	TMR->SMCR = 0x00; //reset
	TMR->SMCR |= (0x00 << TIM_SMCR_TS_Pos) | (0x00 << 16U) | (0x06 << TIM_SMCR_SMS_Pos);

	/*
	 * CR2 reset value = 0x00000000
	 * Master Mode Selection 1: OC1REF Triggers TRGO to start another timer
	 * Master Mode Selection 2: reset
	 * Compare value: 4 (output LOW: CNT = 0, 1, 2, 3; output HIGH CNT = 4, 5)
	 * Duty cycle: 33.33%
	 * Output compare 1 mode - PWM mode 2 (0b0111)
	 *
	 * */
	TMR->CR2 = 0x00; //reset
	TMR->CR2 |= (0x04 << TIM_CR2_MMS_Pos); //OC1REF as TRGO
	TMR->CCR1 = 4U;
	TMR->CCMR1 = 0x00;
	TMR->CCMR1 |= (0x07 << TIM_CCMR1_OC1M_Pos);

	/*
	 * Capture Compare Enable Register
	 * Polarity: default - active high
	 * Capture Compare Output Enable
	 * */
	TMR->CCER = 0x00; //reset
	TMR->CCER |= TIM_CCER_CC1E;

}

void general_purpose_timer_setup_slave2(TIM_TypeDef *TMR) {
	//TIMER 5 channel 4 PI0 ARD D5
	//TIM3,4 16-bit ARR, TIM2,5 32-bit

	/*
	 * CR1 reset value = 0x00000000
	 * ARR is not buffered
	 * upcounter (default)
	 * update request source: only overflow/underflow
	 *
	 * */
	TMR->CR1 = 0x00; //reset
	TMR->CR1 |= TIM_CR1_URS;

	/*
	 * CR2 reset value = 0x00000000
	 * Master Mode Selection 1: OC4REF triggers TRGO
	 * Master Mode Selection 2: reset
	 *
	 * */
	TMR->CR2 = 0x00; //reset
	TMR->CR2 |= (0x07 << TIM_CR2_MMS_Pos);

	/*
	 * SMCR Slave Mode Control Register reset value = 0x00000000
	 * Trigger Selection - ITR1 (TIM3 as Master for TIM5)
	 * Slave Mode Selection - Trigger Mode - The counter starts at a rising edge of the trigger TRGI (but it is not
	 *	reset). Only the start of the counter is controlled (0b0110)
	 */
	TMR->SMCR = 0x00; //reset
	TMR->SMCR |= (0x01 << TIM_SMCR_TS_Pos) | (0x00 << 16U) | (0x06 << TIM_SMCR_SMS_Pos);

	TMR->PSC = 1U - 1U; //full 108MHz counter clock
	TMR->ARR = 108000U - 1U; //32-bit for TIM2/TIM5, 16-bit for TIM3/TIM4; 1ms period
	TMR->CNT = 0x00; //start value
	TMR->EGR |= TIM_EGR_UG; //force update event and load all values into registers
	TMR->SR &= ~TIM_SR_UIF; //force clear update event

	/*
	 * CCMR1 Capture/Compare Mode Register 2
	 * CC4 channel as output
	 * Output Compare 4 mode - PWM2 mode (channel active on CNT>=CCR4) 0b0111
	 *
	 * */
	TMR->CCMR2 = 0x00; //reset
	TMR->CCMR2 |= (0x00 << TIM_CCMR2_CC4S_Pos) | (0x07 << TIM_CCMR2_OC4M_Pos); //CC4 as output
	TMR->CCR4 = 108000U / 100U * 75U - 1U; //Duty cycle 25%

	/*
	 * Capture Compare Enable Register
	 * Polarity: default - active high
	 * Output Enable
	 *
	 * */
	TMR->CCER = 0x00; //reset
	TMR->CCER |= TIM_CCER_CC4E;

}

void general_purpose_timer_setup_slave3(TIM_TypeDef *TMR) {
	//TIMER 12 channel 2 PB15 ARD D11
	//TIM9,TIM12 16-bit ARR, 16-bit PSC

	/*
	 * Update Request Source - only overflow
	 * */
	TMR->CR1 = 0x00; //reset
	TMR->CR1 |= TIM_CR1_URS;

	/*
	 *	Prescaler 1
	 *	Period 4 units
	 *	Period length depends on master timer (see slave setup below)
	 * */
	TMR->PSC = (1U) - 1U;
	TMR->ARR = 4U - 1U; //10 ticks
	TMR->CNT = 0x00; //start value
	TMR->EGR |= TIM_EGR_UG; //force update event and load all values into registers
	TMR->SR &= ~TIM_SR_UIF; //force clear update event

	/*
	 * SMCR Slave Mode Control Register reset value = 0x00000000
	 * Trigger Selection - ITR1 (TIM5 as Master)
	 * Slave Mode Selection - External Clock Mode 1 - Rising edges of the selected trigger (TRGI) clock the counter (0b0111) - counts TIM5 pulses
	 * This timer counts master timer's pulses
	 */
	TMR->SMCR = 0x00; //reset
	TMR->SMCR |= (0x01 << TIM_SMCR_TS_Pos) | (0x00 << 16U) | (0x07 << TIM_SMCR_SMS_Pos);


	/*
	 * CCMR1 Capture/Compare Mode Register 1
	 * CC2 channel as output
	 * Output Compare 2 mode - PWM2 mode (channel active on CNT>=CCR2) 0b0111
	 *
	 * */
	TMR->CCMR1 = 0x00; //reset
	TMR->CCMR1 |= (0x00 << TIM_CCMR1_CC2S_Pos) | (0x07 << TIM_CCMR1_OC2M_Pos); //CC2 as output
	TMR->CCR2 = TMR->ARR; //Duty cycle 1 CNT out of ARR

	/*
	 * Capture Compare Enable Register
	 * Polarity: default - active high
	 * Output Enable
	 *
	 * */
	TMR->CCER = 0x00; //reset
	TMR->CCER |= TIM_CCER_CC2E;

	TMR->CR1 |= TIM_CR1_CEN; //enable counter; counter won't run until master timer runs

}

