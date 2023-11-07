/**
 * @file BrigthC_App.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BRIGTHC_APP
#define __BRIGTHC_APP

/*Defines ------------------------------------------------------------------------------------*/
#define AA_ON			1
#define AA_OFF			0
#define AA_TURN			1
#define AA_REPOSE		0


#define AA_NUM_MAX		4
/*Typedefs -----------------------------------------------------------------------------------*/
typedef enum{
	AA1 = 0,
	AA2 = 1,
	AA3 = 2,
	AA4 = 3
}AA_Enum_t;

typedef enum{
	CANT_AA_1,
	CANT_AA_2,
	CANT_AA_3,
	CANT_AA_4
}Cant_AA_t;


typedef enum{
	BRIGTHC_SEC_DAY_CPLT			= 1,				//! Cambio de turno por secuencia de dias
	BRIGTHC_CHANGE_NUMBER_AA		= 1<<1,				//! cambio de turno por cambio de cantidad de aires acondicionados
	BRIGTHC_CHANGE_BY_FAIL_AA		= 1<<2,				//! cambio de turno por falla de aires acondicionados
	BRIGTHC_SEC_UPADTE				= 1<<3,				//! actualizacion de turnos de los AA
	BRIGTHC_SEC_READY				= 1<<4,				//! listo para iniciar el secuenciamiento de aires acondicionados
	BRIGTHC_SEC_CONF_OK				= 1<<6				//! configuracion correcta se puede continuar con el secuenciamiento
}BrigthC_Sec_Event_t;

typedef enum{
	BRIGTHC_CTRL_CONF_OK			= 1,				//!configuracion correcta se puede continuar con el control
	BRIGTHC_CTRL_BYPASS_EXIT		= 1<<1,				//!modo bypáss
	BRIGTHC_CTRL_NO_ACTION			= 1<<2,				//!se encuentra en modo de no control
	BRIGTHC_CTRL_BYPASS_EXT			= 1<<3,				//!bypass modo externo por switch externo
	BRIGTHC_CTRL_BYPASS_AA			= 1<<4,				//!bypass por falla de control de AA
	BRIGTHC_CTRL_BYPASS_TEMP		= 1<<5,				//!bypass por falla de los sensores de temperatura,
	BRIGTHC_CTRL_BYPASS_LOG			= 1<<6,				//!bypass por comando externo recibido
}BrigthC_Ctrl_Event_t;

typedef enum{
	BRIGTHC_CTRL_CONF_T_ERR			= 1,				//!Error en la configuracion de limites de temperatura
	BRIGTHC_CTRL_CONF_TSP_ERR		= 1<<1,				//!error en la configuracon de tiempo de enfriamiento maximo
	BRIGTHC_SEC_CONF_ERR			= 1<<2				//!error en la configuracon de cantidad de AA 
}BrigthC_Err_Event_t;

typedef enum{
	BRIGTHC_STATE_CTRL 				= 1,
	BRIGTHC_STATE_BYPASS			= 1<<1,
	BRIGTHC_STATE_RETURN_CTRL		= 1<<2,
	BRIGTHC_STATE_MANUAL			= 1<<3
}BrigthC_State_t;

typedef enum{
	BYPASS_AA						= 1,
	BYPASS_TMP						= 1<<1,
	BYPASS_EXT						= 1<<2,
	BYPASS_LOG						= 1<<3
}BrigthC_Bypass_t;


#endif