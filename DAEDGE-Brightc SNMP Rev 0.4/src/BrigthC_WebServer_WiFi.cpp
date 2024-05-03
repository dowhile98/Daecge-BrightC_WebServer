/**
 * @file BrigthC_WebServer_WiFi.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/*Includes -----------------------------------------------------------------*/
#include "main.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "SNMP_OID.h"
#include "Server.h"
/*Defines -------------------------------------------------------------------*/


/*Global variables ----------------------------------------------------------*/
bool ledState = 0;

extern MIB mib;
AsyncWebServer WiFiServer(80);
extern SNMP_Cmd_t cmd;
/*Function prototype --------------------------------------------------------*/


/*Function prototype -------------------------------------------------------*/
void notFound(AsyncWebServerRequest *request);
void handleRoot(AsyncWebServerRequest *request);
void handleJS(AsyncWebServerRequest *request);
void handleCSS(AsyncWebServerRequest *request);
void handleAPIEstado_Actual(AsyncWebServerRequest *request);
void handleAPIUpdateConfig(AsyncWebServerRequest *request);
void handleAPIMedidor(AsyncWebServerRequest *request);
void handleInputConfig(AsyncWebServerRequest *request, JsonVariant &json);
void handleAPIControlManual(AsyncWebServerRequest *request, JsonVariant &json);
void handleAPIobtenerYActualizarEstados(AsyncWebServerRequest *request);
void handleConfig(AsyncWebServerRequest *request);
void handleAPIipconfig(AsyncWebServerRequest *request, JsonVariant &json);
/*Task definition ----------------------------------------------------------*/
void webServer_Task_WiFi(void *params){
  
  
  Serial.println("[WIFI]->Init AP (access Point)");
  WiFi.softAP("BrightC_Manager", "987654321");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP addres: ");
  Serial.println(IP);


  // WiFi.begin("RQUINOB", "R#29qbCivil");
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.print("Attempting to connect to SSID: ");
  //   Serial.println("RQUINOB");
  //   delay(1000);
  // }
  // Serial.print("IP Address: ");
  // Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  WiFiServer.on("/", HTTP_GET, handleRoot);
  WiFiServer.onNotFound(notFound);
  WiFiServer.on("/css/style.css", HTTP_GET, handleCSS);
  WiFiServer.on("/js/index.js", HTTP_GET, handleJS);
  WiFiServer.on("/ipconfig", HTTP_GET, handleConfig);
  /*Api init ----------------------------------------------*/
  WiFiServer.on("/api/estado_actual", HTTP_GET, handleAPIEstado_Actual);
  WiFiServer.on("/api/config_data", HTTP_GET, handleAPIUpdateConfig);
  WiFiServer.on("/api/medidor", HTTP_GET, handleAPIMedidor);
  WiFiServer.on("/api/obtener_estados", HTTP_GET,handleAPIobtenerYActualizarEstados);

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/api/configuracion", handleInputConfig);
  WiFiServer.addHandler(handler);
  handler = new AsyncCallbackJsonWebHandler("/api/control_manual", handleAPIControlManual);
  WiFiServer.addHandler(handler);

  handler = new AsyncCallbackJsonWebHandler("/api/configip", handleAPIipconfig);
  WiFiServer.addHandler(handler);

  AsyncElegantOTA.begin(&WiFiServer, "admin", "admin");    // Start ElegantOTA
  
  WiFiServer.begin();

  Serial.println("[WIFI] HTTP Server started");
  for(;;){
    // ws.cleanupClients();
    // digitalWrite(2, ledState);
    vTaskDelay(portMAX_DELAY);
  }
}

/*Function definition ---------------------------------------------------*/
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
/*Home ---------------------------------------------------------------------------------*/
void handleRoot(AsyncWebServerRequest *request) {
  Serial.println("requet for: /index.html");
  request->send(SPIFFS, "/index.html", "text/html");
}
void handleJS(AsyncWebServerRequest *request) {
  Serial.println("requet for: /js/index.js");
  request->send(SPIFFS, "/js/index.js", "application/javascript");
}
void handleCSS(AsyncWebServerRequest *request) {
  Serial.println("requet for: /css/style.css");
  request->send(SPIFFS, "/css/style.css", "text/css");
}
void handleConfig(AsyncWebServerRequest *request){
  Serial.println("request for: inconfig.html");
  request->send(SPIFFS, "/ipconfig.html", "text/html");
}
void handleAPIEstado_Actual(AsyncWebServerRequest *request){
  //String responseJson;
  AsyncResponseStream *response = request->beginResponseStream("application/json");

  DynamicJsonDocument doc(1024);
  doc["ubicacion"] = String(config.name);
  doc["cantAA"] = sec_AA.SEC_CANT_AA;
  doc["tempSensor1"] = sensorData.TMP1;
  doc["tempSensor2"] = sensorData.TMP2;

  //AA1
  doc["turno1"] = (V_AA[0].turn == true) ?  "EN TURNO" : "NO";
  doc["estadoAA1"] = (V_AA[0].workig == true) ? "ENCENDIDO" : "APAGADO";
  doc["ayuda1"] = (V_AA[0].helper == true) ? "AYUDANDO" : "NO";
  doc["contadorFallas1"] = V_AA[0].failcount;
  //AA2
  doc["turno2"] =(V_AA[1].turn == true) ?  "EN TURNO" : "NO";
  doc["estadoAA2"] = (V_AA[1].workig == true) ? "ENCENDIDO" : "APAGADO";
  doc["ayuda2"] = (V_AA[1].helper == true) ? "AYUDANDO" : "NO";
  doc["contadorFallas2"] = V_AA[1].failcount;
  //AA3
  doc["turno3"] = (V_AA[2].turn == true) ?  "EN TURNO" : "NO";
  doc["estadoAA3"] = (V_AA[2].workig == true) ? "ENCENDIDO" : "APAGADO";
  doc["ayuda3"] = (V_AA[2].helper == true) ? "AYUDANDO" : "NO";
  doc["contadorFallas3"] = V_AA[2].failcount;
  //AA4
  doc["turno4"] = (V_AA[3].turn == true) ?  "EN TURNO" : "NO";
  doc["estadoAA4"] = (V_AA[3].workig == true) ? "ENCENDIDO" : "APAGADO";
  doc["ayuda4"] = (V_AA[3].helper == true) ? "AYUDANDO" : "NO";
  doc["contadorFallas4"] = V_AA[3].failcount;

  //alarmas
  doc["bypass"] = (dataIn.S_BYPASSS_STS == true) ? "EN MODO BYPASS" : "NO";
  doc["A_SAA1"] = (dataIn.in1 == true) ? "Activo" : "-";
  doc["A_ABAA1"] = (dataIn.in2 == true) ? "Activo" : "-";
  doc["A_SAA2"] =(dataIn.in3 == true) ? "Activo" : "-";
  doc["A_ABAA2"] = (dataIn.in4 == true) ? "Activo" : "-";
  doc["A_SAA3"] = (dataIn.in5 == true) ? "Activo" : "-";
  doc["A_ABAA3"] = (dataIn.in6 == true) ? "Activo" : "-";
  doc["A_SAA4"] = (dataIn.in7 == true) ? "Activo" : "-";
  doc["A_ABAA4"] = (dataIn.in8 == true) ? "Activo" : "-";
  doc["AFIRE"] = (dataIn.A_Incendio == true) ? "Activo" : "-";
  doc["ATEMH"] = (dataIn.TMP_High == true) ? "Activo" : "-";
  //serialice
  serializeJson(doc, *response);
  request->send(response);
}

void handleAPIUpdateConfig(AsyncWebServerRequest *request){
  //String responseJson;
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonDocument doc(256);
  doc["cantAAC"] = sec_AA.SEC_CANT_AA;
  doc["DSEC"] = sec_AA.SEC_DIAS;
  doc["TMINC"] = sec_AA.SEC_TMP_MIN;
  doc["TMAXC"] = sec_AA.SEC_TMP_MAX;
  doc["TALC"] = sec_AA.SEC_TMP_ALARMA;
  doc["TBYC"] = sec_AA.SEC_TMP_BYPASS;
  doc["MINC"] = sec_AA.TIM_ENFRIO_SP;
  serializeJson(doc, *response);
  request->send(response);
}

void handleAPIMedidor(AsyncWebServerRequest *request){
  Serial.println("request data medidor electrico");
  //String responseJson;
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonDocument doc(300);
  doc["voltajeRMS"] = sensorData.RMS_V;
  doc["potenciaActiva"] = sensorData.PA;
  doc["potenciaReactiva"] = sensorData.PR;
  doc["potenciaAparente"] = sensorData.PAP;
  doc["energiaActiva"] = sensorData.EA;
  doc["energiaReactiva"] = sensorData.ER;
  doc["factorPotencia"] = sensorData.FP;
  doc["frecuencia"] = sensorData.FREC;
  serializeJson(doc, *response);
  request->send(response);
}

void handleInputConfig(AsyncWebServerRequest *request, JsonVariant &json){
  StaticJsonDocument<256> doc;
  char  val[10] = {0};
  if (json.is<JsonArray>())
  {
    doc = json.as<JsonArray>();
  }
  else if (json.is<JsonObject>())
  {
    doc = json.as<JsonObject>();
  }

  strcpy(val, doc["cantidad_aires"]);
  //se envia el comando de cambio de cantidad de aires acondicionados
  if(config.SEC_CANT_AA != atoi(val)){
    config.SEC_CANT_AA = atoi(val);
    mib.setCant_AA((const char *)val);
    cmd.command =  CMD_CANT_AA;
    cmd.pData = (const char*) &config.SEC_CANT_AA;
    xEventGroupSetBits(secEvents, BRIGTHC_CHANGE_NUMBER_AA);
    xQueueSend(command_queue, (void *)&cmd, 100);
  }
  strcpy(val, doc["dias_secuencia"]);
  config.SEC_DIAS = atoi(val);
  mib.setSec_Dias((const char *)val);

  strcpy(val, doc["temp_alarma"]);
  config.SEC_TMP_ALARMA = atof(val);
  mib.setT_Alarma((const char *)val);

  strcpy(val, doc["temp_bypass"]);
  config.SEC_TMP_BYPASS = atof(val);
  mib.setT_Bypass((const char *)val);

  strcpy(val, doc["temp_maxima"]);
  config.SEC_TMP_MAX = atof(val);
  mib.setT_Max((const char*)val);

  strcpy(val, doc["temp_minima"]);
  config.SEC_TMP_MIN = atof(val);
  mib.setT_Min((const char *)val);

  
  strcpy(val, doc["tiempo_enfriar"]);
  config.TIM_ENFRIO_SP = atoi(val);
  mib.setMax_Cool_sp((const char *)val);
  //todo save data
  SPIFFS_saveConfiguration(filename, &config);
  copy_data_config(sec_AA, config);
  String response;
  serializeJson(doc, response);
  request->send(200, "application/json", response);
  Serial.println(response);
}

void handleAPIControlManual(AsyncWebServerRequest *request, JsonVariant &json){
  StaticJsonDocument<200> doc;
  uint8_t cmdVal = 0;
  if (json.is<JsonArray>())
  {
    doc = json.as<JsonArray>();
  }
  else if (json.is<JsonObject>())
  {
    doc = json.as<JsonObject>();
  }


  cmdVal = doc["valor"];
  if(doc["accion"] == "modoBypass"){
    cmd.command = CMD_BYPASS_LOGICO;
  }else if(doc["accion"] == "modoManual"){
    cmd.command = CMD_MANUAL;
  }else if(doc["accion"] == "aa1"){
    cmd.command = CMD_MANUAL_AA1;
  }else if(doc["accion"] == "aa2"){
    cmd.command = CMD_MANUAL_AA2;;
  }else if(doc["accion"] == "aa3"){
    cmd.command = CMD_MANUAL_AA3;;
  }else if(doc["accion"] == "aa4"){
    cmd.command = CMD_MANUAL_AA4;;
  }else if(doc["accion"] == "reiniciarFallas"){
    cmd.command = CMD_RST_FAIL_COUNT;
  }else if(doc["accion"] == "reiniciarEnergia"){
    cmd.command = CMD_RST_ENERGY;
  }
  //send cmd to callback task
  cmd.pData = (const char *)&cmdVal;
  xQueueSend(command_queue, (void *)&cmd, 100);
  delay(100);
  DynamicJsonDocument docTx(128);
  docTx["modoBypass"] = (dataIn.S_BYPASSS_STS == true) ? "1" : "0";
  docTx["modoManual"] = (sec_AA.MANUAL == true) ? "1" : "0";
  docTx["aa1"] = (V_AA[0].workig == true) ? "1" : "0";
  docTx["aa2"] = (V_AA[1].workig == true) ? "1" : "0";
  docTx["aa3"] = (V_AA[2].workig == true) ? "1" : "0";
  docTx["aa4"] = (V_AA[3].workig == true) ? "1" : "0";

  String response;
  serializeJson(docTx, response);
  request->send(200, "application/json", response);
  Serial.println(response);
  return;
}

void handleAPIipconfig(AsyncWebServerRequest *request, JsonVariant &json){
  StaticJsonDocument<200> data;
  char val[30];
  if (json.is<JsonArray>())
  {
    data = json.as<JsonArray>();
  }
  else if (json.is<JsonObject>())
  {
    data = json.as<JsonObject>();
  }

  strcpy(config.name, data["name"]);
  strcpy(config.ip, data["ip"]);
  strcpy(config.gateway, data["gateway"]);
  strcpy(config.subnet, data["subnet"]);
  strcpy(config.ipserver, data["serverIp"]);

  //todo save data
  SPIFFS_saveConfiguration(filename, &config);
  copy_data_config(sec_AA, config);
  String response;
  serializeJson(data, response);
  request->send(200, "application/json", response);
  Serial.println(response);
  return;
}


void handleAPIobtenerYActualizarEstados(AsyncWebServerRequest *request){
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonDocument doc(256);
  doc["modoBypass"] = (dataIn.S_BYPASSS_STS == true) ? "1" : "0";
  doc["modoManual"] = (sec_AA.MANUAL == true) ? "1" : "0";
  doc["aa1"] = (V_AA[0].workig == true) ? "1" : "0";
  doc["aa2"] = (V_AA[1].workig == true) ? "1" : "0";
  doc["aa3"] = (V_AA[2].workig == true) ? "1" : "0";
  doc["aa4"] = (V_AA[3].workig == true) ? "1" : "0";
  serializeJson(doc, *response);
  request->send(response);
}