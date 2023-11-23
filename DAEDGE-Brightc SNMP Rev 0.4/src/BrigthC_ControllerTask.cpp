/**
 * @file BrigthC_ControllerTask.cpp
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
/*Typedef --------------------------------------------------------------------------*/
typedef enum{
	TASK_CTRL_FREE,
	TASK_CTRL_WORKING,
	TASK_CTRL_WORKING_FAIL,
	TASK_CTRL_BYPASS,
	TASK_CTRL_NO_ACTION
}TaskCtrl_State_t;
/*Global variables -----------------------------------------------------------------*/
extern AA_t V_AA[4];
bool is_controlling = false;
uint8_t helper_aa = 0;
extern MIB mib;
/*Function prototype ---------------------------------------------------------------*/
uint8_t tempConfVerify(float tempMin, float tempMax, float tempAlarm, float tempBypass);
bool failCountVerify(AA_t *AA, uint8_t numAirs, uint8_t failCountLimit);
void BrigthC_TurnOnAA(AA_t *AA);
void BrightC_TurnOffAA(AA_t *AA);
void BrightC_TurnOnBackupAA(AA_t *AA);
void BrigthtC_UpdateFailCount(AA_t *AA);
/*Task definition ------------------------------------------------------------------*/
void Controller_Task(void * argument){
	/*Local variables ---------------------------------------------------*/
	uint32_t controllingCurrentTime = 0;
	uint8_t taskState = TASK_CTRL_FREE;
	EventBits_t controlFlags = 0;
    LCD_queue_t xQueue_lcd;
    buzzer_queue_t xQueue_buzer;
    uint8_t failRepeat = 0;
	/*Init squeuence ---------------------------------------------------*/
    V_AA[AA1].workig = AA_OFF; V_AA[AA2].workig = AA_OFF; V_AA[AA3].workig = AA_OFF; V_AA[AA4].workig = AA_OFF;
	V_AA[AA1].failcount = 0; V_AA[AA2].failcount = 0; V_AA[AA3].failcount = 0; V_AA[AA4].failcount = 0;
    V_AA[0].helper = false; V_AA[1].helper = false; V_AA[2].helper = false; V_AA[3].helper = false;
    //se espera la bandera de ready de secuencia
	printf("[CTRL]: Iniciando.....");
	xEventGroupWaitBits(secEvents, BRIGTHC_SEC_READY, pdTRUE, pdTRUE, portMAX_DELAY);

    for(;;){
        //se verifica los rangos de temperatura
		if(tempConfVerify(sec_AA.SEC_TMP_MIN, sec_AA.SEC_TMP_MAX, sec_AA.SEC_TMP_ALARMA, sec_AA.SEC_TMP_BYPASS) == 0){
            //buzzer
			Serial.printf("[CTRL TASK] Error de configuracion\r\n");
            xQueue_buzer.repeats = 1;
            xQueue_buzer.delay = 1000;
            xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
            //LCD
            sprintf(xQueue_lcd.line1, "...BrightC...");
            sprintf(xQueue_lcd.line2, "ERR! CONFIG");
            xQueue_lcd.view_time = 2;
            xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
            //error event
			xEventGroupSetBits(errorEvents, BRIGTHC_CTRL_CONF_T_ERR);
			xEventGroupWaitBits(controlEvents, BRIGTHC_CTRL_CONF_OK, pdTRUE, pdTRUE, portMAX_DELAY);
		}
		//se verifica la configuracion del tiempo de enfriamiento
		if(sec_AA.TIM_ENFRIO_SP == 0){
			Serial.println("[CTRL TASK] Tiempo de enfriado no valido\r\n");
            //buzzer
            xQueue_buzer.repeats = 1;
            xQueue_buzer.delay = 1000;
            xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
            //LCD
            sprintf(xQueue_lcd.line1, "...BrightC...");
            sprintf(xQueue_lcd.line2, "ERR! CONFIG");
            xQueue_lcd.view_time = 2;
            xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
            //error event
			xEventGroupSetBits(errorEvents, BRIGTHC_CTRL_CONF_TSP_ERR);
			xEventGroupWaitBits(controlEvents, BRIGTHC_CTRL_CONF_OK, pdTRUE, pdTRUE, portMAX_DELAY);
		}
		//se leen los eventos
		controlFlags = xEventGroupWaitBits(systemEvents,
				BRIGTHC_STATE_BYPASS | BRIGTHC_STATE_MANUAL,
				pdFALSE, pdFALSE, 800);
		if(controlFlags & BRIGTHC_STATE_BYPASS ){
			Serial.printf("[CTRL TASK] Dispositivo en modo bypass\r\n");
			vTaskDelay(pdTICKS_TO_MS(10));
			xEventGroupWaitBits(systemEvents,
							BRIGTHC_STATE_RETURN_CTRL,
							pdTRUE, pdTRUE, portMAX_DELAY);
			taskState = TASK_CTRL_FREE;
		}
		if(controlFlags & BRIGTHC_STATE_MANUAL){

			Serial.printf("[CTRL TASK] Dispositivo en modo manual\r\n");
			xEventGroupWaitBits(systemEvents,
										BRIGTHC_STATE_RETURN_CTRL,
										pdTRUE, pdTRUE, portMAX_DELAY);
			taskState = TASK_CTRL_FREE;
		}
		//->maquina de estados
		switch(taskState){
		case TASK_CTRL_FREE:
			//todo se espera la bandera de lectura de datos de temperatura

			xSemaphoreTake(shareDataMutex, portMAX_DELAY);
			if((sensorData.TMP1 > (sec_AA.SEC_TMP_MAX + 0.2)) || (sensorData.TMP2 > (sec_AA.SEC_TMP_MAX + 0.2))){
                is_controlling = true;
				//se verifica el contador de fallas
				if(failCountVerify(V_AA, sec_AA.SEC_CANT_AA, 3) == false){
					Serial.printf("[CTRL TASK] Iniciando control\r\n");
					taskState = TASK_CTRL_WORKING;
					controllingCurrentTime = millis();
                    //LCD
                    sprintf(xQueue_lcd.line1, "...BrightC...");
                    sprintf(xQueue_lcd.line2, "CONTROLANDO...");
                    xQueue_lcd.view_time = 2;
                    xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
					//se encienden los AA
					BrigthC_TurnOnAA(V_AA);
                    mib.setAAState(V_AA);
					//se establece el evento de modo control
					xEventGroupSetBits(systemEvents, BRIGTHC_STATE_CTRL);
				}else{
                    //error en contador de fallas
                    failRepeat += 1;
                    if(failRepeat >= 4){
                        Serial.println("[CTRL TASK] Error! pasando a bypass");
                        sprintf(xQueue_lcd.line1, "ERR! Reiniciar");
                        sprintf(xQueue_lcd.line2, "Contador...");
                        xQueue_lcd.view_time = 2;
                        xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
                        xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_EXT);
                        failRepeat = 0;
                    }else{
                        //se establece el cambio de turno
                        Serial.printf("[CTRL TASK] Error! contador de fallas, realizando cambio de turno\r\n");
                        xEventGroupSetBits(secEvents, BRIGTHC_CHANGE_BY_FAIL_AA);
                        vTaskDelay(1);
                        //LCD
                        sprintf(xQueue_lcd.line1, "ERR! AA");
                        sprintf(xQueue_lcd.line2, "Cambio de turno");
                        xQueue_lcd.view_time = 2;
                        xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
                    }
					
				}
			}else{
                is_controlling = false;
				dataIn.TMP_High = false;
				BrightC_TurnOffAA(V_AA);
            }
			xSemaphoreGive(shareDataMutex);
			break;
		case TASK_CTRL_WORKING:
			xSemaphoreTake(shareDataMutex, portMAX_DELAY);
			if((sensorData.TMP1 >= sec_AA.SEC_TMP_ALARMA) || (sensorData.TMP2 >= sec_AA.SEC_TMP_ALARMA)){
				dataIn.TMP_High = true;
				Serial.printf("[CTRL TASK] error, alarma de alta temperatura\r\n");
				//se enciende el aire acondicionado de respaldo
                BrigthtC_UpdateFailCount(V_AA);
				BrightC_TurnOnBackupAA(V_AA);
                mib.setAAState(V_AA);
                //LCD
                sprintf(xQueue_lcd.line1, "ERR! Encendiendo");
                sprintf(xQueue_lcd.line2, "Respaldo");
                xQueue_lcd.view_time = 2;
                xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
				//se pasa al siguiente estado
				controllingCurrentTime = millis();
				taskState = TASK_CTRL_WORKING_FAIL;
				xSemaphoreGive(shareDataMutex);
				break;
			}
			else if((sensorData.TMP1 <= sec_AA.SEC_TMP_MIN) && (sensorData.TMP2 <= sec_AA.SEC_TMP_MIN)){
                is_controlling = false;
				dataIn.TMP_High = false;
				printf("[CTRL TASK] Se logra conrolar la temperatura\r\n");
				//se apaga los aires AA
				BrightC_TurnOffAA(V_AA);
                mib.setAAState(V_AA);
                //LCD
                sprintf(xQueue_lcd.line1, "Temperatura");
                sprintf(xQueue_lcd.line2, "controlada");
                xQueue_lcd.view_time = 2;
                xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
				//se sale del modo de control
				taskState = TASK_CTRL_FREE;
				xSemaphoreGive(shareDataMutex);
				break;
			}
			//se verifica el tiempo
			if((millis() - controllingCurrentTime)>= (sec_AA.TIM_ENFRIO_SP * 1000 * 60)){
                Serial.printf("[CTRL TASK] error, alarma de alta temperatura\r\n");
				dataIn.TMP_High = true;
                BrigthtC_UpdateFailCount(V_AA);
				BrightC_TurnOnBackupAA(V_AA);
                mib.setAAState(V_AA);
                //LCD
                sprintf(xQueue_lcd.line1, "ERR! Encendiendo");
                sprintf(xQueue_lcd.line2, "Respaldo");
                xQueue_lcd.view_time = 2;
                xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
                //change state
				controllingCurrentTime = millis();
				taskState = TASK_CTRL_WORKING_FAIL;
			}
			xSemaphoreGive(shareDataMutex);
			break;
		case TASK_CTRL_WORKING_FAIL:
			xSemaphoreTake(shareDataMutex, portMAX_DELAY);
			if((sensorData.TMP1 >= sec_AA.SEC_TMP_BYPASS) || (sensorData.TMP2 >= sec_AA.SEC_TMP_BYPASS)){
                //se apagan los AA
				BrightC_TurnOffAA(V_AA);
                mib.setAAState(V_AA);
                //se pasa al modo bypas por falla de control de temperatura
				xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_AA);
				taskState = TASK_CTRL_NO_ACTION;
			}else if((sensorData.TMP1 <= sec_AA.SEC_TMP_MIN) && (sensorData.TMP2 <= sec_AA.SEC_TMP_MIN)){
				dataIn.TMP_High = true;
                is_controlling = false;
				printf("[CTRL TASK`] Se logra controlar la temperatura\r\n");
				//se apaga los AA
				BrightC_TurnOffAA(V_AA);
                mib.setAAState(V_AA);
                //LCD
                sprintf(xQueue_lcd.line1, "Temperatura");
                sprintf(xQueue_lcd.line2, "controlada");
                xQueue_lcd.view_time = 2;
                xQueueSend(lcd_queue, (void *)& xQueue_lcd, 100);
				//se sale del modo de control
				taskState = TASK_CTRL_FREE;
			}

			if((millis() - controllingCurrentTime)>= (sec_AA.TIM_ENFRIO_SP * 1000 * 60)){
				//Se apagan los AA
				BrightC_TurnOffAA(V_AA);
                mib.setAAState(V_AA);
				//se pasa a bypass
				xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_AA);
				taskState = TASK_CTRL_NO_ACTION;
			}
			xSemaphoreGive(shareDataMutex);
			break;
		case TASK_CTRL_NO_ACTION:
			
			break;
		}
		vTaskDelay(pdMS_TO_TICKS(100));
    }
}
/*Function definition --------------------------------------------------------------*/
uint8_t tempConfVerify(float tempMin, float tempMax, float tempAlarm, float tempBypass){
	if((tempMin < tempMax) && (tempMax < tempAlarm) && (tempAlarm < tempBypass)){
		return 1;
	}
	return 0;
}

bool failCountVerify(AA_t *AA, uint8_t numAirs, uint8_t failCountLimit){
	uint8_t count = 0;
	for(uint8_t i = 0; i<numAirs; i++){
		Serial.printf("AA%d_F:%d ", i, V_AA[i].failcount);
		if(AA[i].turn == AA_TURN){
			if (AA[i].failcount > failCountLimit){
				count += 1;
			}
		}
	}

	Serial.printf(" Fail result: %d\r\n", (count > (numAirs / 2)));
	if(numAirs == 2){
		//return (count >= (numAirs / 2));
		return false;
	}
	return (count > (numAirs / 2));
}

void BrigthC_TurnOnAA(AA_t *AA){
	uint8_t i;
	uint8_t cantAA = sec_AA.SEC_CANT_AA;
	Serial.print("[CTRL TASK] ");
	for(i = 0; i<cantAA; i++){
		if(AA[i].turn == AA_TURN){
			AA[i].workig = 1;
			//Todo se encenciende el AA
			Serial.printf("AA%d Encendido ", i);
            PCF2.digitalWrite(V_AA[i].p_aa_pin, HIGH);
		}
	}
	printf("\r\n");
}
void BrightC_TurnOffAA(AA_t *AA){
	uint8_t i;
	uint8_t cantAAMax = 4;
	Serial.print("[CTRL TASK] ");
	for(i = 0; i < cantAAMax; i++){
		AA[i].workig = 0;
		AA[i].helper = 0;
		//Todo se encenciende el AA
		printf("AA%d Apagado ", i);
        PCF2.digitalWrite(V_AA[i].p_aa_pin, LOW);
	}
	printf("\r\n");
}
void BrightC_TurnOnBackupAA(AA_t *AA){
	uint8_t i;
	uint8_t cantAA = sec_AA.SEC_CANT_AA;
	Serial.print("[CTRL TASK] ");
	for(i = 0; i<cantAA; i++){
		if(AA[i].turn == 0){
			AA[i].helper = 1;
			printf("AA%d encendido como respaldo\r\n", i);
			AA[i].workig = 1;
			//todo se enciende el AA de respaldo
            PCF2.digitalWrite(V_AA[i].p_aa_pin, HIGH);
			break;
		}
	}
}

void BrigthtC_UpdateFailCount(AA_t *AA){
    uint8_t i;
	uint8_t cantAA = sec_AA.SEC_CANT_AA;
    for(uint8_t i = 0; i<cantAA; i++){
        if(AA[i].turn == 1){
            AA[i].failcount += 1;
        }
    }
}