/*
 * brightc_typedefs.h
 *
 *  Created on: Dec 2, 2023
 *      Author: jeffr
 */

#ifndef INC_BRIGHTC_TYPEDEFS_H_
#define INC_BRIGHTC_TYPEDEFS_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"

/*CCMRAM Locate ----------------------------------------------------------------------*/
#define CCMRAM_LOCATE					__attribute__((section (".ccmram")))

/*Tdefines ------------------------------------------------------------------------*/
#define MS_TO_TICKS(ms)    ((ms) * TX_TIMER_TICKS_PER_SECOND / 1000)
#define HIGH				GPIO_PIN_SET
#define LOW					GPIO_PIN_RESET

#define CONTACT(a, b)		a ## b
#define PORT(a)				CONTACT(a, _GPIO_Port)
#define PIN(a)				CONTACT(a, _Pin)

#define OUTPUT_SET(a, val)	HAL_GPIO_WritePin(PORT(a), PIN(a), val)
#define INPUT_READ(a)		HAL_GPIO_ReadPin(PORT(a), PIN(a))
#define OUTPUT_TOGGLE(a)	HAL_GPIO_TogglePin(PORT(a), PIN(a))

#define SD_PRESENT                       1UL
#define SD_NOT_PRESENT                   0UL
/*Typedefs ------------------------------------------------------------------------*/

typedef enum {
CARD_STATUS_CHANGED             = 99,
CARD_STATUS_DISCONNECTED        = 88,
CARD_STATUS_CONNECTED           = 77
} SD_ConnectionStateTypeDef;

#ifdef __cplusplus
}
#endif

#endif /* INC_BRIGHTC_TYPEDEFS_H_ */
