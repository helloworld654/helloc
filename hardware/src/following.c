#include "following.h"
#include "includes.h"
#include "camera.h"

uint8_t line_pic0[ROW_A/SKIP_FOR_ROW][COL_B/SKIP_FOR_COL] = {0};
uint8_t line_pic1[ROW_A/SKIP_FOR_ROW][COL_B/SKIP_FOR_COL] = {0};
extern QueueHandle_t xQueueLineProcess;
extern QueueHandle_t xQueueCameraReady;

void vTaskFollowing(void *pvParameters)
{
    uint8_t line_picN,ready;
	xQueueSend(xQueueCameraReady,&ready,3000);
    while(1){
        if(xQueueReceive(xQueueLineProcess,&line_picN,0xffffffff)){

			xQueueSend(xQueueCameraReady,&ready,3000);
        }
    }
}
