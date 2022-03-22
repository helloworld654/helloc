#include "camera.h"
#include "bsp.h"
#include "lcd.h"
#include "key.h"

// int A=240,B=240;
// static uint16_t rgb_buf[240][240] = {0};
int A=200,B=200;
uint16_t rgb_buf[200][200] = {0};
uint16_t gray,num;
uint16_t hang=0;
uint8_t X_MIN,Y_MIN=240;
uint8_t X_MAX,Y_MAX=0; 
uint32_t X_SUM,Y_SUM=0;
uint8_t X,Y;
uint8_t MAX_threshold=47,MIN_threshold=29 ;

void camera_init(void)
{
	bsp_InitDWT();
	LCD_Init();
	printf("aa\r\n");
	start();
	printf("bb\r\n");
	KEY_Init();
	printf("d\r\n");
    OV2640_OutSize_Set(A,B);
    OV2640_RGB565_Mode();	//RGB565模式
    My_DCMI_Init();			//DCMI配置
	printf("e\r\n");
    DCMI_DMA_Init(rgb_buf,sizeof(rgb_buf)/4,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Enable);//DCMI DMA配置
    DCMI_Start(); 		//启动传输 
	OV2640_Special_Effects(2);
}

void key()
{ uint8_t key;
	key=KEY_Scan(0);
	if(key)
	{
		switch(key){
		  case KEY0_PRES :MIN_threshold++;break;
			case KEY1_PRES :MIN_threshold--;break;
			case KEY2_PRES :MAX_threshold++;break;
			case WKUP_PRES :MAX_threshold--;break;
		
	}
	
}
}

// void camera_processing(void)
// {
//     uint16_t i,j;
// 		LCD_SetCursor(0,0);  
// 		LCD_WriteRAM_Prepare();
// 		hang=0;
// 		POINT_COLOR=RED;
// 		key();
		
// 				for(i=0;i<B;i++)
//             {
//                 for(j=0;j<A;j++)
//                 {
//                     if(j==(A-1))
//                     {
//                         hang++;
//                         LCD_SetCursor(0,i+1);  
//                         LCD_WriteRAM_Prepare();		//开始写入GRAM
//                     }
// 									//	LCD->LCD_RAM=rgb_buf[i][j];
// 										// gray=((rgb_buf[i][j]>>11)*19595+((rgb_buf[i][j]>>5)&0x3f)*38469 +(rgb_buf[i][j]&0x1f)*7472)>>16;
//                     if(gray<=MAX_threshold&&gray>=MIN_threshold)                                   //这里是图像黑白二值化
//                     {
// 											num++;
// 											Y_SUM+=i;
// 											X_SUM+=j;
// 											LCD->LCD_RAM=WHITE;
//                     }
//                     else
//                     {
// 											LCD->LCD_RAM=BLACK;
//                     }

//                 }
//             }
						
						
						
// 		X=(X_SUM)/num;Y=(Y_SUM)/num;	
// 		LCD_Draw_Circle(X,Y,10);
// 		LCD_DrawLine(X,Y-10,X,Y+10);
// 		LCD_DrawLine(X-10,Y,X+10,Y);
// 		LCD_ShowNum(0,240,X,8,16);
// 		LCD_ShowNum(0,255,Y,8,16);
// 		LCD_ShowNum(0,270,MIN_threshold,8,16);
// 		LCD_ShowNum(0,285,MAX_threshold,8,16);		
// 		X_SUM=0;Y_SUM=0;num=0;
// }

#include "FreeRTOS.h"
#include "task.h"
#include "includes.h"
void vTaskStart(void *pvParameters)
{
	printf("enter task\r\n");
	camera_init();
	printf("camera init success\r\n");
    while(1)
    {
    	uint16_t i,j;
		LCD_SetCursor(0,0);  
		LCD_WriteRAM_Prepare();
		hang=0;
		POINT_COLOR=RED;
		key();
		
				for(i=0;i<B;i++)
            {
                for(j=0;j<A;j++)
                {
                    if(j==(A-1))
                    {
                        hang++;
                        LCD_SetCursor(0,i+1);  
                        LCD_WriteRAM_Prepare();		//开始写入GRAM
                    }
									//	LCD->LCD_RAM=rgb_buf[i][j];
										gray=((rgb_buf[i][j]>>11)*19595+((rgb_buf[i][j]>>5)&0x3f)*38469 +(rgb_buf[i][j]&0x1f)*7472)>>16;
                    if(gray<=MAX_threshold&&gray>=MIN_threshold)                                   //这里是图像黑白二值化
                    {
											num++;
											Y_SUM+=i;
											X_SUM+=j;
											LCD->LCD_RAM=WHITE;
                    }
                    else
                    {
											LCD->LCD_RAM=BLACK;
                    }

                }
            }
						
						
						
		X=(X_SUM)/num;Y=(Y_SUM)/num;	
		LCD_Draw_Circle(X,Y,10);
		LCD_DrawLine(X,Y-10,X,Y+10);
		LCD_DrawLine(X-10,Y,X+10,Y);
		LCD_ShowNum(0,240,X,8,16);
		LCD_ShowNum(0,255,Y,8,16);
		LCD_ShowNum(0,270,MIN_threshold,8,16);
		LCD_ShowNum(0,285,MAX_threshold,8,16);		
		X_SUM=0;Y_SUM=0;num=0;
		vTaskDelay(5);
    }
}

// void vTaskStart(void *pvParameters)
// {
// 	printf("enter task\r\n");
//     while(1)
//     {
// 		vTaskDelay(1000);
// 		printf("[%s] every second\r\n",__func__);
//     }
// }
