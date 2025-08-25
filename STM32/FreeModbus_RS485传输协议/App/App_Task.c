#include "App_Task.h"

/* ************************************************************************ */
/* ******************************任务配置*********************************** */
/* ************************************************************************ */

// 1. 启动任务的配置
#define START_TASK_NAME "start_task"
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 14
TaskHandle_t start_task_handle;
void start_task(void *pvParameters); // 启动任务函数

// 6. Modbus 任务
#define MODBUS_TASK_NAME "modbus_task"
#define MODBUS_TASK_STACK 128
#define MODBUS_TASK_PRIORITY 11
TaskHandle_t modbus_task_handle;
void modbus_task(void *pvParameters);
#define MODBUS_EXEC_CYCLE 10

/* ************************************************************************ */
/* ******************************启动函数*********************************** */
/* ************************************************************************ */

/**
 * @brief 启动FreeRTOS
 */
void App_Task_Start(void)
{
    // 0. 输出提示
    DEBUG_PRINTLN("电机控制项目....");

    // 1. 创建一个启动任务,这个任务负责创建其他任务
    BaseType_t result = xTaskCreate(
        (TaskFunction_t)start_task,
        (char *)"start_task",
        (configSTACK_DEPTH_TYPE)START_TASK_STACK,
        (void *)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (TaskHandle_t *)&start_task_handle);

    if (result == pdPASS)
    {
        DEBUG_PRINTLN("启动任务创建成功...");
    }
    else
    {
        DEBUG_PRINTLN("启动任务创建失败...");
    }

    // 2. 启动调度器(内部会自动创建空闲任务)
    vTaskStartScheduler();
}

/* ************************************************************************ */
/* ******************************创建任务*********************************** */
/* ************************************************************************ */

// 启动任务的任务函数
void start_task(void *pvParameters)
{
    DEBUG_PRINTLN("启动任务开始执行:开始创建其他任务....");
    // 1.进入临界区
    taskENTER_CRITICAL();

    // 6. 创建 Modbus 任务
    BaseType_t modbus_result = xTaskCreate(

        (TaskFunction_t)modbus_task,
        (char *)MODBUS_TASK_NAME,
        (configSTACK_DEPTH_TYPE)MODBUS_TASK_STACK,
        (void *)NULL,
        (UBaseType_t)MODBUS_TASK_PRIORITY,
        (TaskHandle_t *)&modbus_task_handle);
    if (modbus_result == pdPASS)
    {
        DEBUG_PRINTLN("Modbus任务创建成功...");
    }
    else
    {
        DEBUG_PRINTLN("Modbus任务创建失败...");
    }

    // 退出临界区
    taskEXIT_CRITICAL();

    // 6. 删除启动任务
    vTaskDelete(NULL);
}

/* ************************************************************************ */
/* ******************************任务函数*********************************** */
/* ************************************************************************ */

// Modbus 任务函数
void modbus_task(void *pvParameters)
{
    DEBUG_PRINTLN("Modbus任务: 开始调度");
    App_Modbus_Init();
    uint32_t preTime = xTaskGetTickCount();
    while (1)
    {
        // 处理 Modbus 通信
        App_Modbus_Handle();
        vTaskDelayUntil(&preTime, MODBUS_EXEC_CYCLE);
    }
}

