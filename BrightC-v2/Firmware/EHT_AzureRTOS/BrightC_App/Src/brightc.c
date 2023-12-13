/*
 * brightc.cpp
 *
 *  Created on: Dec 2, 2023
 *      Author: jeffr
 */

/*Includes ----------------------------------------------------------------*/
#include "brightc.h"


/*Defines -----------------------------------------------------------------*/


/*Typedefs ----------------------------------------------------------------*/

/*Global variables --------------------------------------------------------*/
TX_THREAD brigthc_thread;

/*Function prototype ------------------------------------------------------*/

/*Thread definition -------------------------------------------------------*/
void brightc_thread_entry(ULONG params){

	for(;;){
		tx_thread_sleep(MS_TO_TICKS(1000));
	}
}

/*Function definition -----------------------------------------------------*/
