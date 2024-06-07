/**
 * @file RTOS_input_read_task.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "main.h"
#include "SNMP_OID.h"
/*Local variables -----------------------------------------------------------------------------------------------------*/
float voltagePM1=0.0, voltagePM2=0.0,powerPM1=0.0,powerPM2=0.0,energyPM1=0.0,energyPM2=0.0,frequencyPM1=0.0,frequencyPM2=0.0;
double PT=0.0,QT=0.0,ST=0.0;


extern MIB mib;
extern SNMP::Agent snmp;

InputState_t IN1;
InputState_t IN2;
InputState_t IN3;
InputState_t IN4;
InputState_t IN5;
InputState_t IN6;
InputState_t IN7;
InputState_t IN8;
InputState_t BYPS;
InputState_t tempHigh;
InputState_t fire;
/*Typedefs ------------------------------------------------------------------------------------------------------------*/

/*Functions -----------------------------------------------------------------------------------------------------------*/
void pzem_read_measurement(void);


void brightC_trapSend(void);
uint8_t inputRead(InputState_t &button, uint8_t current);
void initInput(InputState_t &button, uint8_t alarmState);
/*Task definition -----------------------------------------------------------------------------------------------------*/
void input_read_task(void *params){
    /*Local variables ----------------------------------*/
    uint64_t last_read_temp = millis();
    uint64_t last_input_read = millis();
    uint64_t last_pzem_read = millis();
    SNMP::Message *message;
    LCD_queue_t xQueue_lcd;
    //Todo
    temp_sensors_read();
    pzem_read_measurement();



    dataIn.in1 = PCF1.digitalRead(P_AAA1_AS);
    dataIn.in2 = PCF1.digitalRead(P_AAA1_AABP);
    dataIn.in3 = PCF1.digitalRead(P_AAA2_AS);
    dataIn.in4 = PCF1.digitalRead(P_AAA2_AABP);
    dataIn.in5 = PCF1.digitalRead(P_AAA3_AS);
    dataIn.in6 = PCF1.digitalRead(P_AAA3_AABP);
    dataIn.in7 = PCF1.digitalRead(P_AAA4_AS);
    dataIn.in8 = PCF1.digitalRead(P_AAA4_AABP);
    dataIn.A_Incendio = (digitalRead(P_A_Incendio) == 1) ? 0 : 1;
    mib.setMeasurement(sensorData);
    uint8_t stateRead = 0;
    initInput(IN1, dataIn.in1);
    initInput(IN2, dataIn.in2);
    initInput(IN3, dataIn.in3);
    initInput(IN4, dataIn.in4);
    initInput(IN5, dataIn.in5);
    initInput(IN6, dataIn.in6);
    initInput(IN7, dataIn.in7);
    initInput(IN8, dataIn.in8);
    initInput(BYPS, dataIn.S_BYPASSS_STS);
    initInput(tempHigh, dataIn.TMP_High);
    initInput(fire, dataIn.A_Incendio);
    while(1){
        //lectura de los sensores de temperatura
        if((millis() - last_read_temp)> TEMP_SENSORS_READ * 1000){
            last_read_temp = millis();
            // switch (stateRead)
            // {
            // case 1:
            //     /* code */
            //     sensorData.TMP1 = 21.5;
            //     sensorData.TMP2 = 20.4;
            //     break;
            // case 2:
            //     sensorData.TMP1 = 26.1;
            //     sensorData.TMP2 = 26.8;
            //     break;
            // case 3:
            //     sensorData.TMP1 = 28.6;
            //     sensorData.TMP2 = 27.8;
            //     break;
            // case 4:
            //     sensorData.TMP1 = 30.1;
            //     sensorData.TMP2 = 32.4;
            //     stateRead = 0;
            //     break;
            // default:

            //     break;
            // }
            // SERIAL_PRINT("-->(TEMP READ)DSB1: "); 
            // SERIAL_PRINT(sensorData.TMP1);
            // SERIAL_PRINT("  DSB2: "); 
            // SERIAL_PRINTLN(sensorData.TMP2);
            //stateRead += 1;
            temp_sensors_read();
            mib.setMeasurement(sensorData);
            last_read_temp = millis();
            //todo
            
        }
        //lectura de entradas 
        if((millis() - last_input_read)>200){
            last_input_read = millis();
            //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
            brightC_trapSend();
            //xSemaphoreGive(ethernetMutex);
        }
        if((millis() - last_pzem_read) >= 60000){
            last_pzem_read = millis();
            pzem_read_measurement();
            mib.setMeasurement(sensorData);
            
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
/*Function definition -----------------------------------------------------------------*/
void brightC_trapSend(void){
    /*Local variables ----------------------------*/
    SNMP::Message *message;
    String msg = "";
    uint8_t severity = 0;
    /*input read*/
    dataIn.in1 = PCF1.digitalRead(P_AAA1_AS);
    dataIn.in2 = PCF1.digitalRead(P_AAA1_AABP);
    dataIn.in3 = PCF1.digitalRead(P_AAA2_AS);
    dataIn.in4 = PCF1.digitalRead(P_AAA2_AABP);
    dataIn.in5 = PCF1.digitalRead(P_AAA3_AS);
    dataIn.in6 = PCF1.digitalRead(P_AAA3_AABP);
    dataIn.in7 = PCF1.digitalRead(P_AAA4_AS);
    dataIn.in8 = PCF1.digitalRead(P_AAA4_AABP);
    dataIn.A_Incendio = (digitalRead(P_A_Incendio) == 1) ? 0 : 1;
    mib.setInputVal(dataIn);
    /*Se verifican los traps*/
    //bypass trap
    if(inputRead(BYPS, dataIn.S_BYPASSS_STS) == 1){
        msg = (dataIn.S_BYPASSS_STS == true) ? "ACTIVACION ALARMA BYPASS" : "CANCELACION ALARMA BYPASS";
        severity = (dataIn.S_BYPASSS_STS == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        //message = mib.trap(ip, msg.c_str(), severity, TRAP_BYPASS);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_BYPASS);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    //high temperature
    if(inputRead(tempHigh, dataIn.TMP_High)){
        msg = (dataIn.TMP_High == true) ? "ACTIVACION ALARMA TEMPERATURA ALTA" : "CANCELACION ALARMA TEMPERATURA ALTA";
        severity = (dataIn.TMP_High == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        //message = mib.trap(ip, msg.c_str(), severity, TRAP_TEMPHIGH);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_TEMPHIGH);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    //AA1
    if(inputRead(IN1, dataIn.in1)){
        msg = (dataIn.in1 == true) ? "ACTIVASION ALARMA DE PRESION AA3" : "CANCELACION ALARMA DE PRESION AA3";
        severity = (dataIn.in1 == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_AS1);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    if(inputRead(IN2, dataIn.in2)){
        msg = (dataIn.in2 == true) ? "ACTIVASION ALARMA DE PRESION AA4" : "CANCELACION ALARMA DE PRESION AA4";
        severity = (dataIn.in2== true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_AAPB1);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    //AA2
    if(inputRead(IN3, dataIn.in3)){
        msg = (dataIn.in3 == true) ? "ACTIVASION ALARMA DE SUMINISTRO AA2" : "CANCELACION ALARMA DE SUMINISTRO AA2";
        severity = (dataIn.in3 == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_AS2);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    if(inputRead(IN4, dataIn.in4)){
        msg = (dataIn.in4 == true) ? "ACTIVACION ALARMA DE SUMINISTROS AA4" : "CANCELACION ALARMA DE SUMINISTROS AA4";
        severity = (dataIn.in4 == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_AAPB2);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    //AA3
    if(inputRead(IN5, dataIn.in5)){
        msg = (dataIn.in5 == true) ? "ACTIVACION ALARMA DE PRESION AA2" : "CANCELACION ALARMA DE PRESION AA2";
        severity = (dataIn.in5 == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_AS3);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    if(inputRead(IN6, dataIn.in6)){
        msg = (dataIn.in6 == true) ? "ACTIVASION ALARMA DE SUMINISTRO PRESION AA1" : "CANCELACION ALARMA DE SUMINISTRO PRESION AA1";
        severity = (dataIn.in6 == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_AAPB3);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    //AA4
    if(inputRead(IN7, dataIn.in7)){
        msg = (dataIn.in7 == true) ? "ACTIVASION ALARMA DE BAJA PRESION AA1" : "CANCELACION ALARMA DE BAJA PRESION AA1";
        severity = (dataIn.in7 == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_AS4);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    if(inputRead(IN8, dataIn.in8)){
        msg = (dataIn.in8 == true) ? "ACTIVASION ALARMA DE PRESION AA1" : "CANCELACION ALARMA DE PRESION AA1";
        severity = (dataIn.in8 == true) ? TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_AAPB4);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
    //A INCENDIO
    if(inputRead(fire, dataIn.A_Incendio)){
        msg = (dataIn.A_Incendio == true) ? "ACTIVASION ALARMA DE INCENDIO" : "CANCELACION ALARMA DE INCENDIO";
        severity = (dataIn.A_Incendio == true) ?  TRAP_SEVERITY_CRITICAL : TRAP_SEVERITY_WARNING;
        //take mutex
        //xSemaphoreTake(ethernetMutex, portMAX_DELAY);
        message = mib.trap(net.ip, msg.c_str(), severity, TRAP_INCENDIO);
        snmp.send(message, net.IPSERVER, SNMP::PORT::TRAP);
        delete message;
        //release mutex
        //xSemaphoreGive(ethernetMutex);
        return;
    }
}

void pzem_read_measurement(void){
    voltagePM1=pmeter1.voltage();
    powerPM1=pmeter1.power();
    energyPM1=pmeter1.energy();
    frequencyPM1=pmeter1.frequency();
    // Read the data from Power Meter 2
    voltagePM2=pmeter2.voltage();
    powerPM2=pmeter2.power();
    energyPM2=pmeter2.energy();
    frequencyPM2=pmeter2.frequency();
    if( !(isnan(powerPM1) || isnan(powerPM2))  ) {          //Update if no error only
        PT=(double) (powerPM1 +  powerPM2);
        QT=sqrt(3)*fabs( (double) (powerPM1 -  powerPM2) );
        ST=sqrt (   PT*PT + QT*QT  );                       // S= sqrt (P^2 + Q^2)
        sensorData.PA = PT;
        sensorData.PR = QT;
        sensorData.PAP = ST;
        sensorData.FP = (PT / ST);  
    }else{
        SERIAL_PRINTLN("PZEM->POWER IS NAN");
    }
    //voltaje
    if(!(isnan(voltagePM1) || isnan(voltagePM2))){
        sensorData.RMS_V = (voltagePM1+voltagePM2) / 2;
    }else{
        SERIAL_PRINTLN("PZEM -> VOLTAJE IS NAN");
    }
    //energia
    if(isnan(energyPM1) || isnan(energyPM2)){
        sensorData.EA = (energyPM1 + energyPM2);
        sensorData.ER = sqrt(3)*fabs( (double) (energyPM1 - energyPM2) );
    }else{
        SERIAL_PRINTLN("PZEM->ENERGYPM IS NAN");
    }
    //frecuencia
    if(isnan(frequencyPM1) || isnan(frequencyPM2)){
        sensorData.FREC = max(frequencyPM1,frequencyPM2);
    }else{
        SERIAL_PRINTLN("PZEM->FREQUENCY IS NAN");
    }
}

uint8_t inputRead(InputState_t &button, uint8_t current){
    uint8_t current_state = current;
    uint8_t output = 0;
    //filter debounce
    if((millis() - button.last_change_time)>=20) //lo usual es 20ms
    {
        button.last_change_time = millis();
        if(button.previous_state != current_state){
            //detect event
            output = 1;
            button.previous_state = current_state;
        }
    }
    return output;
}


void initInput(InputState_t &button, uint8_t alarmState){
    button.previous_state = alarmState;
    button.last_change_time = millis();
}