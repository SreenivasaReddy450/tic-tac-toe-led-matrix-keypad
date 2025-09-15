/*
 * game_logic.c
 *
 *  Created on: Sep 14, 2025
 *      Author: Sreenivasa Reddy P
 */
#include "headers.h"
/**
 * @brief Prints the current state of the Tic-Tac-Toe board to the serial console.
 * @param arr Pointer to the 3x3 character array representing the game board.
 */

void display(char (*arr)[3])
{

	printf(" %c | %c | %c \n", arr[0][0], arr[0][1], arr[0][2]);
	printf("-----------\n");
	printf(" %c | %c | %c \n", arr[1][0], arr[1][1], arr[1][2]);
	printf("-----------\n");
	printf(" %c | %c | %c \n", arr[2][0], arr[2][1], arr[2][2]);
	printf("\n");
}
/**
 * @brief Processes a player's move, validates it, and updates the game board.
 * @param row The row index (0-2) of the desired move.
 * @param col The column index (0-2) of the desired move.
 * @param arr Pointer to the 3x3 character array representing the game board.
 * @param ch The player's character ('X' or 'O') to be placed on the board.
 * @retval 0 if the move was valid and the board was updated.
 * @retval 1 if the move was invalid (out of bounds or position already taken).
 */
int data_entry(int row, int col, char (*arr)[3], char ch)
{
	if (row < 0 || row >= 3 || col < 0 || col >= 3)
	{
		printf("Invalid position! Choose 1-9.\n");
		return 1;
	}
	if (arr[row][col] != ' ')
	{
		printf("The position is already filled!\n");
		return 1;
	}
	arr[row][col] = ch;
	turns++;

	return 0;
}
/**
 * @brief Checks the game board for a win condition.
 * @details Checks all rows, columns, and diagonals for three matching characters.
 * If a win is found, it also updates the global led_brightness array to highlight the winning line.
 * @param arr Pointer to the 3x3 character array representing the game board.
 * @retval 'X' if Player X has won.
 * @retval 'O' if Player O has won.
 * @retval ' ' (space) if there is no winner yet.
 */
char validate(char (*arr)[3])
{

	for (int i = 0; i < 3; i++)
	{
		// Row check for win
		if (arr[i][0] != ' ' && arr[i][0] == arr[i][1] && arr[i][1] == arr[i][2])
		{
			reset_led_brightness();
			if(arr[i][0]== 'X')
			{
				led_brightness[i][0]=led_brightness[i][1]= led_brightness[i][2] = LED_FLICKER;
			}
			else
			{
				led_brightness[i][0]=led_brightness[i][1]= led_brightness[i][2] = LED_ON;
			}

			return arr[i][0];
		}
		// Column check for win
		if (arr[0][i] != ' ' && arr[0][i] == arr[1][i] && arr[1][i] == arr[2][i])
		{
			reset_led_brightness();
			if(arr[0][i]== 'X')
			{
				led_brightness[0][i]=led_brightness[1][i]= led_brightness[2][i] = LED_FLICKER;
			}
			else
			{
				led_brightness[0][i]=led_brightness[1][i]= led_brightness[2][i] = LED_ON;
			}

			return arr[0][i];
		}
	}
	// Diagonals check for win
	if (arr[0][0] != ' ' && arr[0][0] == arr[1][1] && arr[1][1] == arr[2][2])
	{
		reset_led_brightness();
		if(arr[0][0]== 'X')
		{
			led_brightness[0][0]=led_brightness[1][1]= led_brightness[2][2] = LED_FLICKER;
		}
		else
		{
			led_brightness[0][0]=led_brightness[1][1]= led_brightness[2][2] = LED_ON;
		}

		return arr[0][0];
	}
	if (arr[0][2] != ' ' && arr[0][2] == arr[1][1] && arr[1][1] == arr[2][0])
	{
		reset_led_brightness();
		if(arr[0][2]== 'X')
		{
			led_brightness[0][2]=led_brightness[1][1]= led_brightness[2][0]=LED_FLICKER;
		}
		else
		{
			led_brightness[0][2]=led_brightness[1][1]= led_brightness[2][0]=LED_ON;
		}

		return arr[0][2];
	}

	return ' ';
}

