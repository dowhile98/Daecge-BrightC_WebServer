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
/*Defines -------------------------------------------------------------------*/


/*Global variables ----------------------------------------------------------*/
bool ledState = 0;

AsyncWebServer WiFiServer(80);
AsyncWebSocket ws("/ws");
/*Function prototype --------------------------------------------------------*/
void notifyClients() {
  ws.textAll(String(ledState));
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      ledState = !ledState;
      notifyClients();
    }
  }
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}


String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

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
/*Task definition ----------------------------------------------------------*/
void webServer_Task_WiFi(void *params){
  pinMode(2, OUTPUT);
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }else{
    Serial.println("SPIFFS mounted successfully");
  }
  
  // Serial.println("[WIFI]->Init AP (access Point)");
  // WiFi.softAP("BrightC_Manager", NULL);

  // IPAddress IP = WiFi.softAPIP();
  // Serial.print("AP IP addres: ");
  // Serial.println(IP);


  WiFi.begin("RQUINOB", "R#29qbCivil");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("RQUINOB");
    delay(1000);
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  //web socket init
  // ws.onEvent(onEvent);
  // WiFiServer.addHandler(&ws);

  // WiFiServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send(SPIFFS, "/WiFiServer/index.html", "text/html");
  // });

  WiFiServer.on("/", HTTP_GET, handleRoot);
  WiFiServer.onNotFound(notFound);
  WiFiServer.on("/css/style.css", HTTP_GET, handleCSS);
  WiFiServer.on("/js/index.js", HTTP_GET, handleJS);

  /*Api init ----------------------------------------------*/
  WiFiServer.on("/api/estado_actual", HTTP_GET, handleAPIEstado_Actual);
  WiFiServer.on("/api/config_data", HTTP_GET, handleAPIUpdateConfig);
  WiFiServer.on("/api/medidor", HTTP_GET, handleAPIMedidor);
  WiFiServer.on("/api/obtener_estados", HTTP_GET,handleAPIobtenerYActualizarEstados);
  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/api/configuracion", handleInputConfig);
  WiFiServer.addHandler(handler);
  handler = new AsyncCallbackJsonWebHandler("/api/control_manual", handleAPIControlManual);
  WiFiServer.addHandler(handler);
  AsyncElegantOTA.begin(&WiFiServer, "admin", "admin");    // Start ElegantOTA
  
  WiFiServer.begin();

  Serial.println("[WIFI] HTTP Server started");
  for(;;){
    // ws.cleanupClients();
    // digitalWrite(2, ledState);
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

void handleAPIEstado_Actual(AsyncWebServerRequest *request){
  //String responseJson;
  AsyncResponseStream *response = request->beginResponseStream("application/json");

  DynamicJsonDocument doc(1024);
  doc["ubicacion"] = "Bright-C: La cultura";
  doc["cantAA"] = 3;
  doc["tempSensor1"] = 22.1;
  doc["tempSensor2"] = 23.1;

  //AA1
  doc["turno1"] = "SI";
  doc["estadoAA1"] = "encendido";
  doc["ayuda1"] = "NO";
  doc["contadorFallas1"] = 0;
  //AA2
  doc["turno2"] = "SI";
  doc["estadoAA2"] = "encendido";
  doc["ayuda2"] = "NO";
  doc["contadorFallas2"] = 1;
  //AA3
  doc["turno3"] = "SI";
  doc["estadoAA3"] = "encendido";
  doc["ayuda3"] = "NO";
  doc["contadorFallas3"] = 2;
  //AA4
  doc["turno4"] = "SI";
  doc["estadoAA4"] = "encendido";
  doc["ayuda4"] = "NO";
  doc["contadorFallas4"] = 3;

  //alarmas
  doc["bypass"] = "MODO BYPASS ACTIVO";
  doc["A_SAA1"] = "Activo";
  doc["A_ABAA1"] = "Activo";
  doc["A_SAA2"] = "Activo";
  doc["A_ABAA2"] = "Activo";
  doc["A_SAA3"] = "Activo";
  doc["A_ABAA3"] = "Activo";
  doc["A_SAA4"] = "Activo";
  doc["A_ABAA4"] = "Activo";
  doc["AFIRE"] = "Activo";
  doc["ATEMH"] = "Activo";
  //serialice
  serializeJson(doc, *response);
  request->send(response);
}

void handleAPIUpdateConfig(AsyncWebServerRequest *request){
  //String responseJson;
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonDocument doc(256);
  doc["cantAAC"] = 2;
  doc["DSEC"] = 15;
  doc["TMINC"] = 19.0;
  doc["TMAXC"] = 23.5;
  doc["TALC"] = 26.7;
  doc["TBYC"] = 30;
  doc["MINC"] = 10;
  serializeJson(doc, *response);
  request->send(response);
}

void handleAPIMedidor(AsyncWebServerRequest *request){
  Serial.println("request data medidor electrico");
  //String responseJson;
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonDocument doc(300);
  doc["voltajeRMS"] = 380;
  doc["potenciaActiva"] = 100;
  doc["potenciaReactiva"] = 50;
  doc["potenciaAparente"] = 15;
  doc["energiaActiva"] = 123;
  doc["energiaReactiva"] = 30;
  doc["factorPotencia"] = 10;
  doc["frecuencia"] = 60;
  serializeJson(doc, *response);
  request->send(response);
}

void handleInputConfig(AsyncWebServerRequest *request, JsonVariant &json){
  StaticJsonDocument<200> data;
  if (json.is<JsonArray>())
  {
    data = json.as<JsonArray>();
  }
  else if (json.is<JsonObject>())
  {
    data = json.as<JsonObject>();
  }
  String response;
  serializeJson(data, response);
  request->send(200, "application/json", response);
  Serial.println(response);
}

void handleAPIControlManual(AsyncWebServerRequest *request, JsonVariant &json){
  StaticJsonDocument<200> data;
  if (json.is<JsonArray>())
  {
    data = json.as<JsonArray>();
  }
  else if (json.is<JsonObject>())
  {
    data = json.as<JsonObject>();
  }
  String response;
  serializeJson(data, response);
  request->send(200, "application/json", response);
  Serial.println(response);
}

void handleAPIobtenerYActualizarEstados(AsyncWebServerRequest *request){
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  DynamicJsonDocument doc(256);
  doc["modoBypass"] =  "0";
  doc["modoManual"] =  "0";
  doc["aa1"] = "1" ;
  doc["aa2"] =  "1" ;
  doc["aa3"] = "0";
  doc["aa4"] =  "0";
  serializeJson(doc, *response);
  request->send(response);
}