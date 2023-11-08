/**
 * @file BrigthC_WebServer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/*includes ------------------------------------------------------------------------------------*/
#include "main.h"
#include <Ethernet.h>
#include <EthernetClient.h>
#include <aWOT.h>
#include "MimeTypes.h"
#include "SNMP_OID.h"
/*Defines ------------------------------------------------------------------------------------*/
/*Defines -----------------------------------------------------------------------------------------*/
#define BRIGHTC_SERVER_PORT     80

/*Typedef -----------------------------------------------------------------------------------------*/


/*Global variables --------------------------------------------------------------------------------*/
extern MIB mib;
#if (USE_WIIFI == 1)
#include <WiFi.h>
WiFiServer server(BRIGHTC_SERVER_PORT);
#else
EthernetServer server(BRIGHTC_SERVER_PORT);
#endif

Application app;
const char * indexFile = "/index.html";
SNMP_Cmd_t cmd = {0};
/*Function prototype ------------------------------------------------------------------------------*/

static void fileServer(Request &req, Response &res);
static void handleIndexHTML(Request &req, Response &res);
static void handleIndexCSS(Request &req, Response &res);
static void handleIndexJS(Request &req, Response &res);
static void handleAPIEstadoActual(Request &req, Response &res);
static void handleAPIConfigData(Request &req, Response &res);
static void handleAPIMedidor(Request &req, Response &res);
static void handleAPIConfiguracion(Request &req, Response &res);
static void handleAPIControlManual(Request &req, Response &res);
static void handleAPIobtenerYActualizarEstados(Request &req, Response &res);
static void handleAPIupdate(Request &req, Response &res);
/*Task definition ---------------------------------------------------------------------------------*/
void webServer_Task(void *params){
    /*Init ---------------------------------------------------------------------------------------*/
    //->INDEX
    app.get("/", handleIndexHTML);
    app.get("/index.html", handleIndexHTML);
    app.get("/css/style.css", handleIndexCSS);
    app.get("/js/index.js", handleIndexJS);

    /*API ---------------------------------------------------*/
    app.get("/api/estado_actual", handleAPIEstadoActual);
    app.get("/api/config_data", handleAPIConfigData);
    app.get("/api/medidor", handleAPIMedidor);
    app.post("/api/configuracion", handleAPIConfiguracion);
    app.post("/api/control_manual", handleAPIControlManual);
    app.get("/api/obtener_estados", handleAPIobtenerYActualizarEstados);
    app.get("/update", handleAPIupdate);
    server.begin();
    for(;;){
        //mutex take
        if(xSemaphoreTake(ethernetMutex, portMAX_DELAY) == pdTRUE){
#if (USE_WIIFI == 1)
            WiFiClient client = server.available();
#else
            EthernetClient client = server.available();
#endif
            if (client.connected()) {
                app.process(&client);
                client.stop();
            }
            xSemaphoreGive(ethernetMutex);
            vTaskDelay(50);
        }
        
        //mutex release

    }
}

/*Task definition ---------------------------------------------------------------------------------*/


/*Function definition ----------------------------------------------------------------------------*/
static void fileServer(Request &req, Response &res) {
  if (req.method() != Request::GET) {
    return;
  }

  const char * path = req.path();
  if(strcmp(path, "/") == 0){
    path = indexFile;
  }
  if (!SPIFFS.exists(path)) {
    return;
  }
  Serial.println("REQUEST FOR:" + String(path));
  File file = SPIFFS.open(path);

  if (file.isDirectory()) {
    return;
  }
  
  const char * mimeType = MimeTypes::getType(file.name());
  res.set("Content-Type", mimeType);

  while (file.available()) {
    res.write(file.read());
  }

  res.end();
}
/*Index -------------------------------------------------------------------*/
static void handleIndexHTML(Request &req, Response &res){
  fileServer(req, res);
  return;
}
static void handleIndexCSS(Request &req, Response &res){
  fileServer(req, res);
  return;
}
static void handleIndexJS(Request &req, Response &res){
  fileServer(req, res);
  return;
}


/*Api definition --------------------------------------------------------*/
static void handleAPIEstadoActual(Request &req, Response &res){
  DynamicJsonDocument doc(1024);
  String responseJson;
  res.set("Content-Type", "application/json");
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
  //send response
  serializeJson(doc, responseJson);
  res.printP(responseJson.c_str());
  return;
}

static void handleAPIConfigData(Request &req, Response &res){
   DynamicJsonDocument doc(256);
  String responseJson;
  res.set("Content-Type", "application/json");
  doc["cantAAC"] = sec_AA.SEC_CANT_AA;
  doc["DSEC"] = sec_AA.SEC_DIAS;
  doc["TMINC"] = sec_AA.SEC_TMP_MIN;
  doc["TMAXC"] = sec_AA.SEC_TMP_MAX;
  doc["TALC"] = sec_AA.SEC_TMP_ALARMA;
  doc["TBYC"] = sec_AA.SEC_TMP_BYPASS;
  doc["MINC"] = sec_AA.TIM_ENFRIO_SP;
  //send response
  serializeJson(doc, responseJson);
  res.printP(responseJson.c_str());
}

static void handleAPIMedidor(Request &req, Response &res){
  String responseJson;
  res.set("Content-Type", "application/json");
  DynamicJsonDocument doc(300);
  doc["voltajeRMS"] = sensorData.RMS_V;
  doc["potenciaActiva"] = sensorData.PA;
  doc["potenciaReactiva"] = sensorData.PR;
  doc["potenciaAparente"] = sensorData.PAP;
  doc["energiaActiva"] = sensorData.EA;
  doc["energiaReactiva"] = sensorData.ER;
  doc["factorPotencia"] = sensorData.FP;
  doc["frecuencia"] = sensorData.FREC;
  //send response
  serializeJson(doc, responseJson);
  res.printP(responseJson.c_str());
}

static void handleAPIConfiguracion(Request &req, Response &res){
  String responseJson;
  char  val[10] = {0};
  char buffer[256] = {0};
  res.set("Content-Type", "application/json");
  if (!req.readBytes(buffer, 256)) {
    return res.sendStatus(400);
  }
  DynamicJsonDocument doc(256);
  DeserializationError err = deserializeJson(doc, buffer);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
    return res.sendStatus(500);
  }
  /*Set configuration */
  strcpy(val, doc["cantidad_aires"]);
  //se envia el comando de cambio de cantidad de aires acondicionados
  if(config.SEC_CANT_AA != atoi(val)){
    config.SEC_CANT_AA = atoi(val);
    mib.setCant_AA((const char *)val);
    cmd.command =  CMD_CANT_AA;
    cmd.pData = (const char*) &config.SEC_CANT_AA;
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

  serializeJsonPretty(doc, Serial);
  Serial.println();
  //send response
  serializeJson(doc, responseJson);
  responseJson = "{\"status\":\"success\"}";
  res.printP(responseJson.c_str());
}

void handleAPIControlManual(Request &req, Response &res){
  String responseJson;
  uint8_t cmdVal = 0;
  char buffer[64] = {0};
  res.set("Content-Type", "application/json");
  if (!req.readBytes(buffer, 256)) {
    return res.sendStatus(400);
  }
  Serial.println(String(buffer));
  DynamicJsonDocument doc(64);
  DeserializationError err = deserializeJson(doc, buffer);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
    return res.sendStatus(500);
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
  //send response
  serializeJson(docTx, responseJson);
  res.printP(responseJson.c_str());
}

void handleAPIobtenerYActualizarEstados(Request &req, Response &res){
  String responseJson;
  res.set("Content-Type", "application/json");
  DynamicJsonDocument doc(300);
  doc["modoBypass"] = (dataIn.S_BYPASSS_STS == true) ? "1" : "0";
  doc["modoManual"] = (sec_AA.MANUAL == true) ? "1" : "0";
  doc["aa1"] = (V_AA[0].workig == true) ? "1" : "0";
  doc["aa2"] = (V_AA[1].workig == true) ? "1" : "0";
  doc["aa3"] = (V_AA[2].workig == true) ? "1" : "0";
  doc["aa4"] = (V_AA[3].workig == true) ? "1" : "0";
  //send response
  serializeJson(doc, responseJson);
  res.printP(responseJson.c_str());
}


static void handleAPIupdate(Request &req, Response &res){
    File file = SPIFFS.open("/infor.html");

    if (file.isDirectory()) {
    return;
    }
  
  const char * mimeType = MimeTypes::getType(file.name());
  res.set("Content-Type", mimeType);

  while (file.available()) {
    res.write(file.read());
  }

  res.end();
}