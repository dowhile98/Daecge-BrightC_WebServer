/**
 * @file project_typedfs.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __PROJECT_TYPEDEFS_H
#define __PROJECT_TYPEDEFS_H

/*Incluces ------------------------------------------------------------------------*/
#include "Arduino.h"

/*Defines ------------------------------------------------------------------------*/


/*Typedefs -----------------------------------------------------------------------*/
//ESTRUCTURA PARA ENVIAR COMANDOS A LA TAREA LCD
typedef struct{
  char line1[16];
  char line2[16];
  uint8_t view_time;
}LCD_queue_t;
//estructura para el sonido del buzzer
typedef struct{
  uint8_t repeats;
  uint16_t delay;
}buzzer_queue_t;

typedef struct{
    float TMP1;                 //DSB1
    float TMP2;                 //DSB2
    float HUMEDAD;              //HUM DHT22
    float TEMPERATURA;          //TEMP DH22
    float RMS_V;
    float PA;
    float PR;
    float PAP;
    float EA;
    float ER;
    float FP;
    float FREC; 
}Measurements_t;

typedef struct{
  bool in1 ;
  bool in2;
  bool in3 ;
  bool in4 ;
  bool in5 ;
  bool in6 ;
  bool in7 ;
  bool in8 ;
  bool A_Incendio ;
  bool S_BYPASSS_STS ;
  bool TMP_High;
}Input_val_t;

typedef struct{
    int SEC_CANT_AA;
    int SEC_DIAS;
    float SEC_TMP_MAX;
    float SEC_TMP_MIN;
    float SEC_TMP_ALARMA;
    float SEC_TMP_BYPASS;
    int TIM_ENFRIO_SP;          //tiempo en el que debe enfriars
    bool BYPASS_LOGICO;
    bool MANUAL;
}Sequence_AA_t;

typedef struct{
    bool turn;
    bool workig;
    uint8_t p_aa_pin;
    uint8_t p_s_bypass_pin;
    uint8_t failcount;
    bool helper;
}AA_t;

typedef struct {
  uint8_t command;
  const char *pData;
}SNMP_Cmd_t;

typedef struct{
    uint8_t SEC_CANT_AA;           //cantidad de AERES ACONDICIONADOS
    uint8_t SEC_DIAS;
    float SEC_TMP_MAX;
    float SEC_TMP_MIN;
    float SEC_TMP_ALARMA;
    float SEC_TMP_BYPASS;
    uint32_t TIM_ENFRIO_SP;          //tiempo en el que debe enfriarse
    char name[40];
    char ip[20];
    char gateway[20];
    char subnet[20];
    char ipserver[85];
}Config_t;
/*Enums ----------------------------------------------------------------------------------*/
typedef enum{
  CMD_NAME = 1,
  CMD_ID,
  CMD_IP,
  CMD_T_MAX,
  CMD_T_MIN,
  CMD_T_ALARMA,
  CMD_T_BP,
  CMD_SEC_DIAS,
  CMD_CANT_AA,
  CMD_MAXTIME_COOL_SP,
  CMD_SAMPLE_RATE,
  CMD_BYPASS_LOGICO,
  CMD_MANUAL,
  CMD_RST_ENERGY,
  CMD_MANUAL_AA1,
  CMD_MANUAL_AA2,
  CMD_MANUAL_AA3,
  CMD_MANUAL_AA4,
  CMD_RST_FAIL_COUNT,
  CMD_RST_Device
}CMD_CALLBACK_t;
#endif