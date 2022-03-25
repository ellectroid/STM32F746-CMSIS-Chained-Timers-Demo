#include "main.h"

int main(void) {
	rcc_setup(); //clock for peripheral, clock will be set to 216MHz
	gpio_setup(); //set pin modes and functions
	systick_setup(SYSTEM_FREQUENCY);

	/**
	 *    CHAINED TIMERS
	 *
	 * TIM1: Master Timer
	 * One-Pulse Mode
	 * Period: 3s
	 * Update event interrupt sets PI1 HIGH (DISCO LED1)
	 * Starts manually
	 *
	 * TIM3: Slave Timer (Master: TIM1)
	 * PWM2 mode (initially low)
	 * Output Channel 1 to PB4 (DISCO ARD D3)
	 * Period: 3ms
	 * Duty Cycle 33%
	 * Starts by enabling counter of TIM1
	 *
	 * TIM5: Slave Timer (Master: TIM3)
	 * PWM2 mode (initially low)
	 * Output Channel 4 to PI0 (DISCO ARD D5)
	 * Period: 1ms
	 * Duty Cycle: 25%
	 * Starts by the first rising edge of TIM3
	 *
	 * TIM12: Slave Timer (Master: TIM5)
	 * PWM2 mode (initially low)
	 * Output Channel 2 to PB15 (DISCO ARD D11)
	 * Period: 4ms
	 * Duty Cycle: 25%
	 * Counter counts TIM5 rising edges
	 *
	 */

	advanced_timer_setup_master(TIM1);
	general_purpose_timer_setup_slave1(TIM3);
	general_purpose_timer_setup_slave2(TIM5);
	general_purpose_timer_setup_slave3(TIM12);

	TIM1->CR1 |= TIM_CR1_CEN; //Enable chained timers

	while (1) {
	}
}
