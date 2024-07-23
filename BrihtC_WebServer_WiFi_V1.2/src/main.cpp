/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <WiFi.h>
#include <index.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
/*Defines ---------------------------------------------------------------------------------*/
const char* ssid = "RED2";          // Reemplaza con el nombre de tu red Wi-Fi
const char* password = "##KIR@##EPLIMIN";  // Reemplaza con la contraseña de tu red Wi-Fi

/*Global variables -----------------------------------------------------------------------*/
int status = WL_IDLE_STATUS;
//WiFiServer server(80);
String HTTP_header;
AsyncWebServer server(80);
File webFile;

/*Function prototype --------------------------------------------------------------------*/
void printWifiStatus();
void SendResponseHTTP(WiFiClient &client);




// send the state of the switch to the web browser



void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
/*Home ---------------------------------------------------------------------------------*/
void handleRoot(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/index.html", "text/html");
}
void handleJS(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/js/index.js", "application/javascript");
}
void handleCSS(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/css/style.css", "text/css");
}
/*Status actual -----------------------------------------------------------------------*/
void handleStatusActual(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/status_actual.html", "text/html");
}

/*Configuracion ----------------------------------------------------------------------*/
void handleConfiguracion(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/configuracion.html", "text/html");
}
void handleConfiguracionCSS(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/css/configuracion.css", "text/css");
}
void handleConfiguracionJS(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/configuracion.js", "application/javascript");
}

/*Control Manual -----------------------------------------------------------------------*/
void handleControlManual(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/control_manual.html", "text/html");
}
void handleControlManualCSS(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/css/control_manual.css", "text/css");
}
void handleControlManualJS(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/control_manual.js", "application/javascript");
}

/*Medidor electrico*/
void handleMedidorElectrico(AsyncWebServerRequest *request){
  Serial.println("request: medidor_electrico.html");
  request->send(SPIFFS, "/medidor_electrico.html", "text/html");
}
void handleMedidorElectricoCSS(AsyncWebServerRequest *request){
  Serial.println("request: medidor_electrico.css");
  request->send(SPIFFS, "/css/medidor_electrico.css", "text/css");
}
void handleMedidorElectricoJS(AsyncWebServerRequest *request){
  Serial.println("request: medidor_electrico.js");
  request->send(SPIFFS, "/js/medidor_electrico.js", "application/javascript");
}

/*Eventos ------------------------------------------------------------------------------*/
void handleEventos(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/eventos.html", "text/html");
}
void handleEventosCSS(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/css/eventos.css", "text/css");
}
void handleEventosJS(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/js/eventos.js", "application/javascript");
}
/*Info----------------------------------------------------------------------------------*/
void handleInfo(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/informacion.html", "text/html");
}
/*Api Request --------------------------------------------------------------------------*/
void handleAPIEstado_Actual(AsyncWebServerRequest *request){
  //String responseJson;
  AsyncResponseStream *response = request->beginResponseStream("application/json");

  DynamicJsonDocument doc(1024);
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

void handleConfigData(AsyncWebServerRequest *request) {
    if (request->method() == HTTP_POST) {
        String jsonData = request->arg("plain");
        Serial.println("Datos recibidos: " + jsonData);

        // Procesa los datos JSON aquí
        // Por ejemplo, puedes usar la biblioteca ArduinoJson para analizar los datos JSON recibidos

        request->send(200, "application/json", "{ \"status\": \"success\" }");
    } else {
        request->send(405, "text/plain", "Method Not Allowed");
    }
}
/*Setype function ----------------------------------------------------------------------*/
void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    
    // wait 10 seconds for connection:
    delay(1000);
  }
  
  /*SPIFFS Init ------------------------------------------------------------------------*/
  if(SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  if(SPIFFS.exists("/index.html")){
    Serial.println("ERROR - Can't find index.htm file!");
  }
  Serial.println("SUCCESS - Found index.html file.");



  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  /*Home*/
  server.on("/", HTTP_GET, handleRoot);
  server.on("/status_actual.html", HTTP_GET, handleStatusActual);
  server.onNotFound(notFound);
  server.on("/css/style.css", HTTP_GET, handleCSS);
  server.on("/js/index.js", HTTP_GET, handleJS);
  
  
  /*Configuracion*/
  server.on("/configuracion.html", HTTP_GET, handleConfiguracion);
  server.on("/css/configuracion.css", HTTP_GET, handleConfiguracionCSS);
  server.on("/js/configuracion.js", HTTP_GET, handleConfiguracionJS);
  

  /*Control Manual*/
  server.on("/control_manual.html", HTTP_GET, handleControlManual);
  server.on("/css/control_manual.css", HTTP_GET, handleControlManualCSS);
  server.on("/js/control_manual.js", HTTP_GET, handleControlManualJS);

  /*Medidor electrico*/
  server.on("/medidor_electrico.html", HTTP_GET, handleMedidorElectrico);
  server.on("/css/medidor_electrico.css", HTTP_GET, handleMedidorElectricoCSS);
  server.on("/js/medidor_electrico.js", HTTP_GET, handleMedidorElectricoJS);

  /*Eventos*/
  server.on("/eventos.html", HTTP_GET, handleEventos);
  server.on("/css/eventos.css", HTTP_GET, handleEventosCSS);
  server.on("/js/eventos.js", HTTP_GET, handleEventosJS);

  /*Info*/
  server.on("/informacion.html", HTTP_GET, handleInfo);

  /*Api*/
  server.on("/api/estado_actual", HTTP_GET, handleAPIEstado_Actual);
  server.on("/api/config_data", HTTP_GET, handleAPIUpdateConfig);
  server.on("/api/medidor", HTTP_GET, handleAPIMedidor);

  server.on("/api/configuracion", HTTP_POST, handleConfigData);
  /*Server begin*/

  server.begin();

  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop(){

}


/*Function defintion ---------------------------------------------------------------------------*/



void printWifiStatus() {
  // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}