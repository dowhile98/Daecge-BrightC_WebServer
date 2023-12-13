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
UINT webserver_request_notify_callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr){

	return 0;
}