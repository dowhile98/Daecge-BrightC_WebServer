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
    sensorData.TMP1 -= 1.5;
    sensorData.TMP2 -= 1.5;
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
  Ethernet.begin(mac, _ip, _dns, _gateway);
  //Ethernet.begin(mac, _ip);
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

bool SPIFFS_isExist(fs::FS &fs, const char * path){
  return fs.exists(path);
}
void SPIFFS_init(void){
    if(!SPIFFS.begin(true)){
        Serial.println("[SPIFSS]->Se ha producido un error al montar SPIFFS");
    }else{
        Serial.println("[SPIFFS]->montado con éxito");
    }
}
/**
 * @brief lee datos en formato string
 * 
 * @param fs 
 * @param path : direccion del archivo
 * @return String : dato leido en formato string
 */
String SPIFFS_readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("[SPIFSS]->no se pudo abrir el archivo para leer");
        return String();
    }
  
    String fileContent;
    while(file.available()){
        fileContent = file.readStringUntil('\n');
        break;     
    }
    return fileContent;
}
/**
 * @brief almacena datos en formato ASCII
 * 
 * @param fs :
 * @param path : Ubicación del archivo 
 * @param message : dato almacenar
 */
void SPIFFS_writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("[SPIFFS]->no se pudo abrir el archivo para escribir");
        return;
    }
    if(file.print(message)){
        Serial.println("[SPIFFS]->archivo escrito");
    } else {
        Serial.println("[SPIFFS]->Escritura fallida");
    }
}

bool SPIFFS_loadConfiguration(const char *filename, Config_t *xconfig){
  File configFile = SPIFFS.open(filename, FILE_READ);

  if(!configFile){
    SERIAL_PRINTLN("[SPIFFS]->Error al abrir el archivo");
    return false;
  }
  size_t size = configFile.size();

  if(size>1024){
    SERIAL_PRINTLN("[SPIFFS]->El tamaño del archivo de configuración es demasiado grande");
  }
  DynamicJsonDocument doc(700);

  DeserializationError error = deserializeJson(doc, configFile);
  
  if(error){
    SERIAL_PRINTLN("[SPIFFS]No se pudo leer el archivo, usando la configuración predeterminada");
    SERIAL_PRINTLN("[SPIFFS]->first data write");
    SPIFFS_saveConfiguration(filename, xconfig);
    return false;
  }
  //Se carga la configuracion
  strcpy(xconfig->name, doc["Name"]);
  xconfig->SEC_CANT_AA = doc["SEC_CANT_AA"];
  xconfig->SEC_DIAS = doc["SEC_DIAS"];
  xconfig->SEC_TMP_MIN = doc["SEC_TMP_MIN"];
  xconfig->SEC_TMP_MAX = doc["SEC_TMP_MAX"];
  xconfig->SEC_TMP_ALARMA = doc["SEC_T_ALARMA"];
  xconfig->SEC_TMP_BYPASS = doc["SEC_T_BYPASS"];
  xconfig->TIM_ENFRIO_SP = doc["TIM_ENFRIO_SP"];
  strcpy(xconfig->ip, doc["IP_Device"]);
  strcpy(xconfig->gateway, doc["GATEWAY"]);
  strcpy(xconfig->subnet, doc["SUBNET"]);
  strcpy(xconfig->ipserver, doc["IPSERVER"]);
#ifdef __DEBUG_SESRIAL
  serializeJsonPretty(doc, DEBUG_SERIAL);
#endif
  SERIAL_PRINTLN("");
  configFile.close();
  return true;
}

bool SPIFFS_saveConfiguration (const char *filename, Config_t *xconfig){

  DynamicJsonDocument doc(700);
  doc["Name"] = xconfig->name;
  doc["SEC_CANT_AA"] = xconfig->SEC_CANT_AA;
  doc["SEC_DIAS"] = xconfig->SEC_DIAS;
  doc["SEC_TMP_MIN"] = xconfig->SEC_TMP_MIN;
  doc["SEC_TMP_MAX"] = xconfig->SEC_TMP_MAX;
  doc["TIM_ENFRIO_SP"] = xconfig->TIM_ENFRIO_SP;
  doc["IP_Device"] = xconfig->ip;
  doc["GATEWAY"] = xconfig->gateway;
  doc["SUBNET"] = xconfig->subnet;
  doc["SEC_T_ALARMA"] = xconfig->SEC_TMP_ALARMA;
  doc["SEC_T_BYPASS"] = xconfig->SEC_TMP_BYPASS;
  doc["IPSERVER"] = xconfig->ipserver;
  File configFile = SPIFFS.open(filename, FILE_WRITE);
  if(!configFile){
    SERIAL_PRINTLN("[SPIFFS]->Error al abrir el archivo de configuración para escribir");
    return false;
  }
  if(serializeJson(doc, configFile) == 0){
    SERIAL_PRINTLN("[SPIFFS]->Error al escribir en el archivo");
    return false;
  }
  configFile.close();
  return true;
}

void copy_data_config(Sequence_AA_t &xsec_AA, Config_t &xconfig){
  xsec_AA.SEC_CANT_AA = xconfig.SEC_CANT_AA;
  xsec_AA.SEC_DIAS = xconfig.SEC_DIAS;
  xsec_AA.SEC_TMP_MIN = xconfig.SEC_TMP_MIN;
  xsec_AA.SEC_TMP_MAX = xconfig.SEC_TMP_MAX;
  xsec_AA.TIM_ENFRIO_SP = xconfig.TIM_ENFRIO_SP;
  xsec_AA.SEC_TMP_ALARMA = xconfig.SEC_TMP_ALARMA;
  xsec_AA.SEC_TMP_BYPASS = xconfig.SEC_TMP_BYPASS;                          //Nombre del dispositivo
  net.ip.fromString(xconfig.ip);
  net.DNS.fromString(xconfig.gateway);
  net.GATEWAY.fromString(xconfig.gateway);
  net.SUBNET.fromString(xconfig.subnet);
  net.IPSERVER.fromString(xconfig.ipserver);
  return ;
}

uint8_t readButton(InputState_t &button, uint8_t physical_value){
    uint8_t current_state = physical_value;
    uint8_t output = STATE_NONE;
    //filter debounce
    if((millis() - button.last_change_time)>=20) //lo usual es 20ms
    {
        button.last_change_time = millis();
        if(button.previous_state != current_state){
            //detect event
            if(current_state == 0 && button.validState == STATE_TO_LOW)                  //on release
            {
                output = STATE_TO_LOW;
            }else if(current_state == 1 && button.validState == STATE_TO_HIGH){
                output = STATE_TO_HIGH;
            }
            button.previous_state = current_state;
        }
    }
    return output;
}

void initButtonState(InputState_t &button, ValidState_t v){
    button.previous_state = 1; // Suponemos que el pulsador está en estado bajo al principio
    button.last_change_time = millis();
    button.validState = v;
}