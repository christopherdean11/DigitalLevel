/*
 * led.h
 *
 *  Created on: Oct 19, 2020
 *      Author: christopherdean
 */

#ifndef SRC_LED_H_
#define SRC_LED_H_
#include "stm32f0xx_hal.h"



void LED_writeState(uint8_t led_id, GPIO_PinState state);
void LED_disableAll();
void LED_blink_withDelay(uint8_t led_id, uint8_t iter, uint32_t delay);
void LED_blink(uint8_t led_id, uint8_t iter);
void LED_sweep();
void LED_errorState();


#endif /* SRC_LED_H_ */
