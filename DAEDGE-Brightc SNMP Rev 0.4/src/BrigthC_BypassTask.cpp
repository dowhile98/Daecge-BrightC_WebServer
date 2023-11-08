/**
 * @file BrigthC_BypassTask.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/*Includes -------------------------------------------------------------------------*/
#include "main.h"
#include "BrigthC_App.h"
#include "SNMP_OID.h"
/*Defines-------------------------------------------------------------------*/
#define TASK_STATE_AA		0
#define TASK_STATE_OTHER	1
#define TASK_STATE_NONE		2
#define TASK_STATE_EXIT		3
/*Global variables ---------------------------------------------------------*/
extern MIB mib;
/*Private function ---------------------------------------------------------*/
static void brigthcEnterBypass(void);
static void brigthcExitBypass(void);

/*Task definition ----------------------------------------------------------*/


void Bypass_Task(void  * argument){
	/*Local variables ------------------------------------------------------*/
	EventBits_t controlFlags;
	uint8_t state = TASK_STATE_NONE;
	//Bypass_queue_t xQueue;                       //entrar al modo bypass de acuerdo a lo recibido
  	LCD_queue_t xQueue_lcd;
  	buzzer_queue_t xQueue_buzer;
	uint32_t lcdUpdateTicks = millis();
	/*Init------------------------------------------------------------------*/
	printf("[bypass]: Iniciando.....");
	for(;;){

		controlFlags = xEventGroupWaitBits(controlEvents,
						(BRIGTHC_CTRL_BYPASS_AA | BRIGTHC_CTRL_BYPASS_EXT | \
						BRIGTHC_CTRL_BYPASS_TEMP | BRIGTHC_CTRL_BYPASS_LOG | \
						BRIGTHC_CTRL_BYPASS_EXIT),
						pdTRUE, pdFALSE, 50);
		//se verifica cual fue el motivo del evento
		if(controlFlags & BRIGTHC_CTRL_BYPASS_AA){
			dataIn.S_BYPASSS_STS = true;           //indica que se está en modo bypass
			//BUZZER
			xQueue_buzer.repeats = 3;
        	xQueue_buzer.delay = 500;
        	xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
			//LCD
			sprintf(xQueue_lcd.line1, "->MODO BYPASS<-");
          	sprintf(xQueue_lcd.line2, "T1:%dC T2:%dC",(int)sensorData.TMP1,(int)sensorData.TMP2);
          	xQueue_lcd.view_time = 5;
          	xQueueSend(lcd_queue, (void*)&xQueue_lcd, 100);

			//ENTER BYPASS
			brigthcEnterBypass();
			xEventGroupSetBits(systemEvents, BRIGTHC_STATE_BYPASS);
			xEventGroupSetBits(bypasState, BYPASS_AA);
			state = TASK_STATE_AA;
			Serial.printf("[BYPASS TASK] entrando a bypass por falla de control de temperatura\r\n");
		}else if(controlFlags & BRIGTHC_CTRL_BYPASS_EXT){
			dataIn.S_BYPASSS_STS = true;           //indica que se está en modo bypass
			//BUZZER
			xQueue_buzer.repeats = 3;
        	xQueue_buzer.delay = 500;
        	xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
			//LCD
			sprintf(xQueue_lcd.line1, "->MODO BYPASS<-");
          	sprintf(xQueue_lcd.line2, "MANTENIMIENTO");
          	xQueue_lcd.view_time = 5;
          	xQueueSend(lcd_queue, (void*)&xQueue_lcd, 100);
			//ENTER BYPASS
			brigthcEnterBypass();
			xEventGroupSetBits(bypasState, BYPASS_EXT);
			xEventGroupSetBits(systemEvents, BRIGTHC_STATE_BYPASS);
			state = TASK_STATE_OTHER;
			Serial.printf("[BYPASS TASK] entrando a bypass por switch externo\r\n");
		}else if(controlFlags & BRIGTHC_CTRL_BYPASS_LOG){
			dataIn.S_BYPASSS_STS = true;           //indica que se está en modo bypass
			//BUZZER
			xQueue_buzer.repeats = 3;
        	xQueue_buzer.delay = 500;
        	xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
			//LCD
			sprintf(xQueue_lcd.line1, "->MODO BYPASS<-");
          	sprintf(xQueue_lcd.line2, "-----LOGICO-----");
          	xQueue_lcd.view_time = 5;
          	xQueueSend(lcd_queue, (void*)&xQueue_lcd, 100);
			//ENTER BYPASS
			brigthcEnterBypass();
			xEventGroupSetBits(systemEvents, BRIGTHC_STATE_BYPASS);
			state = TASK_STATE_OTHER;
			xEventGroupSetBits(bypasState, BYPASS_LOG);
			Serial.printf("[BYPASS TASK] entrando a bypass por comando wireless\r\n");
		}else if(controlFlags & BRIGTHC_CTRL_BYPASS_TEMP){
			dataIn.S_BYPASSS_STS = true;           //indica que se está en modo bypass
			//BUZZER
			xQueue_buzer.repeats = 3;
        	xQueue_buzer.delay = 500;
        	xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
			//LCD
			sprintf(xQueue_lcd.line1, "->MODO BYPASS<-");
          	sprintf(xQueue_lcd.line2, "TEMP. SENSOR");
          	xQueue_lcd.view_time = 5;
          	xQueueSend(lcd_queue, (void*)&xQueue_lcd, 100);
			//ENTER BYPASS
			brigthcEnterBypass();
			state = TASK_STATE_OTHER;
			xEventGroupSetBits(bypasState, BYPASS_TMP);
			xEventGroupSetBits(systemEvents, BRIGTHC_STATE_BYPASS);
			Serial.printf("[BYPASS TASK] entrando a bypass por falla de sensores de temperatura\r\n");
		}else if(controlFlags & BRIGTHC_CTRL_BYPASS_EXIT){
			state = TASK_STATE_EXIT;
			Serial.printf("[BYPASS TASK] saliendo del modo bypass\r\n");
		}

		switch (state) {
			case TASK_STATE_AA:
				if((millis() - lcdUpdateTicks) >= 10000){
					lcdUpdateTicks = millis();
					sprintf(xQueue_lcd.line1, "->MODO BYPASS<-");
          			sprintf(xQueue_lcd.line2, "ERR! AA",(int)sensorData.TMP1,(int)sensorData.TMP2);
          			xQueue_lcd.view_time = 5;
          			xQueueSend(lcd_queue, (void*)&xQueue_lcd, 100);
				}
				xSemaphoreTake(shareDataMutex, portMAX_DELAY);
				if((sensorData.TMP1 <= sec_AA.SEC_TMP_MIN) && (sensorData.TMP2 <= sec_AA.SEC_TMP_MIN)){ //se comprueba el rango de temperatura para salir de bypass
					dataIn.S_BYPASSS_STS = 0;
					xEventGroupClearBits(systemEvents, BRIGTHC_STATE_BYPASS);
					xEventGroupSetBits(systemEvents, BRIGTHC_STATE_RETURN_CTRL);
					state = TASK_STATE_NONE;
					brigthcExitBypass();
					Serial.printf("[BYPASS TASK] saliendo de bypass. Temperatura en el rango permitido\r\n");
					//LCD
					sprintf((char*)xQueue_lcd.line1, "--->SALIENDO<---");
					sprintf((char*)xQueue_lcd.line2, "BYPAS TIMOUT");
					xQueue_lcd.view_time = 3;
					xQueueSend(lcd_queue, (void *) &xQueue_lcd, 100);
					//BUZZER
					xQueue_buzer.repeats = 3;
					xQueue_buzer.delay = 100;
					xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
					mib.setAAState(V_AA);
				}
				xSemaphoreGive(shareDataMutex);
				break;
			case TASK_STATE_OTHER:
				if((millis() - lcdUpdateTicks) >= 10000){
					lcdUpdateTicks = millis();
					sprintf(xQueue_lcd.line1, "--->BRIGHTC<---");
          			sprintf(xQueue_lcd.line2, "BYPASS",(int)sensorData.TMP1,(int)sensorData.TMP2);
          			xQueue_lcd.view_time = 5;
          			xQueueSend(lcd_queue, (void*)&xQueue_lcd, 100);
				}
				break;
			case TASK_STATE_EXIT:
				dataIn.S_BYPASSS_STS = 0;
				//se sale de bypass
				xEventGroupClearBits(systemEvents, BRIGTHC_STATE_BYPASS);
				xEventGroupSetBits(systemEvents, BRIGTHC_STATE_RETURN_CTRL);
				state = TASK_STATE_NONE;
				brigthcExitBypass();
				//LCD
				sprintf((char*)xQueue_lcd.line1, "--->SALIENDO<---");
				sprintf((char*)xQueue_lcd.line2, "BYPAS");
				xQueue_lcd.view_time = 3;
				xQueueSend(lcd_queue, (void *) &xQueue_lcd, 100);
				//BUZZER
				xQueue_buzer.repeats = 3;
				xQueue_buzer.delay = 100;
				xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
				mib.setAAState(V_AA);
				break;
			default:
				break;
		}
	}
}
/*Function definition --------------------------------------------------------------------*/
static void brigthcEnterBypass(void){
	//todo se entra a bypass
	for(uint8_t  i = 0; i < 4; i++){
		//Se apaga los pines S_BYPASS
		PCF2.digitalWrite(V_AA[i].p_s_bypass_pin, LOW);
		//Se enciendo la salida BYPASS
		digitalWrite(P_S_BYPASS_sts, HIGH);
	}
}
static void brigthcExitBypass(void){
	for(uint8_t  i = 0; i < sec_AA.SEC_CANT_AA; i++){
		//Se apaga los pines S_BYPASS
		PCF2.digitalWrite(V_AA[i].p_s_bypass_pin, HIGH);
		//Se enciendo la salida BYPASS
		digitalWrite(P_S_BYPASS_sts, LOW);
	}
}
