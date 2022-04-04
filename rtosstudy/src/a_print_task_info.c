#include "a_print_task_info.h"
#include "FreeRTOS.h"
#include "task.h"

void *app_task_handle = NULL;

static void app_task_func(void *pvParameters)
{
    char str_task[500];
    while(1){
        vTaskList(str_task);
        printf("print task list info\r\n");
        printf("name\t\tstate\tprio\tstack\tnum\r\n");
        printf("%s\r\n",(char *)str_task);
        vTaskGetRunTimeStats(str_task);
        printf("print task run time state\r\n");
        printf("name\t\ttimes\t\tpercent\r\n");
        printf("%s\r\n",(char *)str_task);
        vTaskDelay(1000);
    }
}

void create_tasks_func(void)
{
    xTaskCreate(app_task_func,"app task",256,NULL,1,&app_task_handle);
}
