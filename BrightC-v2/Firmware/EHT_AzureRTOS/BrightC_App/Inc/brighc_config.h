/*
 * brighc_config.h
 *
 *  Created on: Dec 2, 2023
 *      Author: jeffr
 */

#ifndef INC_BRIGHC_CONFIG_H_
#define INC_BRIGHC_CONFIG_H_

#ifdef __cplusplus
extern "C"{
#endif

/*Defines ----------------------------------------------------------*/
/**
 * @brief Brightc Entry APP
 */
#define BRIGHTC_STACK			        1024
#define BRICHTC_PRIORITY		        10

/**
 * @brief NX
 */
#define DEFAULT_ARP_CACHE_SIZE	        1024 * 2
#define DEFAULT_PRIORITY                5
#define DEFAULT_MEMORY_SIZE             1024


#ifdef __cplusplus
}
#endif

#endif /* INC_BRIGHC_CONFIG_H_ */
