#include "following.h"
#include "includes.h"
#include "camera.h"

uint8_t line_pic0[ROW_A/SKIP_FOR_ROW][COL_B/SKIP_FOR_COL] = {0};
uint8_t line_pic1[ROW_A/SKIP_FOR_ROW][COL_B/SKIP_FOR_COL] = {0};
static uint8_t leftBlackLoc[ROW_A/SKIP_FOR_ROW] = {0};
static uint8_t rightBlackLoc[ROW_A/SKIP_FOR_ROW] = {0};
extern QueueHandle_t xQueueLineProcess;
extern QueueHandle_t xQueueCameraReady;

void print_mess_info(uint8_t *p_data,uint16_t length)
{
    uint16_t i;
    for(i=0;i<length;i++){
        printf("%d ",p_data[i]);
    }
    printf("\r\n");
}

void getLineEdge(uint8_t p_line_pic[][COL_B/SKIP_FOR_COL])
{
	uint16_t tempCol = 0;
	uint16_t tmpRow = 0; 
	uint8_t left_loc_get = 0;
	uint8_t right_loc_get = 0;
	
	for(tmpRow = 0;tmpRow < ROW_A/SKIP_FOR_ROW;tmpRow++)
	{
		left_loc_get = 0;
		right_loc_get = 0;
		for(tempCol = 0;tempCol < COL_B/SKIP_FOR_COL - 4;tempCol ++)  //连续判断三个点，所以最后三个点舍去
		{
			if(p_line_pic[tmpRow][tempCol]==1 && p_line_pic[tmpRow][tempCol+1]== 0 && 
				p_line_pic[tmpRow][tempCol+2]==0 && p_line_pic[tmpRow][tempCol+3]==0)
			{
				left_loc_get = 1;
				leftBlackLoc[tmpRow] = tempCol+1;
                tempCol += 2;
                continue;
			}
            if(p_line_pic[tmpRow][tempCol]==0 && p_line_pic[tmpRow][tempCol+1]== 1 && 
				p_line_pic[tmpRow][tempCol+2]==1 && p_line_pic[tmpRow][tempCol+3]==1)
			{
				right_loc_get = 1;
				rightBlackLoc[tmpRow] = tempCol+1;
                tempCol += 2;
                continue;
			}
		}
		if(left_loc_get == 0){
			leftBlackLoc[tmpRow] = 0;
		}
		if(right_loc_get == 0){
			rightBlackLoc[tmpRow] = COL_B/SKIP_FOR_COL;
		}
	}
}

void vTaskFollowing(void *pvParameters)
{
    uint8_t line_picN,ready;
	xQueueSend(xQueueCameraReady,&ready,3000);
    while(1){
        if(xQueueReceive(xQueueLineProcess,&line_picN,0xffffffff)){
            if(line_picN){
                getLineEdge(line_pic1);
            }
            else{
                getLineEdge(line_pic0);
            }
            printf("[%s] the left line loc:\r\n",__func__);
            print_mess_info(leftBlackLoc,ROW_A/SKIP_FOR_ROW);
            printf("[%s] the right line loc:\r\n",__func__);
            print_mess_info(rightBlackLoc,ROW_A/SKIP_FOR_ROW);
			xQueueSend(xQueueCameraReady,&ready,3000);
        }
    }
}
