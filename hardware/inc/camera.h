#ifndef __CAMERA_H
#define __CAMERA_H	

#include "bsp.h"

#define USE_F407ZG_BOARD    0
#define USE_F407VE_BOARD    1

// #define ROW_A    240
// #define COL_B    240
#define ROW_A    180
#define COL_B    180

#define SKIP_FOR_ROW    2
#define SKIP_FOR_COL    2

#define LINE_LOC_NOT_FOND    0xff
#define LINE_EDGE_DEVIATION    5

void camera_init(void);
void update_threshold_through_key(void);
void vTaskCameraCapture(void *pvParameters);
void update_key_for_camera(uint8_t key_num);

#endif
