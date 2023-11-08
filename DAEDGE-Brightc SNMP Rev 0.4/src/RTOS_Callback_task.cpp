/**
 * @file RTOS_Callback_task.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "main.h"
#include "SNMP_OID.h"

/*Global variables ---------------------------------------------*/
String chipid;

extern SNMP::Agent snmp;
extern MIB mib;
/*Function prototype -------------------------------------------*/

void turn_on_air_conditioner(void){
  uint8_t i;
  for(i = 0; i<sec_AA.SEC_CANT_AA;i++){
    if(V_AA[i].turn == true){
      PCF2.digitalWrite(V_AA[i].p_aa_pin, HIGH);
      V_AA[i].workig = true;
    }
  }
  return;
}

void turn_off_air_conditioner(void){
  uint8_t i;
  for(i = 0; i<4;i++){
    V_AA[i].workig = false;
    PCF2.digitalWrite(V_AA[i].p_aa_pin, LOW);;
    V_AA[i].workig = false;
  }
}

/*Task definition ----------------------------------------------*/
void callback_command_task(void *params){
  /*Local variables*/
  uint32_t len;
  LCD_queue_t xQueue_lcd;                             //para mostrar en la lcd
  bool backup_aa[4];
  buzzer_queue_t xQueue_buzer;
  uint8_t sec_flag = 0;
  SNMP_Cmd_t data;
  uint8_t match;
  String strval;
  uint8_t boolval;
  while(1){
    if(xQueueReceive(command_queue, (void *)&data, portMAX_DELAY) == pdPASS){
      xQueue_buzer.repeats = 1;
      xQueue_buzer.delay = 80;
      xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
      //se evalua el comando recibido
      match = data.command;
      SERIAL_PRINT("[TASK CALLBACK]->VALOR: ");
      SERIAL_PRINTLN(match);
      switch (match)
      {
      case CMD_NAME:
        strcpy(config.name, data.pData);
        SPIFFS_saveConfiguration(filename, &config);
        copy_data_config(sec_AA, config);
        //lcd
        sprintf(xQueue_lcd.line1,"NOMBRE CAMBIADO");
        strcpy(xQueue_lcd.line2,config.name);
        xQueue_lcd.view_time = 2;
        xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        break;
      case CMD_ID:  
        chipid = String(data.pData);
        //lcd
        sprintf(xQueue_lcd.line1,"ID CAMBIADO");
        strcpy(xQueue_lcd.line2,chipid.c_str());
        xQueue_lcd.view_time = 2;
        xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        break;
      case CMD_IP:
        strval = String(data.pData);
        if(net.ip.fromString(strval)){
          SERIAL_PRINT("[TASK CALLBACK]->IP CAMBIADA: ");
          SERIAL_PRINTLN(strval);
          strcpy(config.ip, strval.c_str());
          SPIFFS_saveConfiguration(filename, &config);
          copy_data_config(sec_AA, config);
          //delat
          vTaskDelay(pdMS_TO_TICKS(1000));
          ethernet_connect(mac, net.ip, net.DNS, net.GATEWAY, net.SUBNET);
          snmp.begin(&udp);
          vTaskDelay(pdMS_TO_TICKS(5000));
          //lcd
          sprintf(xQueue_lcd.line1, "IPDISP CAMBIADA");
          strcpy(xQueue_lcd.line2, strval.c_str());
          xQueue_lcd.view_time = 2;
          xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        }else{
          SERIAL_PRINT("[TASK CALLBACK]->IP NO VALIDA");
        }
        break;
      case CMD_T_MAX:
        config.SEC_TMP_MAX = atof(data.pData);
        SPIFFS_saveConfiguration(filename, &config);
        copy_data_config(sec_AA, config);
        //lcd
        sprintf(xQueue_lcd.line1, "T MAX CAMBIADO");
        strcpy(xQueue_lcd.line2, data.pData);
        xQueue_lcd.view_time = 2;
        xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        break;
      case CMD_T_MIN:
        config.SEC_TMP_MIN = atof(data.pData);
        SPIFFS_saveConfiguration(filename, &config);
        copy_data_config(sec_AA, config);
        //lcd
        sprintf(xQueue_lcd.line1, "T MIN CAMBIADO");
        strcpy(xQueue_lcd.line2, data.pData);
        xQueue_lcd.view_time = 2;
        xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        break;
      case CMD_T_ALARMA:
        config.SEC_TMP_ALARMA = atof(data.pData);
        SPIFFS_saveConfiguration(filename, &config);
        copy_data_config(sec_AA, config);
        //lcd
        sprintf(xQueue_lcd.line1, "T ALARMA CAMBIADO");
        strcpy(xQueue_lcd.line2, data.pData);
        xQueue_lcd.view_time = 2;
        xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        break;
      case CMD_T_BP:
        config.SEC_TMP_BYPASS = atof(data.pData);
        SPIFFS_saveConfiguration(filename, &config);
        copy_data_config(sec_AA, config);
        //lcd
        sprintf(xQueue_lcd.line1, "T BYPASS SET");
        strcpy(xQueue_lcd.line2, data.pData);
        xQueue_lcd.view_time = 2;
        xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        break;
      case CMD_SEC_DIAS:
        config.SEC_DIAS = *((uint8_t*)data.pData);
        SERIAL_PRINT("DIAS DE SECUENCIA:");
        SERIAL_PRINTLN(config.SEC_DIAS);
        SPIFFS_saveConfiguration(filename, &config);
        copy_data_config(sec_AA, config);
        //sound
        delay(1000);
        // xQueue_buzer.repeats = sec_AA.SEC_DIAS;
        // xQueue_buzer.delay = 50;
        // xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
        break;
      case CMD_CANT_AA:
        config.SEC_CANT_AA = *((uint8_t*)data.pData);
        SERIAL_PRINT("CANTIDAD DE AA:");
        SERIAL_PRINTLN(config.SEC_CANT_AA);
        SPIFFS_saveConfiguration(filename, &config);
        copy_data_config(sec_AA, config);
        
        /*se reinicia los turnos y la bandera de secuencia*/
        xEventGroupSetBits(secEvents, BRIGTHC_CHANGE_NUMBER_AA);
        vTaskDelay(pdMS_TO_TICKS(50));
        if(is_controlling == true){
          turn_on_air_conditioner();
        }
        //sound
        break;
      case CMD_MAXTIME_COOL_SP:
        SERIAL_PRINT("TIEMPO ENFRIO CAMBIADO:");
        config.TIM_ENFRIO_SP = *((uint32_t*)data.pData);
        SERIAL_PRINTLN(config.TIM_ENFRIO_SP);
        SPIFFS_saveConfiguration(filename, &config);
        copy_data_config(sec_AA, config);           //se almacena el valor
        break;
      case CMD_SAMPLE_RATE:
        //todo

        break;
      case CMD_BYPASS_LOGICO:
        SERIAL_PRINT("BYPASS LOGICO:");
        boolval = *((uint8_t*)data.pData);
        SERIAL_PRINTLN(boolval);
        if((boolval == 1) && (dataIn.S_BYPASSS_STS == false)){
          sec_AA.BYPASS_LOGICO = true;
          SERIAL_PRINTLN("[TASK CALLBACK]->Entrando a modo bypass logido");
          //se envia el comando
          xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_LOG);
          
        }else if((boolval == 0) && (dataIn.S_BYPASSS_STS == true)){
          SERIAL_PRINTLN("[TASK CALLBACK]->Saliendo del modo bypass logido");
          sec_AA.BYPASS_LOGICO = false;
          //se envia el comando
          xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_EXIT);
        }
        break;
      case CMD_MANUAL:
        boolval = *((uint8_t*)data.pData);
        if((boolval == 1) && (sec_AA.MANUAL == false)){  //modo manual
          SERIAL_PRINTLN("[TASK CALLBACK]->Entrando al modo manual");
          sec_AA.MANUAL = true;                         //se establece la bandera de manual
          if(dataIn.S_BYPASSS_STS == 1){
            sec_AA.BYPASS_LOGICO = false;
            xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_EXIT);
            vTaskDelay(pdMS_TO_TICKS(1));
            vTaskSuspend(bypass_handle);
          }
          vTaskSuspend(ctrl_handle);
          vTaskSuspend(bypass_handle);
          //se verifica si se estaba controlando
          if(is_controlling == true){
            turn_off_air_conditioner();       //se apaga los AA
          }
          for(uint8_t i = 0;i<sec_AA.SEC_CANT_AA; i++){
            V_AA[i].turn = true;
          }
          
          //lcd
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "  PRUEBA DE AA");
          xQueue_lcd.view_time = 3;
          xQueueSend(lcd_queue, (void *) &xQueue_lcd, 100);
        }else if((boolval == 0) && (sec_AA.MANUAL == true)){
          SERIAL_PRINTLN("[TASK CALLBACK]->Saliendo del modo manual");
          //se sale del modo manual
          sec_AA.MANUAL = false;
          //se restuara el turno anterior
          //se envia el comando para reiniciar la secuencia
          xEventGroupSetBits(secEvents, BRIGTHC_CHANGE_NUMBER_AA);
          //se apagan los AA
          turn_off_air_conditioner();
          sprintf(xQueue_lcd.line1, "MODO INTELIGENTE");
          sprintf(xQueue_lcd.line2, "PRUEBA TERMINADA");
          xQueue_lcd.view_time = 2;
          xQueueSend(lcd_queue, (void *) &xQueue_lcd, 100);
          //se verifica si antes de entrar al modo manual se estaba contralando
          if(is_controlling == true){
            turn_on_air_conditioner();
          }
          //se restaura el funcionamiento de las tareas
          vTaskResume(ctrl_handle);
          vTaskResume(bypass_handle);
        }
        break;
      case CMD_RST_ENERGY:
        //RESET ENERGY COUNTER PZEM
        SERIAL_PRINTLN("[TASK CALLBACK]->RESETEANDO CONTADOR DE ENERGY");
        led_blink(LED, 3, 50);
        //todo
        pmeter1.resetEnergy();
        pmeter2.resetEnergy();
        //LCD
        sprintf(xQueue_lcd.line1, " PZEM-004T-v30");
        sprintf(xQueue_lcd.line2, "   RESETEADO");
        xQueueSend(lcd_queue, (void *)&xQueue_lcd, 100);
        break;
      case CMD_MANUAL_AA1:
        boolval = *((uint8_t*)data.pData);
        if((boolval == 0) && (sec_AA.MANUAL == true)){
          PCF2.digitalWrite(P_AA1, LOW);
          V_AA[0].workig = false;
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "AA1 APAGADO");
          xQueue_lcd.view_time = 2;
        }else if((boolval == 1) && (sec_AA.MANUAL == true)){
          PCF2.digitalWrite(P_AA1, HIGH);
          V_AA[0].workig = true;
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "AA1 ENCENDIDO");
          xQueue_lcd.view_time = 2;
        } 
        //lcd
        xQueueSend(lcd_queue, (void *) &xQueue_lcd, 100);
        break;
      case CMD_MANUAL_AA2:
        boolval = *((uint8_t*)data.pData);
        if((boolval == 0) && (sec_AA.MANUAL == true)){
          PCF2.digitalWrite(P_AA2, LOW);
          V_AA[1].workig = false;
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "AA2 APAGADO");
          xQueue_lcd.view_time = 2;
        }else if((boolval == 1) && (sec_AA.MANUAL == true)){
          PCF2.digitalWrite(P_AA2, HIGH);
          V_AA[1].workig = true;
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "AA2 ENCENDIDO");
          xQueue_lcd.view_time = 2;
        } 
        //lcd
        xQueueSend(lcd_queue, (void *) &xQueue_lcd, 100);
        break;
      case CMD_MANUAL_AA3:
        boolval = *((uint8_t*)data.pData);
        if((boolval == 0) && (sec_AA.MANUAL == true)){
          PCF2.digitalWrite(P_AA3, LOW);
          V_AA[2].workig = false;
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "AA3 APAGADO");
          xQueue_lcd.view_time = 2;
        }else if((boolval == 1) && (sec_AA.MANUAL == true)){
          PCF2.digitalWrite(P_AA3, HIGH);
          V_AA[2].workig = true;
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "AA3 ENCENDIDO");
          xQueue_lcd.view_time = 2;
        } 
        //lcd
        xQueueSend(lcd_queue, (void *) &xQueue_lcd, 100);
        break;
      case CMD_MANUAL_AA4:
        boolval = *((uint8_t*)data.pData);
        if((boolval == 0) && (sec_AA.MANUAL == true)){
          PCF2.digitalWrite(P_AA4, LOW);
          V_AA[3].workig = false;
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "AA4 APAGADO");
          xQueue_lcd.view_time = 2;
        }else if((boolval == 1) && (sec_AA.MANUAL == true)){
          PCF2.digitalWrite(P_AA4, HIGH);
          V_AA[3].workig = true;
          sprintf(xQueue_lcd.line1, "--MODO MANUAL--");
          sprintf(xQueue_lcd.line2, "AA4 ENCENDIDO");
          xQueue_lcd.view_time = 2;
        } 
        //lcd
        xQueueSend(lcd_queue, (void *) &xQueue_lcd, 100);
        break;
      case CMD_RST_FAIL_COUNT:
        boolval = *((uint8_t*)data.pData);
        if(boolval == 1){
          SERIAL_PRINTLN("[TASK CALLBACK]->REINICIANDO CONTADOR DE FALLAS");
          for(uint i = 0; i<4; i++){
            V_AA[i].failcount = 0;
          }
          sprintf(xQueue_lcd.line1, "--REINICIO--");
          sprintf(xQueue_lcd.line2, "CONTADOR");
          xQueue_lcd.view_time = 2;
          xQueueSend(lcd_queue, (void *)&xQueue_lcd, 1000);
        }
        break;
      case CMD_RST_Device:
        boolval = *((uint8_t*)data.pData);
        if(boolval == 1){
          SERIAL_PRINTLN("[TASK CALLBACK]->RESETEANDO...");
          xQueue_buzer.repeats = 2;
          xQueue_buzer.delay = 500;
          xQueueSend(buzer_queue, (void *)&xQueue_buzer, 30);
          delay(2000);
          ESP.restart();
        }
        break;
      default:
        break;
      }
      mib.setAAState(V_AA);
    }
  }
}