/*
 * led.c
 *
 *  Created on: Sep 14, 2025
 *      Author: Sreenivasa Reddy P
 */
#include "headers.h"
/**
 * @brief Displays the winning LED pattern for a fixed duration.
 * @details Enters a blocking loop that repeatedly calls pwm_led() to animate the winning
 * pattern.
 */

void display_win_row()
{
	// Hold the winning LEDs pattern for visual confirmatio
	for (int i = 0; i < 10000; i++)
	{
		pwm_led();
	}
	reset_led_brightness();
}

/**
 * @brief Refreshes the 3x3 LED matrix display for one full cycle (all three rows).
 * @details Scans through the three rows, setting the PWM duty cycle for each of the 9 LEDs
 * based on the global led_brightness array. It also manages the slow flicker
 * effect for any LED set to the LED_FLICKER state.
 */
void pwm_led()
{
	int blinkrate=222;

	// Decide if this is an "ON" or "OFF" part of the flicker cycle
	uint32_t flicker_state = (flicker_counter < blinkrate) ? 0 : LED_OFF;

	for(int r = 0; r < 3; r++)
	{
		// deactivate all rows before enabling current row
		GPIOB_ODR &= ~((1 << 1) | (1 << 2) | (1 << 6));


		// Check each LED: if it's a flicker state (500), use the flicker_state.
		// Otherwise, use its normal ON/OFF value.
		TIM3_CCR1 = (led_brightness[r][0] == LED_FLICKER) ? flicker_state : led_brightness[r][0];
		TIM3_CCR2 = (led_brightness[r][1] == LED_FLICKER) ? flicker_state : led_brightness[r][1];
		TIM3_CCR3 = (led_brightness[r][2] == LED_FLICKER) ? flicker_state : led_brightness[r][2];

		// Force the PWM registers to update
		TIM3_EGR |= (1 << 0);
		GPIOB_ODR |= (1 << row_shift[r]);

		// Wait for the row scan delay
		delay_us(1500);
	}

	// Increment the slow flicker counter after each full matrix scan
	flicker_counter++;
	if (flicker_counter >= blinkrate*2) {
		flicker_counter = 0;
	}
}



/**
 * @brief Initializes the TIM2 hardware timer to be used for microsecond delays.
 * @details Configures TIM2 with a prescaler to make its counter increment every 1 microsecond.
 */
void timer_delay_init(void)
{
	RCC_APB1ENR = RCC_APB1ENR | (1 << 0); // Enable TIM2
	TIM2_PSC = 15; // divide to make it 1MHz
	TIM2_ARR = 0xFFFFFFFF; //make ARR count to max
	TIM2_CR1 = TIM2_CR1 |(1 << 0); // Enable TIM2
}
/**
 * @brief Creates a precise, delay for a specified number of microseconds.
 * @param us The number of microseconds to wait.
 */
void delay_us(uint32_t us)
{
	TIM2_CNT = 0;
	while (TIM2_CNT < us);
}
/**
 * @brief Resets the global led_brightness array, setting all LEDs to the OFF state.
 */
void reset_led_brightness()
{
	for(int row=0;row<3;row++)
	{
		for(int col=0;col<3;col++)
		{
			led_brightness[row][col]=LED_OFF; //turn off leds
		}
	}

}
