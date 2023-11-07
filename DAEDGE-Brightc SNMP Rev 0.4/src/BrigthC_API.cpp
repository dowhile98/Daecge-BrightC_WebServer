/**
 * @file BrigthC.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "main.h"

void led_blink(uint16_t led_pin,uint8_t blink_num, uint64_t delayBlink){
    
    digitalWrite(led_pin, LOW);
    for(uint8_t i = 0; i < blink_num; i++){
        digitalWrite(led_pin, HIGH);
        delay(delayBlink);
        digitalWrite(led_pin, LOW);
        delay(delayBlink);
    }
}
void ethernetReset(const uint8_t resetPin){
    pinMode(resetPin, OUTPUT); 
    digitalWrite(resetPin, HIGH);
    delay(250);
    digitalWrite(resetPin, LOW);
    delay(50);
    digitalWrite(resetPin, HIGH);
    delay(350);
}

void temp_sensors_read(void){
  LCD_queue_t xQueue;
  bool status_temp = false;
  if(xSemaphoreTake(tempReadMutex, portMAX_DELAY) == pdPASS){
    DSB1.requestTemperatures();
    DSB2.requestTemperatures();
    sensorData.TMP1=DSB1.getTempCByIndex(0); 
    sensorData.TMP2=DSB2.getTempCByIndex(0);

    if(sensorData.TMP1 == -127.00){
      sensorData.TMP1 = 0;
      SERIAL_PRINTLN("-->(TEMP READ)CONECTAR SENSOR DE TEMPERATURA 1");
      status_temp = true;
    }
    if(sensorData.TMP2  == -127.00){
      sensorData.TMP2 = 0;
      
      SERIAL_PRINTLN("-->(TEMP READ)CONECTAR SENSOR DE TEMPERATURA 2");
      status_temp = true;
    }
    if(status_temp == true){
      sprintf(xQueue.line1, "ERROR SENSOR DE");
      sprintf(xQueue.line2, "TEMPERATURA");
      xQueue.view_time = 1;
      xQueueSend(lcd_queue, (void *) &xQueue, 10);
      if((sensorData.TMP1 == 0) && (sensorData.TMP2 == 0) && (dataIn.S_BYPASSS_STS == 0)){
        SERIAL_PRINTLN("[CNFIG]->ENTRAR A MODO BYPAS POR FALLA DE SENSOR DE TEMPERATURA");
        xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_TEMP);
      }
    }
    //SE VERIFICA SI SE ENTRO A BYPASS
    SERIAL_PRINT("-->(TEMP READ)DSB1: "); 
    SERIAL_PRINT(sensorData.TMP1);
    SERIAL_PRINT("  DSB2: "); 
    SERIAL_PRINTLN(sensorData.TMP2);
    sensorData.HUMEDAD = 75;
    xSemaphoreGive(tempReadMutex);
  }
}

void ethernet_connect(uint8_t *mac, IPAddress _ip, IPAddress _dns, IPAddress _gateway, IPAddress _subnet){
  SERIAL_PRINTLN("ESP32->W5100 Iniciciando...");
  SPI.begin(ETH_SCLK, ETH_MISO, ETH_MOSI);
  /*Generar un reset al shield ethernet*/
  ethernetReset(ETH_RST);
  Ethernet.init(ETH_CS);
  led_blink(LED, 4, 40);
  //LCD...
  lcd.clear();
  lcd.print("Iniciando ETH");
  lcd.setCursor(0, 1);
  delay(1000);
  SERIAL_PRINTLN("Iniciando conexion ETH...");
  //->
  //Ethernet.begin(mac, _ip, _dns, _gateway, _subnet);
  Ethernet.begin(mac, _ip);
  delay(8000);
  lcd.clear();
  lcd.print("ADAPTADOR");
  lcd.setCursor(0, 1);
  lcd.print("CONFIGURADO");
  delay(2000);
  lcd.clear();
  lcd.print("ETH CONFIGURADO");
  lcd.setCursor(0, 1);
  //BLINK DE RETARDO
  for(int i = 0; i<15; i++){
    lcd.print(".");
    led_blink(LED, 1, 100);
  }
  lcd.clear();
  lcd.print("------>IP<------");
  lcd.setCursor(0, 1);
  lcd.print(Ethernet.localIP());
  SERIAL_PRINT("Ethernet IP es: ");
  SERIAL_PRINTLN(Ethernet.localIP());
  delay(1000);
  return;
}


void PCF_Pin_Init(void){
  //ENTRADA
  PCF1.pinMode(P_AAA1_AS,INPUT);
  PCF1.pinMode(P_AAA1_AABP,INPUT);
  PCF1.pinMode(P_AAA2_AS,INPUT);
  PCF1.pinMode(P_AAA2_AABP,INPUT);
  PCF1.pinMode(P_AAA3_AS,INPUT);
  PCF1.pinMode(P_AAA3_AABP,INPUT);
  PCF1.pinMode(P_AAA4_AS,INPUT);
  PCF1.pinMode(P_AAA4_AABP,INPUT); 
  if (PCF1.begin()){
    SERIAL_PRINT("PCFf8574 1 ..."); 
    SERIAL_PRINTLN("OK");
  }else{
    SERIAL_PRINT("PCFf8574 1...");
    SERIAL_PRINTLN("ERROR");
  }
  pinMode(BTN_CNFIG_1, INPUT_PULLUP);
  pinMode(P_A_Incendio, INPUT_PULLUP);
  //SALIDA
  PCF2.pinMode(P_AA1, OUTPUT);
  PCF2.pinMode(P_AA2, OUTPUT);
  PCF2.pinMode(P_AA3, OUTPUT);
  PCF2.pinMode(P_AA4, OUTPUT); 
  PCF2.pinMode(P_S_BYPASS1, OUTPUT);
  PCF2.pinMode(P_S_BYPASS2, OUTPUT);
  PCF2.pinMode(P_S_BYPASS3, OUTPUT);
  PCF2.pinMode(P_S_BYPASS4, OUTPUT);
  if (PCF2.begin()){
    SERIAL_PRINT("PCFf8574 2 ..."); 
    SERIAL_PRINTLN("OK");}
  else{
    SERIAL_PRINT("PCFf8574 2...");
    SERIAL_PRINTLN("ERROR");
  }
  return;
}
void digital_pin_config(void){
  PCF_Pin_Init();
  pinMode(LED, OUTPUT);
  pinMode(P_S_BYPASS_sts, OUTPUT);
  pinMode(BUZER, OUTPUT);
  digitalWrite(BUZER, LOW);
  return;
}