/*
 * led.c
 *
 *  Created on: Oct 19, 2020
 *      Author: christopherdean
 */
/**
  * @brief  Write an LED high or low
  * @param  LED ID from 1 - 9
  * @param  State to set, 1 = High, 0 = Low
  * @retval None
  */
#include "led.h"

extern int8_t direction;
extern uint8_t state;
extern uint32_t delay;

void LED_writeState(uint8_t led_id, GPIO_PinState state){
	if (led_id < 5){
		HAL_GPIO_WritePin(GPIOA, 1<<(led_id-1), state);
	} else if (led_id > 5) {
		HAL_GPIO_WritePin(GPIOA, 1<<(led_id-2), state);
	} else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, state);
	}
}

void LED_disableAll(){
	for (uint8_t i = 1; i < 10; i++){
		LED_writeState(i, GPIO_PIN_RESET);
	}
}

void LED_blink_withDelay(uint8_t led_id, uint8_t iter, uint32_t delay){
	for (uint8_t i = 0; i < iter; i++){
		LED_writeState(led_id, 0);
		HAL_Delay(delay);
		LED_writeState(led_id, 1);
		HAL_Delay(delay);
	}
	// disable after blink is done
	LED_writeState(led_id, 0);
}

void LED_blink(uint8_t led_id, uint8_t iter){
	LED_blink_withDelay(led_id, iter, (uint32_t)100);
}


void LED_sweep(){
	// globals won't be known about if move this func to led.h

	// direction == 1 -> count from 1 to 9
	// direction == 0 -> count from 9 to 1
	int8_t cur_dir = direction;
	int8_t i;
	i = direction ? 1 : 9;
    // sweep either direction and reflect direction state on the fly
	while (i < 10 && i > 0) {
		state = cur_dir == direction ? state : 1 - state;
		cur_dir = direction;
		i += direction ? 1 : -1;
		LED_writeState(i, state);
		HAL_Delay(delay);
	}
	  state = 1 - state;
}

void LED_errorState(){
	LED_disableAll();
	LED_blink(1, 4);
}


