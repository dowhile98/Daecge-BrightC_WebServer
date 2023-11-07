/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/*Includes ------------------------------------------------------------------------------*/
#include "main.h"

/*Defines ------------------------------------------------------------------------------*/

/*Global variables ---------------------------------------------------------------------*/

//->semaphores
SemaphoreHandle_t tempReadMutex;
SemaphoreHandle_t  shareDataMutex;
//->queues
QueueHandle_t lcd_queue = NULL;
QueueHandle_t buzer_queue = NULL;
//->task handles

//->events
EventGroupHandle_t secEvents;
EventGroupHandle_t controlEvents;
EventGroupHandle_t errorEvents;
EventGroupHandle_t systemEvents;
EventGroupHandle_t bypasState;
//->data
Measurements_t sensorData;
Input_val_t dataIn = {0};
/*Object instances --------------------------------------------------------------------*/
DallasTemperature DSB1;
DallasTemperature DSB2;
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
PCF8574 PCF2;  //A0=1, A1=0, A2=0 
PCF8574 PCF1;  //A0=0, A1=0, A2=0 
/*FUnction prototype ------------------------------------------------------------------*/
void webServer_Task_WiFi(void *params);

/*Setup---------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(webServer_Task_WiFi, "WiFi Server", 8000, NULL, 4, NULL, 1);
}

/*Loop----------------------------------------------------------------------------------*/
void loop() {
  // put your main code here, to run repeatedly:
}

/*Function definition ------------------------------------------------------------------*/