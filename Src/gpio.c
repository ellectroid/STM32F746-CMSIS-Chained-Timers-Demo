#include "gpio.h"

/*
 * Private functions
 */

void gpio_setup_porta(void);
void gpio_setup_portb(void);
void gpio_setup_porti(void);

void gpio_setup(void) {
	gpio_setup_porta();
	gpio_setup_portb();
	gpio_setup_porti();
}

void gpio_setup_porta(void) {

}
void gpio_setup_portb(void) {
	/*
	 *  PB4 TIM3_CH1 OUTPUT AF2
	 * */
	GPIOB->MODER = ((GPIOB->MODER & ~(0x03 << GPIO_MODER_MODER4_Pos)) | (0x02 << GPIO_MODER_MODER4_Pos)); //PB4 set to Alternate Function
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_4); //PB4 output push-pull
	GPIOB->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR4_Pos); //PB4 output speed very high
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR4); //PB4 no pull-up no pull-down
	GPIOB->AFR[0] |= (0x02 << GPIO_AFRL_AFRL4_Pos);  //Alternate Function 2

	/*
	 *  PB15 TIM12_CH2 OUTPUT AF9
	 * */
	GPIOB->MODER = ((GPIOB->MODER & ~(0x03 << GPIO_MODER_MODER15_Pos)) | (0x02 << GPIO_MODER_MODER15_Pos)); //PB15 set to Alternate Function
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_15); //PB15 output push-pull
	GPIOB->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR15_Pos); //PB15 output speed very high
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR15); //PB15 no pull-up no pull-down
	GPIOB->AFR[1] |= (0x09 << GPIO_AFRH_AFRH7_Pos);  //Alternate Function 9 TIM12_CH2

}
void gpio_setup_porti(void) {
	/*
	 *	PI0 ARD D5 TIM5_CH4 AF2
	 * */

	GPIOI->MODER = ((GPIOI->MODER & ~(0x03 << GPIO_MODER_MODER0_Pos)) | (0x02 << GPIO_MODER_MODER0_Pos)); //PI0 set to Alternate Function
	GPIOI->OTYPER &= ~(GPIO_OTYPER_OT_0); //PI0 output push-pull
	GPIOI->OSPEEDR |= (0x03 << GPIO_OSPEEDR_OSPEEDR4_Pos); //PI0 output speed very high
	GPIOI->PUPDR &= ~(GPIO_PUPDR_PUPDR0); //PI0 no pull-up no pull-down
	GPIOI->AFR[0] |= (0x02 << GPIO_AFRL_AFRL0_Pos);  //Alternate Function 2

	/*
	 *  PI1 GREEN LED LED1 ACTIVE HIGH
	 * */
	GPIOI->MODER = (GPIOI->MODER & ~(1U << 3U)) | (1U << 2U); //PI1 set to general purpose output mode
	GPIOI->OTYPER &= ~(GPIO_OTYPER_OT_1); //PI1 output push-pull
	GPIOI->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR1); //PI1 output speed very low
	GPIOI->PUPDR &= ~(GPIO_PUPDR_PUPDR1); //PI1 no pull-up no pull-down
}

void blink_led1(void) {
	GPIOI->ODR |= GPIO_ODR_OD1; //set PI1 to high
	system_msdelay(100U);
	GPIOI->ODR &= ~GPIO_ODR_OD1; //set PI1 to low
}
void toggle_led1(void) {
	GPIOI->ODR ^= GPIO_ODR_OD1;
}
