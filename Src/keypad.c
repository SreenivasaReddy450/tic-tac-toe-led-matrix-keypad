/*
 * keypad.c
 *
 *  Created on: Sep 14, 2025
 *      Author: Sreenivasa Reddy P
 */
#include "headers.h"


/**
 * @brief Scans the 4x4 keypad to detect a single, debounced keypress.
 * @retval The ASCII character of the pressed key ('0'-'9', etc.), or 0 if no key is pressed.
 */
int keypad_scan(void)
{
	int pressed =0;

	for(int row =0; row<4 ; row++)
	{
		//all row gpios are made high
		GPIOC_ODR = GPIOC_ODR| (0xF<<0);

		//row[i] made low
		GPIOC_ODR=GPIOC_ODR & ~(1<<row);
		for(int col =4; col<8; col++)
		{
			//check which col is active for the row made low
			if(!((GPIOC_IDR>>col) & 1))
			{
				delay_us(250000); //debounce
				pressed = keypad_map[row][col-4];

				while(!((GPIOC_IDR>>col) & 1)); // wait until key is released
				return pressed;

			}

		}
	}
	return pressed;
}
