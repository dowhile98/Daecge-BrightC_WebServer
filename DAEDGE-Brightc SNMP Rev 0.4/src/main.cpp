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
#include "SNMP_OID.h"
/*Defines ------------------------------------------------------------------------------*/

/*Global variables ---------------------------------------------------------------------*/

//->semaphores
SemaphoreHandle_t tempReadMutex;
SemaphoreHandle_t  shareDataMutex;

SemaphoreHandle_t mibMutex;
SemaphoreHandle_t ethernetMutex;
//->queues
QueueHandle_t lcd_queue = NULL;
QueueHandle_t buzer_queue = NULL;
//->task handles
TaskHandle_t air_handle = NULL;
TaskHandle_t ctrl_handle = NULL;
TaskHandle_t bypass_handle = NULL;
TaskHandle_t callback_handle = NULL;
//->events
EventGroupHandle_t secEvents;
EventGroupHandle_t controlEvents;
EventGroupHandle_t errorEvents;
EventGroupHandle_t systemEvents;
EventGroupHandle_t bypasState;

QueueHandle_t command_queue = NULL;
//->data
Measurements_t sensorData;
Input_val_t dataIn = {0};

uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
const char *filename = "/data.json";
ESP32Time rtc;

InputState_t sw;
/*Object instances --------------------------------------------------------------------*/
OneWire onewire1(P_DS18B20_1);
OneWire onewire2(P_DS18B20_2);
DallasTemperature DSB1(&onewire1);
DallasTemperature DSB2(&onewire2);
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
PCF8574 PCF2(PCF2_ADDR);                           //A0=1, A1=0, A2=0 
PCF8574 PCF1(PCF1_ADDR);                           //A0=0, A1=0, A2=0 
PZEM004Tv30 pmeter1(PZEM_SERIAL_PM1, PZEM_RX_PIN_PM1, PZEM_TX_PIN_PM1);
PZEM004Tv30 pmeter2(PZEM_SERIAL_PM2, PZEM_RX_PIN_PM2, PZEM_TX_PIN_PM2);




EthernetUDP udp;


SNMP::Agent snmp;
MIB mib;
Brigthc_Addres_t net = {
  // .ip = IPAddress(10, 208, 85, 42),
  .ip = IPAddress(10, 235, 125, 15),
  // .DNS = IPAddress(10, 208, 85, 41),
  .DNS = IPAddress(10,235,125,2),
  //.GATEWAY = IPAddress(10, 208, 85, 41),
  .GATEWAY = IPAddress(10, 235, 125, 2),
  .SUBNET = IPAddress(255,255, 255, 0),
  .IPSERVER = IPAddress(172, 19, 216, 30)
};
struct tm timeinfo = {
  .tm_sec = 0,
  .tm_min = 0,
  .tm_hour = 0,
  .tm_mday = 7,
  .tm_mon = 11,
  .tm_year = 2023
};

Config_t config;
//CONTROL DE SECUENCIA DE AA
Sequence_AA_t sec_AA;
uint64_t timestamp_ant=0;
AA_t V_AA[4] = {{false, false, 0,4},{false,false, 1,5},{false, false, 2,6},{false, false, 3,7}};

/*FUnction prototype ------------------------------------------------------------------*/
void onMessage(const SNMP::Message *message, const IPAddress remote, const uint16_t port);
/*Setup---------------------------------------------------------------------------------*/
void setup() {
  /*Local variables -------------------------------------------------------------------*/
  buzzer_queue_t xQueuebuzzer;  
  BaseType_t status;
  uint32_t config_time = 0;
  uint8_t count = 0;
  uint32_t temp_id = ESP.getEfuseMac();
  String chipid = String(temp_id);
  /*Init sequence --------------------------------------------------------------------*/
  DEBUG_SERIAL.begin(115200);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  digital_pin_config();

  /*Bypass mode by default ----------------------------------------------------------*/
   
  digitalWrite(P_S_BYPASS_sts, HIGH); 
  /*sound*/
  for(uint8_t i = 0; i<4; i++){
    digitalWrite(BUZER, HIGH);
    delay(50);
    digitalWrite(BUZER, LOW);
    delay(50);
  }
  /*Restauracion de valores--------------------------------------------------*/
  SPIFFS_init();
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.print("--DAECGE SAC---");
  lcd.setCursor(0, 1);
  lcd.print("    BrightC");
  delay(5000);
  lcd.clear();
  SERIAL_PRINTLN("Restaurando datos de la flash...");   
  //default values
  strcpy(config.name, "BRIGHTC");
  strcpy(config.ip, net.ip.toString().c_str());
  strcpy(config.gateway, net.GATEWAY.toString().c_str());
  strcpy(config.subnet, net.SUBNET.toString().c_str());
  strcpy(config.ipserver, net.IPSERVER.toString().c_str());
  config.SEC_CANT_AA = 2;
  config.SEC_DIAS = 7;
  config.SEC_TMP_MIN = 23;
  config.SEC_TMP_MAX = 24.5;
  config.SEC_TMP_ALARMA = 30;
  config.SEC_TMP_BYPASS = 32;
  config.TIM_ENFRIO_SP = 30;
  //retauracion de valores por defecto
  if(SPIFFS_loadConfiguration(filename, &config) == true){
    
    lcd.print("INICIANDO...");
    lcd.setCursor(0, 1);
    delay(1500);
    lcd.print("OK!");
  }else{
    lcd.print("CARGAR DATOS");
    lcd.setCursor(0, 1);
    delay(1500);
    lcd.print("ERROR...");
    delay(2000);
    ESP.restart();
  }
  delay(1000);
  copy_data_config(sec_AA, config);
/******************************************************************/
  //queue
  buzer_queue = xQueueCreate(3, sizeof(buzzer_queue_t));
  configASSERT(buzer_queue != NULL);
  //RTOS
  status = xTaskCreatePinnedToCore(buzer_task, 
                                  "BUZZER",
                                  1000,
                                  NULL,
                                  10,
                                  NULL,
                                  1);
  configASSERT(status == pdPASS);
/*****************************************************************/
  led_blink(LED, 2, 50);
  delay(3000);
  lcd.clear();
  lcd.print("HABILITAR SERVER");
  lcd.setCursor(0, 1);
  lcd.print("WIFI");
  delay(4000);
  lcd.clear();
  lcd.print("PRESIONAR BOTON");
  lcd.setCursor(0, 1);
  lcd.print("Esperando.");
  //entrar en modo configuracion
  config_time = millis();
  initButtonState(sw, STATE_TO_LOW);
  while(1){
    if(readButton(sw, digitalRead(BTN_CNFIG_1)) == STATE_TO_LOW){
      //se crea la tarea
      SERIAL_PRINTLN("[CONFIG]->Servidor Web creado");
      status = xTaskCreatePinnedToCore(webServer_Task_WiFi, "WebServer WiFi",
      4096, NULL, 6, NULL, ARDUINO_RUNNING_CORE);
      configASSERT(status == pdPASS);
      delay(3000);
      lcd.clear();
      lcd.print(config.name);
      lcd.setCursor(0, 1);
      lcd.print("IP:192.168.4.1");
      SERIAL_PRINTLN("SERVER INIT: IP:192.168.4.1");
      delay(3000);
      break;
    }
    if((millis() - config_time)>2000){
      xQueuebuzzer.delay = 50;
      xQueuebuzzer.repeats = 2;
      xQueueSend(buzer_queue, (void *) & xQueuebuzzer, 10);
      config_time = millis();
      lcd.print(".");
      count++;
      if(count>5){
        break;
      }
    }
  }
  /**************************************************************************************/
  xQueuebuzzer.delay = 50;
  xQueuebuzzer.repeats = 4;
  xQueueSend(buzer_queue, (void *) & xQueuebuzzer, 10);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CONECTANDO A RED");
  lcd.setCursor(0,1);
  delay(1000);
  /*-----------------------------------------------------------------------------------*/
  rtc.setTime(timeinfo.tm_sec, timeinfo.tm_min, timeinfo.tm_hour, timeinfo.tm_mday, timeinfo.tm_mon, timeinfo.tm_year);
  delay(1500);
  //->
  // ip = IPAddress(192,168, 1, 100);
  // IPM = IPAddress(192, 168, 1, 79);
  ethernet_connect(mac, net.ip, net.DNS, net.GATEWAY, net.SUBNET);
  //->
  //Se emite un sonido
  for(uint8_t i = 0; i<2; i++){
    digitalWrite(BUZER, HIGH);
    delay(40);
    digitalWrite(BUZER, LOW);
    delay(40);
  }
  //LEER LAS DIRECCIONES DE LOS SENSORES DE CORRIENTE
  Serial.print("Custom Address Power Meter 1:");
  Serial.println(pmeter1.readAddress(), HEX);
  Serial.print("Custom Address Power Meter 2:");
  Serial.println(pmeter2.readAddress(), HEX);
  bool tempbool = false;
  //SNMP
  mib.setContact("emilce.meza@daecge.net");
  mib.setLocation("POP-Aeropuerto");
  mib.setName("BRIGHT-C");
  mib.setID(chipid.c_str());
  mib.setIP(net.ip.toString().c_str());
  mib.setT_Max(String(sec_AA.SEC_TMP_MAX).c_str());
  mib.setT_Min(String(sec_AA.SEC_TMP_MIN).c_str());
  mib.setT_Alarma(String(sec_AA.SEC_TMP_ALARMA).c_str());
  mib.setT_Bypass(String(sec_AA.SEC_TMP_BYPASS).c_str());
  mib.setSec_Dias((const char*)&sec_AA.SEC_DIAS);
  mib.setCant_AA((const char*)&sec_AA.SEC_CANT_AA);
  mib.setMax_Cool_sp((const char*)&sec_AA.TIM_ENFRIO_SP);
  mib.setBypassLogico((const char*)&dataIn.S_BYPASSS_STS);
  mib.setManual((const char*)&sec_AA.MANUAL);
  mib.setRstEnergy((const char*)&tempbool);
  mib.setManualAA1((const char*)&sec_AA.AA1_MANUAL);
  mib.setManualAA2((const char*)&sec_AA.AA2_MANUAL);
  mib.setManualAA3((const char*)&sec_AA.AA3_MANUAL);
  mib.setManualAA4((const char*)&sec_AA.AA4_MANUAL);
  mib.setRstFailCount((const char*)&tempbool);
  mib.setMeasurement(sensorData);
  mib.setAAState(V_AA);
  mib.setInputVal(dataIn);
  // SNMP
  snmp.begin(&udp);
  snmp.onMessage(onMessage);

  delay(6000);
  // SNMP::Message *message = mib.trap(net.ip, "", TRAP_SEVERITY_WARNING, TRAP_COLDSTART);
  // snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
  // delete message;
  // delay(2000);
  // message = mib.trap(net.ip, "", TRAP_SEVERITY_WARNING, TRAP_SYSTEMUP);
  // snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
  // delete message;
  // delay(4000);

  command_queue = xQueueCreate(20, sizeof(String));
  configASSERT( command_queue != NULL);

  lcd_queue =  xQueueCreate(5, sizeof(LCD_queue_t));
  configASSERT( lcd_queue != NULL);
/****************************************************************************************/
  tempReadMutex = xSemaphoreCreateMutex();
  configASSERT(tempReadMutex != NULL);
  //mutex para snmp
  mibMutex = xSemaphoreCreateMutex();
  configASSERT( mibMutex != NULL);
  //mutex para el ethernet
  ethernetMutex = xSemaphoreCreateMutex();
  configASSERT(ethernetMutex != NULL);

  //events 
  secEvents = xEventGroupCreate();
	configASSERT(secEvents != NULL);

	controlEvents = xEventGroupCreate();
	configASSERT(controlEvents != NULL);

	errorEvents = xEventGroupCreate();
	configASSERT(errorEvents != NULL);

	systemEvents = xEventGroupCreate();
	configASSERT(systemEvents != NULL);

	bypasState = xEventGroupCreate();
	configASSERT(bypasState != NULL);

  shareDataMutex = xSemaphoreCreateMutex();
	configASSERT(shareDataMutex != NULL);
  //->task
  configASSERT(status == pdPASS);
  status = xTaskCreatePinnedToCore(lcd_task,
                                  "LCD",
                                  (4000),
                                  NULL,
                                  8,
                                  NULL,
                                  ARDUINO_RUNNING_CORE);
  initButtonState(sw, STATE_TO_LOW);
  
  status = xTaskCreatePinnedToCore(Sec_Task,
                                  "AA SEC TASK",
                                  (8000),
                                  NULL,
                                  8,
                                  &air_handle,
                                  ARDUINO_RUNNING_CORE);
  configASSERT(status == pdPASS);
  status = xTaskCreatePinnedToCore(Controller_Task,
                                  "CTRL",
                                  (4000),
                                  NULL,
                                  8,
                                  &ctrl_handle,
                                  ARDUINO_RUNNING_CORE);
  configASSERT(status == pdPASS);

  status = xTaskCreatePinnedToCore(Bypass_Task,
                                  "BYPASS",
                                  (5000),
                                  NULL,
                                  8,
                                  &bypass_handle,
                                  ARDUINO_RUNNING_CORE);
  configASSERT(status == pdPASS);

  status = xTaskCreatePinnedToCore(callback_command_task,
                                  "CALLBACK",
                                  (8000),
                                  NULL,
                                  8,
                                  &callback_handle,
                                  ARDUINO_RUNNING_CORE);
  configASSERT(status == pdPASS);
  status = xTaskCreatePinnedToCore(input_read_task,
                                  "SENSOR",
                                  (30000),
                                  NULL,
                                  8,
                                  NULL,
                                  ARDUINO_RUNNING_CORE);


  status = xTaskCreatePinnedToCore(webServer_Task,
                                  "SERVER",
                                  (8000),
                                  NULL,
                                  8,
                                  NULL,
                                  ARDUINO_RUNNING_CORE);
  configASSERT(status == pdPASS);

  initButtonState(sw, STATE_TO_LOW);
}
/*Loop----------------------------------------------------------------------------------*/
void loop() {
  buzzer_queue_t xQueue_buzer;
  // put your main code here, to run repeatedly:
  if(xSemaphoreTake(ethernetMutex, portMAX_DELAY) == pdTRUE){
    snmp.loop();
    xSemaphoreGive(ethernetMutex);
  }
  if(readButton(sw, digitalRead(BTN_CNFIG_1)) == STATE_TO_LOW){
    SERIAL_PRINT("S_BYPASS_STS = ");
    SERIAL_PRINTLN(dataIn.S_BYPASSS_STS);
    SERIAL_PRINT("MANUAL: ");
    xQueue_buzer.delay = 50;
    xQueue_buzer.repeats  = 1;
    xQueueSend(buzer_queue, (void *) & xQueue_buzer, 30);
    //se verifica el estado del bypass
    if((dataIn.S_BYPASSS_STS == false)){
      //se envia el comando para entrar a modo bypass
      SERIAL_PRINTLN("[SETUP]->ENTRANDO A MODO BYPASS");
      xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_EXT);
      delay(10);
      dataIn.S_BYPASSS_STS = 1;
    }else if((dataIn.S_BYPASSS_STS == true)){
      sec_AA.BYPASS_LOGICO = false;
      SERIAL_PRINTLN("[SETUP]->SALIENDO A MODO BYPASS");
      xEventGroupSetBits(controlEvents, BRIGTHC_CTRL_BYPASS_EXIT);
      dataIn.S_BYPASSS_STS = 0;
      delay(10);
    }
    while(digitalRead(BTN_CNFIG_1) == 0){}
    SERIAL_PRINTLN("[SETUP]->SW liberado\r\n");
  }
  vTaskDelay(3);
  
}

/*Function definition ------------------------------------------------------------------*/

void onMessage(const SNMP::Message *message, const IPAddress remote, const uint16_t port) {
    SERIAL_PRINTLN("MENSAJE RECIBIDO");
    mib.setAAState(V_AA);
    // Create an SNMP message for response
    SNMP::Message *response = new SNMP::Message(message->getVersion(),
            message->getCommunity(), SNMP::TYPE_GETRESPONSE);
    // The response must have the same request-id as the request
    response->setRequestID(message->getRequestID());
    // Build response for message
    if (mib.message(message, response)) {
        // Send response
        snmp.send(response, remote, port);
    }
    // Avoid memory leak
    delete response;
}