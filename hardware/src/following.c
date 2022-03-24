#include "following.h"
#include "includes.h"

#define ROW_PROCESS    100
#define LINE_PROCESS    100

uint8_t line_pic0[ROW_PROCESS][LINE_PROCESS] = {0};
uint8_t line_pic1[ROW_PROCESS][LINE_PROCESS] = {0};
extern QueueHandle_t xQueueLineProcess;

void vTaskFollowing(void *pvParameters)
{
    uint8_t line_picN = 0;
    while(1){
        if(xQueueReceive(xQueueLineProcess,&line_picN,0xffffffff)){
            printf("get the line_picN:%d\r\n",line_picN);
        }
    }
}
