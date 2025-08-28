#include "App_Task.h"

// 1. 启动任务的配置
#define START_TASK_NAME "start_task"
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 14
TaskHandle_t start_task_handle;
void start_task(void *pvParameters); // 启动任务函数

// 2. W5500 任务的配置
#define W5500_TASK_NAME "w5500_task"
#define W5500_TASK_STACK 1024
#define W5500_TASK_PRIORITY 10
TaskHandle_t w5500_task_handle;
void w5500_task(void *pvParameters); // W5500 任务函数
#define W5500_DELAY_TIME 50

/* 其他任务的配置 */
/* code */

/**
 * @brief 启动FreeRTOS
 */
void App_Task_Start(void)
{
    // 0. 输出提示
    DEBUG_PRINTLN("网关项目启动....");

    // 1. 创建一个启动任务，这个任务负责创建其他任务
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

// 启动任务的执行函数
void start_task(void *pvParameters)
{
    DEBUG_PRINTLN("启动任务开始执行: 开始创建其他任务....");
    // 1.进入临界区
    taskENTER_CRITICAL();

    /* 创建其他任务 */
    /* code */

    // 2. 创建 W5500 任务
    BaseType_t w5500_result = xTaskCreate(

        (TaskFunction_t)w5500_task,
        (char *)W5500_TASK_NAME,
        (configSTACK_DEPTH_TYPE)W5500_TASK_STACK,
        (void *)NULL,
        (UBaseType_t)W5500_TASK_PRIORITY,
        (TaskHandle_t *)&w5500_task_handle);
    if (w5500_result == pdPASS)

    {
        DEBUG_PRINTLN("W5500 任务创建成功...");
    }
    else
    {
        DEBUG_PRINTLN("W5500 任务创建失败...");
    }

    // 退出临界区
    taskEXIT_CRITICAL();

    // 4. 删除启动任务
    vTaskDelete(NULL);
}

// W5500 任务的执行函数
void w5500_task(void *pvParameters)
{
    DEBUG_PRINTLN("W5500 任务开始执行...");
    App_W5500_Init();
    // 获取当前时间
    uint32_t current_time = xTaskGetTickCount();
    while (1)
    {
        App_MQTT_Receive();
        xTaskDelayUntil(&current_time, W5500_DELAY_TIME);
    }
}
