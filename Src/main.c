/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Sreenivasa Reddy P
 * @brief          : Tic Tac Toe game
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "headers.h"
int turns = 0;
int row_shift[3] = {1, 2, 6};
uint32_t flicker_counter = 0;
char keypad_map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
uint32_t led_brightness[3][3] = {
    { 999, 999, 999 },
    { 999, 999, 999 },
    { 999, 999, 999 }
};

/**
 * @brief Main entry point of the Tic-Tac-Toe game.
 * @details Initializes all hardware peripherals (GPIO, Timers) and then enters the main game loop.
 * The loop refreshes the LED display, waits for keypad input, and processes game logic.
 * @retval int Returns 0 upon normal game completion.
 */
int main(void)
{
	char arr[3][3] = {{' ', ' ', ' '},
					  {' ', ' ', ' '},
					  {' ', ' ', ' '}};
	int ret, row, col, choice, display_grid=1;
	char win;

	RCC_AHB1ENR = RCC_AHB1ENR | (0b1<<2); //enable gpio C
	// Configure GPIOC Keypad Pins: PC0-3 are Output (Rows), PC4-7 are Input (Columns)
	GPIOC_MODER = GPIOC_MODER | (0b01<<0) | (0b01<<2) | (0b01<<4) | (0b01<<6); //configure gpio as ouput
	GPIOC_MODER = GPIOC_MODER &~((0b11<<8) | (0b11<<10) | (0b11<<12) | (0b11<<14)); //reset gpio configuration
	GPIOC_PUPDR = GPIOC_PUPDR | (0b01<<8) | (0b01<<10) | (0b01<<12) | (0b01<<14); //pullup resistor

	timer_delay_init();
	RCC_AHB1ENR = RCC_AHB1ENR | (0b1<<1); //enable gpio B
	RCC_APB1ENR = RCC_APB1ENR | (0b1<<1); //enable timer 3
	// Set PB0, PB4, PB5 columns of led matrix
	// Set PB1, PB2, PB6 rows of led matrix
	GPIOB_MODER = GPIOB_MODER | (0b10 << 0) | (0b10 << 8) | (0b10 << 10); //configure gpio for alternate function
	GPIOB_MODER = GPIOB_MODER | (0b01 << 2) | (0b01 << 4) | (0b01 << 12); //configure gpio as general purpose
	GPIOB_AFRL  =  GPIOB_AFRL | (0b0010 << 0) | (0b0010 << 16) |(0b0010 << 20); //configure for AF2
	GPIOB_ODR   = GPIOB_ODR   &~((0b1<<1)|(0b1<<2)|(0b1<<6)); //reset gpio of rows

	TIM3_CR1    = TIM3_CR1    &~(0b1<<4); //configured as upcounter
	TIM3_CR1 	= TIM3_CR1    | (0b1<<7); //auto reload enable

	TIM3_PSC 	= 15; // divide 16 MHz APB1 clock by 16 → 1 MHz timer clock
	TIM3_ARR 	= 999; // ARR for period

	TIM3_CCMR1 = TIM3_CCMR1 &~(0b11<<0); //channel configured as output
	TIM3_CCMR1 = TIM3_CCMR1 &~(0b11<<8); //channel configured as output
	TIM3_CCMR1 = TIM3_CCMR1 | (0b110<<4); //configured for pwm
	TIM3_CCMR1 = TIM3_CCMR1 | (0b110<<12); //configured for pwm
	TIM3_CCMR1 = TIM3_CCMR1 | (0b1<<3); //preload enabled
	TIM3_CCMR1 = TIM3_CCMR1 | (0b1<<11); //preload enabled

	TIM3_CCMR2 = TIM3_CCMR2 | (0b110<<4); //configured for pwm
	TIM3_CCMR2 = TIM3_CCMR2 &~(0b11<<0); //channel configured as output
	TIM3_CCMR2 = TIM3_CCMR2 | (0b1<<3); //preload enabled
	TIM3_CCER  = TIM3_CCER  | (0b1<<0); //enable channel 1
	TIM3_CCER  = TIM3_CCER  | (0b1<<4); //enable channel 2
	TIM3_CCER  = TIM3_CCER  | (0b1<<8); //enable channel 3
	TIM3_EGR   = TIM3_EGR   | (1<<0); //update generation
	TIM3_CR1   = TIM3_CR1   | (0b1<<0); //enable counter

	while(1)
	{
		pwm_led();
		if(turns>=9)
			break;

		if(display_grid)
		{
			display(arr);
			display_grid=0;
			printf("Player %d Enter position of %c\n", (turns % 2) + 1, ((turns % 2) + 1) == 1 ? 'X' : 'O');
		}
		choice = keypad_scan();
		choice = choice -'0';
		row = (choice - 1) / 3;
		col = (choice - 1) % 3;
		if(choice>0 && choice<=9)
		{
			display_grid=1;
			ret = data_entry(row, col, arr, ((turns % 2) + 1) == 1 ? 'X' : 'O');
			if(ret==0)
			{
				led_brightness[row][col] = ((turns % 2) + 1) == 1 ? LED_ON : LED_FLICKER;
			}
			if (turns >= 5)
			{
				win = validate(arr);

				display(arr);
				if (win == 'X')
				{
					printf("Player 1 wins\n");
					display_win_row();
					GPIOB_ODR &= ~((1 << 1) | (1 << 2) | (1 << 6)); //reset gpio
					return 0;
				}
				else if (win == 'O')
				{
					printf("Player 2 wins\n");
					display_win_row();
					GPIOB_ODR &= ~((1 << 1) | (1 << 2) | (1 << 6)); //reset gpio
					return 0;
				}
			}

		}


	}

	if(turns>=9)
	{
		printf("its a draw\n");
		GPIOB_ODR &= ~((1 << 1) | (1 << 2) | (1 << 6)); //reset gpio
	}

	return 0;
}


