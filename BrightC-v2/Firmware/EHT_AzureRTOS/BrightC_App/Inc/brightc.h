/*
 * brightc.h
 *
 *  Created on: Dec 2, 2023
 *      Author: jeffr
 */

#ifndef INC_BRIGHTC_H_
#define INC_BRIGHTC_H_

/*Includes ----------------------------------------------------------------*/
#include "brighc_config.h"
#include "brightc_typedefs.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "tx_api.h"
#include "fx_api.h"
#include "nx_api.h"
//#include "nxd_snmp.h"
#include "stm32_log.h"
#include  "nx_web_http_server.h"
#include  "app_filex.h"

/*defines -----------------------------------------------------------------*/

/*Typedefs ----------------------------------------------------------------*/

/*Extern definition -------------------------------------------------------*/
extern TX_THREAD brigthc_thread;

extern NX_IP  NetXDuoEthIpInstance;
extern FX_MEDIA        sdio_disk;
extern NX_WEB_HTTP_SERVER HTTPServer;
extern TX_THREAD AppServerThread;

extern ULONG IpAddress;
extern ULONG NetMask;
extern TX_THREAD      NxAppThread;
extern NX_PACKET_POOL NxAppPool;
/*function prototype ------------------------------------------------------*/
/**
 * @brief app entry thread
 */
void brightc_thread_entry(ULONG params);

/**
 * @brief 
 * 
 */
void  nx_server_thread_entry(ULONG thread_input);
/**
 * @brief 
 * 
 */
UINT webserver_request_notify_callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);


#endif /* INC_BRIGHTC_H_ */
