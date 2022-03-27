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
			if(tempCol == 0){
				if(p_line_pic[tmpRow][tempCol]==0 && p_line_pic[tmpRow][tempCol+1]== 0 && p_line_pic[tmpRow][tempCol+2]==0)
				{
					left_loc_get = 1;
					leftBlackLoc[tmpRow] = tempCol;
					tempCol += 2;
					continue;
				}
			}
			if(!left_loc_get){
				if(p_line_pic[tmpRow][tempCol]==1 && p_line_pic[tmpRow][tempCol+1]== 0 && 
					p_line_pic[tmpRow][tempCol+2]==0 && p_line_pic[tmpRow][tempCol+3]==0)
				{
					left_loc_get = 1;
					leftBlackLoc[tmpRow] = tempCol+1;
					tempCol += 2;
					continue;
				}
			}
			if(!right_loc_get){
				if(p_line_pic[tmpRow][tempCol]==0 && p_line_pic[tmpRow][tempCol+1]== 1 && 
					p_line_pic[tmpRow][tempCol+2]==1 && p_line_pic[tmpRow][tempCol+3]==1)
				{
					right_loc_get = 1;
					rightBlackLoc[tmpRow] = tempCol+1;
					tempCol += 2;
					continue;
				}
			}
			if(left_loc_get==1 && right_loc_get==1)
				break;
		}
		if(left_loc_get == 0){
			leftBlackLoc[tmpRow] = LINE_LOC_NOT_FOND;
		}
		if(right_loc_get == 0){
			rightBlackLoc[tmpRow] = LINE_LOC_NOT_FOND;
		}
	}
}

// get the useful part line of needblackloc line
void getOneSideUsefulLine(uint8_t *needBlackLoc,uint8_t countZero,uint8_t *maxUBlackLineX,uint8_t *maxUBlackLineY,uint8_t *maxUBlackLineLen)
{
	uint8_t i,j;
	uint8_t tmpMaxUsefulLineLen = 0;
	uint8_t tmpMaxUsefulLineX[ROW_A/SKIP_FOR_ROW]  = {0};
	uint8_t tmpMaxUsefulLineY[ROW_A/SKIP_FOR_ROW] = {0};
	*maxUBlackLineLen = 0;
	
	for(i = countZero;i < ROW_A/SKIP_FOR_ROW - 2;i ++)
	{
		//  needBlackLoc[i+2] point is an empyt point,so record this part line
		if(needBlackLoc[i+2] == LINE_LOC_NOT_FOND)
		{
			//  record the get part line when the length is big than last one
			if((*maxUBlackLineLen) < tmpMaxUsefulLineLen)
			{
				(*maxUBlackLineLen) = tmpMaxUsefulLineLen;
				memset(maxUBlackLineX,0,ROW_A/SKIP_FOR_ROW);
				memset(maxUBlackLineY,0,ROW_A/SKIP_FOR_ROW);
				for(j = 0;j <= tmpMaxUsefulLineLen;j ++)
				{
					maxUBlackLineX[j] = tmpMaxUsefulLineX[j];
					maxUBlackLineY[j] = tmpMaxUsefulLineY[j];
				}
			}
			//  clear the temp line reocrding verible
			memset(tmpMaxUsefulLineX,0,ROW_A/SKIP_FOR_ROW);
			memset(tmpMaxUsefulLineY,0,ROW_A/SKIP_FOR_ROW);
			tmpMaxUsefulLineLen = 0;
			//  skip the already get line
			if((i + 3) < ROW_A/SKIP_FOR_ROW)
			{
				i += 3;
			}
			//  always skip for continue empty point
			while((i < (ROW_A/SKIP_FOR_ROW - 2)) && (needBlackLoc[i] == LINE_LOC_NOT_FOND))
			{
				i ++;
			}
			continue;
		}
		//  recording the point to line when the three points are continue
		if(needBlackLoc[i]!=LINE_LOC_NOT_FOND && needBlackLoc[i+1]!=LINE_LOC_NOT_FOND &&
			abs((needBlackLoc[i] - needBlackLoc[i+1]) - (needBlackLoc[i+1] - needBlackLoc[i+2]))<=LINE_EDGE_DEVIATION)
		{
			tmpMaxUsefulLineX[tmpMaxUsefulLineLen] = needBlackLoc[i];
			tmpMaxUsefulLineY[tmpMaxUsefulLineLen] = i*SKIP_FOR_ROW;
			tmpMaxUsefulLineLen++;
		}
	}
	
	if((*maxUBlackLineLen) < tmpMaxUsefulLineLen)
	{
		(*maxUBlackLineLen) = tmpMaxUsefulLineLen;
		memset(maxUBlackLineY,0,ROW_A/SKIP_FOR_ROW);
		memset(maxUBlackLineX,0,ROW_A/SKIP_FOR_ROW);
		for(j = 0;j <= tmpMaxUsefulLineLen;j ++)
		{
			maxUBlackLineX[j] = tmpMaxUsefulLineX[j];
			maxUBlackLineY[j] = tmpMaxUsefulLineY[j];
		}
	}
	
}

uint8_t line_regression_calcu(uint8_t *maxLineX,uint8_t *maxLineY,uint8_t maxLineLen,double *oK,int *ob)
{
	double x_sum = 0,y_sum = 0,xy_sum = 0,xx_sum = 0;
	double x_avr = 0,y_avr = 0,xy_avr = 0,xx_avr = 0;
	uint8_t i;
	x_sum=0;y_sum=0;xy_sum=0;xx_sum=0;
	
	for(i=0;i < maxLineLen;i++)
	{
		x_sum += maxLineX[i];
		y_sum += maxLineY[i];
		xy_sum+=maxLineX[i]*maxLineY[i];
		xx_sum+=maxLineX[i]*maxLineX[i];
	}

	x_avr=x_sum/(maxLineLen);
	y_avr=y_sum/(maxLineLen);
	xy_avr=xy_sum/(maxLineLen);
	xx_avr=xx_sum/(maxLineLen);
	
	if((xy_avr-x_avr*y_avr)==0)
	{
		*oK = 0xff;
		return 0;
	}
	else
	{
		*oK = (x_avr*x_avr-xx_avr)/(xy_avr-x_avr*y_avr);    // calcu the 1/k
		*ob = maxLineY[0]-(1/(*oK))*(maxLineX[0]);		//  calcu the b
		return 1;
	}
}

uint8_t black_line_process(void)
{
	uint8_t leftLineGet,rightLineGet,countLeftZero,countRightZero;
	// the left line info
	uint8_t maxUsefulLeftLineX[ROW_A/SKIP_FOR_ROW] = {0};
	uint8_t maxUsefulLeftLineY[ROW_A/SKIP_FOR_ROW] = {0};
	uint8_t maxUsefulLeftLength = 0;
	double left_K;
	int left_B;
	// the right line info
	uint8_t maxUsefulRightLineX[ROW_A/SKIP_FOR_ROW] = {0};
	uint8_t maxUsefulRightLineY[ROW_A/SKIP_FOR_ROW] = {0};
	uint8_t maxUsefulRightLength = 0;
	double right_K;
	int right_B;

	leftLineGet = rightLineGet = countLeftZero = countRightZero = 0;

	// skip the empty point for leftblackloc
	while((countLeftZero < ROW_A/SKIP_FOR_ROW/2) && (leftBlackLoc[countLeftZero] == LINE_LOC_NOT_FOND))
	{
		countLeftZero ++;
	}
	if(countLeftZero < ROW_A/SKIP_FOR_ROW/2)
	{
		leftLineGet = 1;
	}
	
	// skip the empty point for tightblackloc
	while((countRightZero < ROW_A/SKIP_FOR_ROW/2) && (rightBlackLoc[countRightZero] == LINE_LOC_NOT_FOND))
	{
		countRightZero ++;
	}
	if(countRightZero < ROW_A/SKIP_FOR_ROW/2)
	{
		rightLineGet = 1;
	}
	
	// return false when not get the useful point for blackloc
	if((leftLineGet == 0) && (rightLineGet == 0))
	{
		return 0;
	}

	if(leftLineGet == 1)
	{
		getOneSideUsefulLine(leftBlackLoc,countLeftZero,maxUsefulLeftLineX,maxUsefulLeftLineY,&maxUsefulLeftLength);
		line_regression_calcu(maxUsefulLeftLineX,maxUsefulLeftLineY,maxUsefulLeftLength,&left_K,&left_B);
#if 1
		printf("the left info left_K:%f,left_B:%d\r\n",left_K,left_B);
#endif
#if 0
		printf("[%s] the left max useful line %d loc:\r\n",__func__,maxUsefulLeftLength);
		print_mess_info(maxUsefulLeftLineX,maxUsefulLeftLength);
#endif
	}
	
	if(rightLineGet == 1)
	{
		getOneSideUsefulLine(rightBlackLoc,countRightZero,maxUsefulRightLineX,maxUsefulRightLineY,&maxUsefulRightLength);
#if 0
		printf("[%s] the right max useful line %d loc:\r\n",__func__,maxUsefulRightLength);
		print_mess_info(maxUsefulRightLineX,maxUsefulRightLength);
#endif
	}
	
	if(maxUsefulLeftLength<=3 && maxUsefulRightLength<=3)
	{
		return 0;
	}
	return 1;
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
			black_line_process();
#if 0
            printf("[%s] the left line loc:\r\n",__func__);
            print_mess_info(leftBlackLoc,ROW_A/SKIP_FOR_ROW);
            printf("[%s] the right line loc:\r\n",__func__);
            print_mess_info(rightBlackLoc,ROW_A/SKIP_FOR_ROW);
#endif
			xQueueSend(xQueueCameraReady,&ready,3000);
        }
    }
}
