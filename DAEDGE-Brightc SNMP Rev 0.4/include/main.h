/**
 * @file main.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __MAIN_H
#define __MAIN_H
/*Includes ---------------------------------------------------------------------------------*/
#include "project_typedfs.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <mat.h>
#include <PZEM004Tv30.h>
#include <stdio.h>
#ifdef ESP32
  #include <SPIFFS.h>
#endif
#include <ESP32Time.h>                      //RTC
#include <PCF8574.h>                        //extensor de pines I2C
#include <LiquidCrystal_I2C.h>              //Complete project details at https://randomnerdtutorials.com 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BrigthC_App.h>
/*Defines ----------------------------------------------------------------------------------*/
#define __DEBUG_SESRIAL
/**
 * @brief Para realizar pruebas de la logica de la secuencia
 * 0: funcionamiento normal
 * 1: Funcionamiento de prueba
 */
//#define __TEST__            1
/**
 * @brief para realizar pruebas con el sensor PZEM
 * 0: uso del senor
 * 1: uso de datos aleatoreas
 */
#define __TEST_PZEM__       0
/*TIME STAMP*/
#define TIME_STAMP_ONE_DAY  86400
/*SERIAL*/
#define DEBUG_SERIAL        Serial

#ifdef  __DEBUG_SESRIAL
  #define SERIAL_PRINT(...)   {DEBUG_SERIAL.print(__VA_ARGS__);}
  #define SERIAL_PRINTLN(...) {DEBUG_SERIAL.println(__VA_ARGS__); }
#else
  #define SERIAL_PRINT(...)   {}
  #define SERIAL_PRINTLN(...) {}
#endif

#if (__TEST__)
  #undef  TIME_STAMP_ONE_DAY
  #define TIME_STAMP_ONE_DAY  120
#endif
/**
 * @brief si en caso se quiere mostrar los topicos en la pantall LCD
 * Comentar en caso no se quiera
 */
//#define __LCD_TOPIC_SHOW

/**
 * @brief numero maximo y minimo de dias para la secuencia
 */
#define MIN_NUMBERS_DAYS_SEC  7
#define MAX_NUMBERS_DAYS_SEC  30

/**
 * @brief tiempos para escaneo de sensores y entradas
 * @note definir estos tiempos en segundos (esto define la frecuencia con la que se escanean)
 */
#define INPUT_TIME_READ     2
#define TEMP_SENSORS_READ   1
/**
 * @brief direcciones de los dispositivos I2C conectados 
 */
#define PCF1_ADDR           0x26
#define PCF2_ADDR           0x21
#define LCD_ADDR            0x27
/*ETHERNET PIN*/
#define     ETH_RST         4
#define     ETH_CS          5
#define     ETH_SCLK       18
#define     ETH_MISO       23
#define     ETH_MOSI       19
#define     LED             2
#define     BTN_CNFIG_1     35
//I2C
#define     I2C_SDA_PIN     21
#define     I2C_SCL_PIN     22
//PZEM PINOUT

#define     PZEM_RX_PIN_PM1 16
#define     PZEM_TX_PIN_PM1 17
#define     PZEM_RX_PIN_PM2 25
#define     PZEM_TX_PIN_PM2 26

#if !defined(PZEM_SERIAL_1)
#define PZEM_SERIAL_PM1 Serial2             // Power Meter 1 (PM1) on Serial 2
#endif
#if !defined(PZEM_SERIAL_2)
#define PZEM_SERIAL_PM2 Serial1             // Power Meter 2 (PM2) on Serial 1
#endif
//->Definicion Pines Entrada
#define     P_AAA1_AS       0 //alarma aire acondicionado 1, Alarma de suministro
#define     P_AAA1_AABP     1 //alarma aire acondicionado 1, Alarma de alta y baja presiÃƒÂ³n
#define     P_AAA2_AS       2 //alarma aire acondicionado 2, Alarma de suministro
#define     P_AAA2_AABP     3 //alarma aire acondicionado 2, Alarma de alta y baja presiÃƒÂ³n
#define     P_AAA3_AS       4 //alarma aire acondicionado 3, Alarma de suministro
#define     P_AAA3_AABP     5 //alarma aire acondicionado 3, Alarma de alta y baja presiÃƒÂ³n
#define     P_AAA4_AS       6 //alarma aire acondicionado 4, Alarma de suministro
#define     P_AAA4_AABP     7 //alarma aire acondicionado 4, Alarma de alta y baja presiÃƒÂ³n
#define     P_A_Incendio    34 //Alarma de incendio (NC)
#define     P_BTN_CNFIG_1   33 //Boton de cofiguracion 1
#define     P_DS18B20_1     13 //Sensor de temperatura DSB18B20 NÃ‚Â°1
#define     P_DS18B20_2     32 //Sensor de temperatura DSB18B20 NÃ‚Â°2
//->Definicion pines de salida
#define     P_AA1           0  //p0 PCF 2 -Salida para activar Aire acondicionado 1
#define     P_AA2           1  //p1 PCF 2 - Salida para activar Aire acondicionado 2
#define     P_AA3           2  //p2 PCF 2 - Salida para activar Aire acondicionado 3
#define     P_AA4           3  //p3 PCF 2 - Salida para activar Aire acondicionado 4
#define     P_S_BYPASS1     4  //Salida bypas AA1
#define     P_S_BYPASS2     5  //Salida bypas AA2
#define     P_S_BYPASS3     6  //Salida bypas AA3
#define     P_S_BYPASS4     7  //Salida bypas AA4
#define     P_S_BYPASS_sts  15 //Salida para bypass para para cliente

#define     BUZER           27

/*Extern global variables ------------------------------------------------------------*/
extern Measurements_t sensorData;
extern Input_val_t dataIn;
/*Extern RTOS definition -------------------------------------------------------------*/
extern SemaphoreHandle_t tempReadMutex;

extern QueueHandle_t lcd_queue;
extern QueueHandle_t buzer_queue;

extern SemaphoreHandle_t  shareDataMutex;

extern EventGroupHandle_t secEvents;
extern EventGroupHandle_t controlEvents;
extern EventGroupHandle_t errorEvents;
extern EventGroupHandle_t systemEvents;
extern EventGroupHandle_t bypasState;
/*Extern object instances ------------------------------------------------------------*/
extern DallasTemperature DSB1;
extern DallasTemperature DSB2;
extern LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
extern PCF8574 PCF2;  //A0=1, A1=0, A2=0 
extern PCF8574 PCF1;  //A0=0, A1=0, A2=0 
/*Function api prototype -------------------------------------------------------------*/
void led_blink(uint16_t led_pin,uint8_t blink_num, uint64_t delayBlink);
void ethernetReset(const uint8_t resetPin);
void temp_sensors_read(void);
void ethernet_connect(uint8_t *mac, IPAddress _ip, IPAddress _dns, IPAddress _gateway, IPAddress _subnet);
void PCF_Pin_Init(void);
void digital_pin_config(void);
#endif