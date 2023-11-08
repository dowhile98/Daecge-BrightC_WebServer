/**
 * @file RTOS_buzer_task.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-09-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "main.h"


void buzer_task(void *params){

    buzzer_queue_t xQueue;

    while(1){
        if(xQueueReceive(buzer_queue, (void *) &xQueue, portMAX_DELAY) == pdTRUE){
            for(uint8_t i = 0; i < xQueue.repeats; i++){
                digitalWrite(BUZER, HIGH);
                delay(xQueue.delay);
                digitalWrite(BUZER, LOW);
                delay(xQueue.delay);
            }
        }
    }
}