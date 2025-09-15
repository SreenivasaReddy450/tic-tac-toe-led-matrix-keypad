/*
 * headers.h
 *
 *  Created on: Sep 14, 2025
 *      Author: Sreenivasa Reddy P
 */

#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// --- Clock Control (RCC) ---
#define RCC_BASE   0x40023800
#define RCC_AHB1ENR   (*(volatile uint32_t*)(RCC_BASE + 0x30))
#define RCC_APB1ENR   (*(volatile uint32_t*)(RCC_BASE + 0x40))

// --- Timer 3 (PWM for LEDs) ---
#define TIM3_BASE  0x40000400
#define TIM3_CR1      (*(volatile uint32_t*)(TIM3_BASE + 0x00))
#define TIM3_CNT 	  (*(volatile uint32_t*)(TIM3_BASE + 0x24))
#define TIM3_PSC 	  (*(volatile uint32_t*)(TIM3_BASE + 0x28))
#define TIM3_ARR 	  (*(volatile uint32_t*)(TIM3_BASE + 0x2C))
#define TIM3_CCR1 	  (*(volatile uint32_t*)(TIM3_BASE + 0x34))
#define TIM3_CCR2 	  (*(volatile uint32_t*)(TIM3_BASE + 0x38))
#define TIM3_CCR3 	  (*(volatile uint32_t*)(TIM3_BASE + 0x3C))
#define TIM3_CCMR1 	  (*(volatile uint32_t*)(TIM3_BASE + 0x18))
#define TIM3_CCMR2 	  (*(volatile uint32_t*)(TIM3_BASE + 0x1C))
#define TIM3_EGR 	  (*(volatile uint32_t*)(TIM3_BASE + 0x14))
#define TIM3_CCER 	  (*(volatile uint32_t*)(TIM3_BASE + 0x20))

// --- Timer 2 (Microsecond Delay) ---
#define TIM2_BASE  0x40000000
#define TIM2_CR1      (*(volatile uint32_t*)(TIM2_BASE + 0x00))
#define TIM2_CNT      (*(volatile uint32_t*)(TIM2_BASE + 0x24))
#define TIM2_PSC      (*(volatile uint32_t*)(TIM2_BASE + 0x28))
#define TIM2_ARR      (*(volatile uint32_t*)(TIM2_BASE + 0x2C))

// --- GPIO Port B (LED Matrix) ---
#define GPIOB_BASE 0x40020400
#define GPIOB_MODER   (*(volatile uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_ODR     (*(volatile uint32_t*)(GPIOB_BASE + 0x14))
#define GPIOB_AFRL    (*(volatile uint32_t*)(GPIOB_BASE + 0x20))

// --- GPIO Port C (Keypad) ---
#define GPIOC_BASE 0x40020800
#define GPIOC_MODER   (*(volatile uint32_t*)(GPIOC_BASE))
#define GPIOC_IDR     (*(volatile uint32_t*)(GPIOC_BASE + 0x10))
#define GPIOC_ODR     (*(volatile uint32_t*)(GPIOC_BASE + 0x14))
#define GPIOC_PUPDR   (*(volatile uint32_t*)(GPIOC_BASE + 0x0C))

//led brightness levels
#define LED_ON      0
#define LED_FLICKER 500
#define LED_OFF     999

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void display(char (*arr)[3]);
int  data_entry(int row, int col, char (*arr)[3], char ch);
char validate(char (*arr)[3]);
int  keypad_scan();
void timer_delay_init();
void delay_us(uint32_t us);
void pwm_led();
void reset_led_brightness();
void display_win_row();

extern int turns;
extern int row_shift[3];
extern uint32_t flicker_counter;
extern char keypad_map[4][4];
extern uint32_t led_brightness[3][3];

#endif /* HEADERS_H_ */
