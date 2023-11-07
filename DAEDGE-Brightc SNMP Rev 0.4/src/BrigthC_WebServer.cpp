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
/*Defines ------------------------------------------------------------------------------------*/
#define USE_SERIAL Serial
/*Global variables ---------------------------------------------------------------------------*/
uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 120); 


EthernetServer ethServer(80);
/*Function prototype -------------------------------------------------------------------------*/

/*Task definition ----------------------------------------------------------------------------*/

void webServer_Task(void *params){
    SPI.begin(ETH_SCLK, ETH_MISO, ETH_MOSI);
    pinMode(ETH_RST, OUTPUT); 
    digitalWrite(ETH_RST, HIGH);
    delay(250);
    digitalWrite(ETH_RST, LOW);
    delay(50);
    digitalWrite(ETH_RST, HIGH);
    delay(350);
    delay(1000);
    Ethernet.begin(mac, ip);
    Serial.print("Ethernet IP es: ");
    Serial.println(Ethernet.localIP());

    
    for(;;){
    }
}
