#include "camera.h"
#include "includes.h"
#include "following.h"
#include "lcd.h"
#include "key.h"

#define LCD_X_MOVE    25
#define LCD_Y_MOVE    25

uint16_t fps_recording = 0;
extern uint8_t line_pic0[ROW_A/2][LINE_B/2];
extern uint8_t line_pic1[ROW_A/2][LINE_B/2];
extern QueueHandle_t xQueueLineProcess;
extern QueueHandle_t xQueueCameraReady;

uint16_t rgb_buf[ROW_A][LINE_B] = {0};
uint16_t gray,num;
uint16_t hang=0;
uint8_t X_MIN,Y_MIN=240;
uint8_t X_MAX,Y_MAX=0; 
uint32_t X_SUM,Y_SUM=0;
uint8_t X,Y;
uint8_t MAX_threshold=47,MIN_threshold=29 ;

void camera_init(void)
{
	fps_recording = 0;
	bsp_InitDWT();   //init for us timer
	LCD_Init();
	start();
    OV2640_OutSize_Set(ROW_A,LINE_B);
    OV2640_RGB565_Mode();	//RGB565模式
    My_DCMI_Init();			//DCMI配置
    DCMI_DMA_Init(rgb_buf,sizeof(rgb_buf)/4,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Enable);//DCMI DMA配置
    DCMI_Start(); 		//启动传输 
	OV2640_Special_Effects(2);
}

void update_threshold_through_key(void)
{ 	
	uint8_t key;
	key=KEY_Scan(0);
	if(key)
	{
		switch(key){
		  	case KEY0_PRES :MIN_threshold++;
			  	printf("get the key0\r\n");
				break;
			case KEY1_PRES :MIN_threshold--;
			  	printf("get the key1\r\n");
				break;
			case WKUP_PRES :MAX_threshold--;
			  	printf("get the wkup\r\n");
				break;
			default:
				break;
		}
	}
}

void vTaskStart(void *pvParameters)
{
	uint8_t i,j,line_pic_send,line_pic_num,ready;
	line_pic_num = line_pic_send =  0;
	printf("enter camera task\r\n");
	camera_init();
	printf("camera init success\r\n");
	xQueueSend(xQueueLineProcess,&line_pic_send,1000);
    while(1)
    {
        if(xQueueReceive(xQueueCameraReady,&ready,0xffffffff)){
			line_pic_send = (++line_pic_num)%2;
			LCD_SetCursor(0,0);  
			LCD_WriteRAM_Prepare();
			hang=0;
			POINT_COLOR=RED;
			for(i=0;i<ROW_A;i++)
			{
				for(j=0;j<LINE_B;j++)
				{
					if(j==(LINE_B-1))
					{
						hang++;
						LCD_SetCursor(LCD_X_MOVE,i+1+LCD_Y_MOVE);  
						LCD_WriteRAM_Prepare();		//开始写入GRAM
					}
					//	LCD->LCD_RAM=rgb_buf[i][j];
					gray=((rgb_buf[ROW_A-i][LINE_B-j]>>11)*19595+((rgb_buf[ROW_A-i][LINE_B-j]>>5)&0x3f)*38469 +(rgb_buf[ROW_A-i][LINE_B-j]&0x1f)*7472)>>16;
					if(gray<=MAX_threshold&&gray>=MIN_threshold)                                   //这里是图像黑白二值化
					{
						num++;
						Y_SUM+=i;
						X_SUM+=j;
						LCD->LCD_RAM=WHITE;
						if(i%2==0 && j%2==0){
							if(line_pic_send)
								line_pic1[i/2][j/2] = 0;
							else
								line_pic0[i/2][j/2] = 0;
						}
					}
					else
					{
						LCD->LCD_RAM=BLACK;
						if(i%2==0 && j%2==0){
							if(line_pic_send)
								line_pic1[i/2][j/2] = 1;
							else
								line_pic0[i/2][j/2] = 1;
						}
					}

				}
			}
			fps_recording++;
			X=(X_SUM)/num;Y=(Y_SUM)/num;	
			LCD_Draw_Circle(X,Y,10);
			LCD_DrawLine(X,Y-10,X,Y+10);
			LCD_DrawLine(X-10,Y,X+10,Y);
			LCD_ShowNum(0,240,X,8,16);
			LCD_ShowNum(0,255,Y,8,16);
			LCD_ShowNum(0,270,MIN_threshold,8,16);
			LCD_ShowNum(0,285,MAX_threshold,8,16);		
			X_SUM=0;Y_SUM=0;num=0;

			xQueueSend(xQueueLineProcess,&line_pic_send,3000);
		}
    }
}
