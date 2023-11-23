/**
 * @file BrigthC_SecTask.cpp
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
/*Global variables -----------------------------------------------------------------*/


/*Function prototype ---------------------------------------------------------------*/
void brigthC_UpdateAirsTurn(uint8_t *data);
void brigthc_InitairsTurn(uint8_t *data);
void brigthC_SetReady(AA_t *A);
extern MIB mib;
/*Task definition ------------------------------------------------------------------*/
void Sec_Task(void * argument){
	/*Local variables -----------------------------------------------------------------*/
	uint8_t airsTurn[AA_NUM_MAX] = {0};
	uint32_t ticksReInit;
	uint32_t timeStamp = 0;
	uint8_t dias = 0;
	EventBits_t secFlags;
    LCD_queue_t xQueue_lcd;
    buzzer_queue_t xQueue_buzer;
    /*Init sequence -------------------------------------------------------------------*/
	//Se verifica la cantidad de aires acondicionados configurados
	if((sec_AA.SEC_CANT_AA == 0) || (sec_AA.SEC_CANT_AA > AA_NUM_MAX)){
		xEventGroupSetBits(errorEvents, BRIGTHC_SEC_CONF_ERR);
        sprintf(xQueue_lcd.line1, "ERROR CANT. AA");
        sprintf(xQueue_lcd.line2, "CANT: %d", sec_AA.SEC_CANT_AA);
        xQueue_lcd.view_time = 4;
        xQueueSend(lcd_queue, (void *) &xQueue_lcd, 10);
        xQueue_buzer.delay = 100;
        xQueue_buzer.repeats = 4;
        xQueueSend(buzer_queue, (void *) & xQueue_buzer, 10);
        //wait for correct configuration
		xEventGroupWaitBits(secEvents, BRIGTHC_SEC_CONF_OK, pdTRUE, pdTRUE, portMAX_DELAY);
	}
	brigthc_InitairsTurn(airsTurn);
	//se inicia el turno de los AA
	brigthC_UpdateAirsTurn(airsTurn);

	for (uint8_t i = 0; i < sec_AA.SEC_CANT_AA - 1; i++) {
		Serial.printf("Aire %d ", airsTurn[i]);
	}
    printf("en turno.\n");
	
    digitalWrite(P_S_BYPASS_sts, LOW);                           //para el usuario
    //mensaje de confirmacion
    sprintf(xQueue_lcd.line1, "INICIANDO MODO");
    sprintf(xQueue_lcd.line2, "INTELIGENTE");
    xQueue_lcd.view_time = 2;
    xQueueSend(lcd_queue, (void *) &xQueue_lcd, 10);
	
	timeStamp = rtc.getEpoch();
	//sec ready
	//se entrega la bandera de secuencia lista
	xEventGroupSetBits(secEvents, BRIGTHC_SEC_READY);
	printf("[SEC]iniciando....\r\n");
    //se actualiza el contenido del MIB
	ticksReInit = rtc.getEpoch();
    mib.setAAState(V_AA);
    for(;;){
		
        if((rtc.getEpoch() - timeStamp) > TIME_STAMP_ONE_DAY){    //si transcurre un día
            timeStamp = rtc.getEpoch();
            dias += 1;
			//se reinicia la LCD
			lcd.init();
			lcd.clear();
			lcd.backlight();
            //transcurió se completó los dias de secuencia
            if(dias == sec_AA.SEC_DIAS){
                dias = 0;
                xEventGroupSetBits(secEvents, BRIGTHC_SEC_DAY_CPLT);
            }
			
            SERIAL_PRINT("[AA_SEC]->Timestamp: "); SERIAL_PRINTLN(rtc.getEpoch()); SERIAL_PRINT("[AA_SEC]->N° Dia: ");
            SERIAL_PRINTLN(dias);
            sprintf(xQueue_lcd.line1, "DIA DE SECUENCIA");
            sprintf(xQueue_lcd.line2,"       %d",dias);
            xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        }
        secFlags = xEventGroupWaitBits(secEvents,
				(BRIGTHC_SEC_DAY_CPLT | BRIGTHC_CHANGE_BY_FAIL_AA | BRIGTHC_CHANGE_NUMBER_AA),
				pdTRUE, pdFALSE, pdMS_TO_TICKS(1000));

		xSemaphoreTake(shareDataMutex, portMAX_DELAY);
        //->si se cambia el numero de AA
		if(secFlags & BRIGTHC_CHANGE_NUMBER_AA){
			dias  = 0;
			timeStamp = rtc.getEpoch();
			brigthc_InitairsTurn(airsTurn);
			brigthC_UpdateAirsTurn(airsTurn);
            mib.setAAState(V_AA);
		}
		//->Si se detecto una falla
		if(secFlags & BRIGTHC_CHANGE_BY_FAIL_AA){
			dias  = 0;
			timeStamp = rtc.getEpoch();
		}

		if(secFlags & (BRIGTHC_CHANGE_BY_FAIL_AA | BRIGTHC_SEC_DAY_CPLT)){

			// Realizar el cambio de turno
			//->cambio de turno
			int temp = airsTurn[0];
			for (uint8_t i = 0 ; i < (sec_AA.SEC_CANT_AA - 1);  i++) {
				airsTurn[i] = airsTurn[i + 1];
			}
			airsTurn[sec_AA.SEC_CANT_AA - 1] = temp;
			brigthC_UpdateAirsTurn(airsTurn);
			xEventGroupSetBits(secEvents, BRIGTHC_SEC_UPADTE);

			for (uint8_t i = 0; i < sec_AA.SEC_CANT_AA - 1; i++) {
				Serial.printf("Aire %d ", airsTurn[i]);
                if(V_AA[i].turn == 0){
                    sprintf(xQueue_lcd.line2, "AA%d EN DESCANSO", i+1);
                }
			}
			Serial.printf("en turno.\n");
            sprintf(xQueue_lcd.line1, "CAMBIO DE TURNO");
            xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
            //sound
            xQueue_buzer.repeats = 1;
            xQueue_buzer.delay = 200;
            xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
            //se publica el mensaje
            mib.setAAState(V_AA);
		}
		if((rtc.getEpoch() - ticksReInit)>TIME_STAMP_ONE_DAY/2){
			if(is_controlling != true){
				ticksReInit = rtc.getEpoch();
				PCF_Pin_Init();
				brigthC_SetReady(V_AA);
			}
		}
		xSemaphoreGive(shareDataMutex);
    }
}
/*Function definition --------------------------------------------------------------*/
void brigthc_InitairsTurn(uint8_t *data){
	for(uint8_t i = 0; i < sec_AA.SEC_CANT_AA; i++){
		data[i] = i + 1;
	}
}
void brigthC_UpdateAirsTurn(uint8_t *data){   
	V_AA[AA1].turn = AA_REPOSE;
	V_AA[AA2].turn = AA_REPOSE;
	V_AA[AA3].turn = AA_REPOSE;
	V_AA[AA4].turn = AA_REPOSE;
	if(sec_AA.SEC_CANT_AA == 1){
		V_AA[0].turn = 1;
	}else{
		for(uint8_t i = 0; i< (sec_AA.SEC_CANT_AA - 1); i++){
			V_AA[data[i] - 1].turn = AA_TURN;
		}
	}
	brigthC_SetReady(V_AA);
	return;
}

void brigthC_SetReady(AA_t *A){
    uint8_t i;
    for(i = 0; i<sec_AA.SEC_CANT_AA; i++){
        PCF2.digitalWrite(V_AA[i].p_s_bypass_pin, HIGH);
    }
    return;
}