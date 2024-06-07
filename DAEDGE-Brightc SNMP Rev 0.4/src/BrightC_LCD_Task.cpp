/**
 * @file RTOS_LCD_Task.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "main.h"


/*Global variables -----------------------------------------------------------*/

uint32_t update_lcd_time = 2;               //en segundos
uint8_t turn_aa_off = 0;
/*Task function --------------------------------------------------------------*/

void lcd_task(void *params){
  //lacal variables
  Sequence_AA_t xSec;
  LCD_queue_t xQueue;                     //para recibir si se quiere mostrar algo en la pantalla
  uint8_t count = 0;
  uint32_t deInitCount = 0;
  while(1){
    //verificar si se quiere mostrar algo
    while(xQueueReceive(lcd_queue, (void *)&xQueue, 0) == pdTRUE){
      lcd.clear();
      lcd.print(xQueue.line1);
      lcd.setCursor(0, 1);
      lcd.print(xQueue.line2);
      delay(xQueue.view_time * 1000);
    }
    //hacer una copia
    xSec = sec_AA;
    //cantidad de AA
    switch (count)
    {
      case 1:                     //se muestrea el nombre
        lcd.clear();
        lcd.print("...BrightC...");
        lcd.setCursor(0, 1);
        lcd.print(config.name);
        break;
      case 2:                     //se muestra la cantidad de AA y dias de secuencia
        lcd.clear();
        lcd.print("CANT. DE AA: ");
        lcd.print(xSec.SEC_CANT_AA);
        lcd.setCursor(0, 1);
        lcd.print("DIAS DE SEC: ");
        lcd.print(xSec.SEC_DIAS);
        break;
      case 3:                   //Temperatura maxima y minima configurada
        lcd.clear();
        lcd.print("RANGO DE TEMP.");
        lcd.setCursor(0, 1);
        lcd.print("TMIN:");
        lcd.print((int)xSec.SEC_TMP_MIN);
        lcd.print(" TMAX:");
        lcd.print((int)xSec.SEC_TMP_MAX);
        break;
      case 4:                   //Temperatura maxima y minima configurada
        lcd.clear();
        lcd.print("TEMP ALARMA:");
        lcd.print((int)xSec.SEC_TMP_ALARMA);
        lcd.setCursor(0, 1);
        lcd.print("TEMP BYPAS:");
        lcd.print((int)xSec.SEC_TMP_BYPASS);
        break;
      case 5:                   //tiempo para enfriar la sala
        lcd.clear();
        lcd.print("--MAX COOL TIME-");
        lcd.setCursor(0, 1);
        lcd.print(xSec.TIM_ENFRIO_SP);
        lcd.print(" minutos");
        break;
        
      case 6:                   //temperatura
        lcd.clear();
        lcd.print("TMP1: ");
        lcd.printf("%.1f C",sensorData.TMP1);
        lcd.setCursor(0, 1);
        lcd.print("TMP2: ");
        lcd.printf("%.1f C",sensorData.TMP2);
        break;
      case 7:                   //temperatura
        lcd.clear();
        lcd.print("IP: "); 
        lcd.setCursor(0, 1);
        lcd.print(net.ip);
        break;
      case 8:                   //temperatura
        lcd.clear();
        lcd.print("GATEWAY: "); 
        lcd.setCursor(0, 1);
        lcd.print(net.GATEWAY);
        break;
      case 9:                   //temperatura
        lcd.clear();
        lcd.print("SUBNET: "); 
        lcd.setCursor(0, 1);
        lcd.print(net.SUBNET);
        break;
      case 10:                   //temperatura
        lcd.clear();
        lcd.print("SNMP MANAGER"); 
        lcd.setCursor(0, 1);
        lcd.print(net.IPSERVER);
        break;
      // case 7:                   //humedad
      //   lcd.clear();
      //   lcd.print("----HUMEDAD----");
      //   lcd.setCursor(5, 1);
      //   lcd.print(sensorData.HUMEDAD);
      //   lcd.print(" %");
      //   break;  
      default:
        count = 0;
        break;
    }
    count++;
    delay(update_lcd_time * 1000);
    deInitCount += 1;
    if(deInitCount >= 50){
      deInitCount = 0;
      lcd.init();
      lcd.clear();
      lcd.backlight();
    }
  }
}