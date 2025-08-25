#ifndef __APP_TASK_H__
#define __APP_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "Com_Debug.h"
#include "App_Display.h"
#include "App_Key.h"
#include "App_EEPROM.h"
#include "App_Motor.h"
#include "App_Modbus.h"

/**
 * @brief 启动FreeRTOS
 */
void App_Task_Start(void);

/**
 * @brief 启动任务的任务函数
 * 
 * @param pvParameters 
 */

void start_task(void *pvParameters);

#endif /* __APP_TASK_H__ */
