#include "b_timer_test.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

void *timer_task_handle = NULL;
void *p_timer_handle = NULL;
uint8_t timer_id = 0;

void test_timer_callback(void *p_text)
{
    (void)p_text;
    printf("[%s] timeout\r\n",__func__);
    xTimerChangePeriod(p_timer_handle,1500,0);
}

static void timer_task_func(void *pvParameters)
{
    printf("enter task\r\n");
    p_timer_handle = xTimerCreate("test timer",1500,0,&timer_id,test_timer_callback);
    // xTimerStart(p_timer_handle,0);
    xTimerChangePeriod(p_timer_handle,1500,0);
    while(1){
        printf("timer test abcdef\r\n");
        vTaskDelay(1000);
    }
}

void create_timer_task_func(void)
{
    printf("create task\r\n");
    xTaskCreate(timer_task_func,"timer task",256,NULL,1,&timer_task_handle);
}

