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

// 3. Modbus 任务的配置
#define MODBUS_TASK_NAME "modbus_task"
#define MODBUS_TASK_STACK 1024
#define MODBUS_TASK_PRIORITY 10
TaskHandle_t modbus_task_handle;
void modbus_task(void *pvParameters); // Modbus 任务函数

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

    // 3. 创建 Modbus 任务
    BaseType_t modbus_result = xTaskCreate(
        (TaskFunction_t)modbus_task,
        (char *)MODBUS_TASK_NAME,
        (configSTACK_DEPTH_TYPE)MODBUS_TASK_STACK,
        (void *)NULL,
        (UBaseType_t)MODBUS_TASK_PRIORITY,
        (TaskHandle_t *)&modbus_task_handle);
    if (modbus_result == pdPASS)
    {
        DEBUG_PRINTLN("Modbus 任务创建成功...");
    }
    else
    {
        DEBUG_PRINTLN("Modbus 任务创建失败...");
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

// Modbus 任务的执行函数
uint8_t modbus_rx_buffer[256];
uint16_t modbus_rx_len;
void modbus_task(void *pvParameters)
{
    DEBUG_PRINTLN("Modbus 任务开始执行...");
    // 使用串口接收不定长数据
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, modbus_rx_buffer, 256);
    while (1)
    {
        // 等待任务通知
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // 解析 Modbus 数据
        Int_Modbus_ResponseParse(modbus_rx_buffer,modbus_rx_len);

        // 打印接收到的数据
        for (int i = 0; i < modbus_rx_len; i++)
        {
            printf("0x%02x\t", modbus_rx_buffer[i]);
        }
        printf("\n");
    }
}

// UART 接收事件回调函数 !每次调用前需要开启UART接收中断
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    debug_printfln("\n接收到数据....");

    modbus_rx_len = Size;

    // 发送任务通知
    vTaskNotifyGiveFromISR(modbus_task_handle, NULL);

    // 使用串口接收不定长数据
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, modbus_rx_buffer, 256);
}
