/*
 * retarget.c
 *
 *  Created on: Dec 2, 2023
 *      Author: jeffr
 */

#include "main.h"

int __io_putchar(int ch){
	ITM_SendChar(ch);
	return ch;
}
