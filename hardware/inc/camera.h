#ifndef __CAMERA_H
#define __CAMERA_H	

// #define ROW_A    240
// #define COL_B    240
#define ROW_A    180
#define COL_B    180

#define SKIP_FOR_ROW    2
#define SKIP_FOR_COL    2

void camera_init(void);
void update_threshold_through_key(void);
void vTaskStart(void *pvParameters);

#endif
