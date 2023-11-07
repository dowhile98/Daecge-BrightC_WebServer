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
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>

#include <WiFi.h>


void webServer_Task_WiFi(void *params);
void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(webServer_Task_WiFi, "WiFi Server", 8000, NULL, 4, NULL, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
}

