/**
 * @file brightc_server.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/*Includes -----------------------------------------------------------------------------------*/
#include "brightc.h"
#include "app_azure_rtos.h"
#include "app_netxduo.h"
/*Defines ------------------------------------------------------------------------------------*/


/*Typedefs ----------------------------------------------------------------------------------*/

/*Global variables --------------------------------------------------------------------------*/
static const char *TAG = "SERVER";
static NX_WEB_HTTP_SERVER_MIME_MAP app_mime_maps[] =
{
  {"css", "text/css"},
  {"svg", "image/svg+xml"},
  {"png", "image/png"},
  {"jpg", "image/jpg"}
};

static uint8_t nx_server_pool[SERVER_POOL_SIZE];
static uint8_t server_stack[SERVER_STACK];
/*Function prototype ------------------------------------------------------------------------*/
/*Thread definition -------------------------------------------------------------------------*/
 void  nx_server_thread_entry(ULONG thread_input){
    
    /* HTTP WEB SERVER THREAD Entry */
    UINT    status;
    UINT ret ;
    NX_PARAMETER_NOT_USED(thread_input);
    STM32_LOGI(TAG, "Init...");
    STM32_LOGI(TAG, "Wait for media...");
	do{
	  tx_queue_receive(&ux_app_MsgQueue_msc, &fx_media, 10);
	  tx_thread_sleep(10);
	}while(fx_media == NULL);
	STM32_LOGI(TAG, "Media ready...");

	STM32_LOGI(TAG, "Server create");
	ret = nx_packet_pool_create(&WebServerPool, "HTTP Server Packet Pool", SERVER_PACKET_SIZE, nx_server_pool, SERVER_POOL_SIZE);

	/* Check for server pool creation status. */
	if (ret != NX_SUCCESS)
	{
		STM32_LOGI(TAG,"Server pool creation failed : 0x%02x\n", ret);
	}
	/* Create the HTTP Server. */
	ret = nx_web_http_server_create(&HTTPServer, "WEB HTTP Server", &NetXDuoEthIpInstance, CONNECTION_PORT,fx_media, server_stack,
								  SERVER_STACK, &WebServerPool, NX_NULL, webserver_request_notify_callback);

	if (ret != NX_SUCCESS)
	{
		 STM32_LOGI(TAG, "HTTP Server creation failed: 0x%02x\n", ret);
	}
	STM32_LOGI(TAG, "Server create finish");
    status = nx_web_http_server_mime_maps_additional_set(&HTTPServer,&app_mime_maps[0], 4);

    /* Start the WEB HTTP Server. */
    status = nx_web_http_server_start(&HTTPServer);

    /* Check the WEB HTTP Server starting status. */
    if (status != NX_SUCCESS)
    {
        /* Print HTTP WEB Server starting error. */
        STM32_LOGI(TAG, "HTTP WEB Server Starting Failed, error: 0x%02x\n", status);
        /* Error, call error handler.*/
        Error_Handler();
    }
    else
    {
        /* Print HTTP WEB Server Starting success. */
        STM32_LOGI(TAG, "HTTP WEB Server successfully started.\n");
        /* LED1 On. */
    }
    tx_thread_relinquish();
 }

/*Function definition ----------------------------------------------------------------------*/
 UINT webserver_request_notify_callback(NX_WEB_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr){
CHAR temp_string[30] = {'\0'};
  CHAR data[512] = {'\0'};
  UINT string_length;
  NX_PACKET *resp_packet_ptr;
  UINT status;
  ULONG resumptions;
  ULONG suspensions;
  ULONG idle_returns;
  ULONG non_idle_returns;
  ULONG total_bytes_sent;
  ULONG total_bytes_received;
  ULONG connections;
  ULONG disconnections;
  ULONG main_thread_count;
  ULONG server_thread_count;
  ULONG led_thread_count;
  CHAR *main_thread_name;
  CHAR *server_thread_name;
  CHAR *led_thread_name;

  /*
  * At each new request we toggle the green led, but in a real use case this callback can serve
  * to trigger more advanced tasks, like starting background threads or gather system info
  * and append them into the web page.
  */
  /* Get the requested data from packet */
  if( strcmp(resource, "/") == 0 && request_type == NX_WEB_HTTP_SERVER_GET_REQUEST){
	  STM32_LOGI(TAG, "Root");
	  /* Found it, override the GET processing by sending the resource
		  contents directly. */
	  nx_web_http_server_callback_data_send(server_ptr,
		  "HTTP/1.0 200 \r\nContent-Length:" "103\r\nContent-Type: text/html\r\n\r\n",
		  63);
	  nx_web_http_server_callback_data_send(server_ptr, "<HTML>\r\n<HEAD><TITLE>NetX HTTP Test </TITLE></HEAD>\r\n <BODY>\r\n<H1>NetX Test Page</H1>\r\n</BODY>\r\n</HTML>\r\n", 103);
	  /* Return completion status. */
	  return(NX_WEB_HTTP_CALLBACK_COMPLETED);
  }
  else if (strcmp(resource, "/GetTXData") == 0)
  {
    /* Let HTTP server know the response has been sent. */
    //tx_thread_performance_system_info_get(&resumptions, &suspensions, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &non_idle_returns, &idle_returns);

    sprintf (data, "%lu,%lu,%lu,%lu", resumptions, suspensions, idle_returns, non_idle_returns);
  }
  else if (strcmp(resource, "/GetNXData") == 0)
  {
    nx_tcp_info_get(&NetXDuoEthIpInstance, NULL, &total_bytes_sent, NULL, &total_bytes_received, NULL,  NULL, NULL, &connections, &disconnections, NULL, NULL);
    sprintf (data, "%lu,%lu,%lu,%lu",total_bytes_received, total_bytes_sent, connections, disconnections);
  }
    else if (strcmp(resource, "/GetNetInfo") == 0)
  {
   sprintf(data, "%lu.%lu.%lu.%lu,%d", (IpAddress >> 24) & 0xff, (IpAddress >> 16) & 0xff, (IpAddress >> 8) & 0xff, IpAddress& 0xff, CONNECTION_PORT);
  }

    else if (strcmp(resource, "/GetTxCount") == 0)
  {
    tx_thread_info_get(&NxAppThread, &main_thread_name, NULL, &main_thread_count, NULL, NULL, NULL, NULL, NULL);
    tx_thread_info_get(&AppServerThread, &server_thread_name, NULL, &server_thread_count, NULL, NULL, NULL, NULL, NULL);
    
    sprintf (data, "%s,%lu ,%s,%lu,%s,%lu", main_thread_name, main_thread_count, server_thread_name, server_thread_count,led_thread_name, led_thread_count);

  }
    else if (strcmp(resource, "/GetNXPacket") == 0)
  {
    sprintf (data, "%lu", NxAppPool.nx_packet_pool_available);
  }
    else if (strcmp(resource, "/GetNXPacketlen") == 0)
  {
    sprintf (data, "%lu", (NxAppPool.nx_packet_pool_available_list)->nx_packet_length );
  }
  else if (strcmp(resource, "/LedOn") == 0)
  {
    printf(" Loggling Green Led On \n");
    
  }
  else if (strcmp(resource, "/LedOff") == 0)
  {
    printf(" Loggling Green Led Off \n");
    
  }
  else
  {
    return NX_SUCCESS;
  }
  /* Derive the client request type from the client request. */
  nx_web_http_server_type_get(server_ptr, server_ptr -> nx_web_http_server_request_resource, temp_string, &string_length);

  /* Null terminate the string. */
  temp_string[string_length] = '\0';

  /* Now build a response header with server status is OK and no additional header info. */
  status = nx_web_http_server_callback_generate_response_header(server_ptr, &resp_packet_ptr, NX_WEB_HTTP_STATUS_OK,
                                                                strlen(data), temp_string, NX_NULL);

  status = _nxe_packet_data_append(resp_packet_ptr, data, strlen(data), server_ptr->nx_web_http_server_packet_pool_ptr, NX_WAIT_FOREVER);
  /* Now send the packet! */

  status = nx_web_http_server_callback_packet_send(server_ptr, resp_packet_ptr);
  if (status != NX_SUCCESS)
  {
    nx_packet_release(resp_packet_ptr);
    return status;
  }
  return(NX_WEB_HTTP_CALLBACK_COMPLETED);
 }
