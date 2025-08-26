# FreeModbus实现

## 发送方

**不需要移植FreeModbus源文件,但是需要打开并配置串口(需要打开串口的中断)**

### 添加 CRC校验文件 

#### #include "mbcrc.c" 

```c
#include "mbcrc.h"

static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40};

static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40};

uint16_t
usMBCRC16(uint8_t *pucFrame, uint16_t usLen)
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    int iIndex;

    while (usLen--)
    {
        iIndex = ucCRCLo ^ *(pucFrame++);
        ucCRCLo = (uint8_t)(ucCRCHi ^ aucCRCHi[iIndex]);
        ucCRCHi = aucCRCLo[iIndex];
    }
    return (uint16_t)(ucCRCHi << 8 | ucCRCLo);
}

```

#### #include "mbcrc.h" 

```c
#ifndef _MB_CRC_H
#define _MB_CRC_H

#include "gpio.h"

/**
 * @brief CRC校验
 */

uint16_t
usMBCRC16(uint8_t *pucFrame, uint16_t usLen);

#endif
```

### Modbus 发送数据

#### \#include "Int_Modbus.c"

```c
#include "Int_Modbus.h"

void Int_Modbus_Send(uint8_t *data, uint16_t len)
{
    // 打印发送的数据
    printf("\n");
    debug_printfln("发送数据....");
    for (uint16_t i = 0; i < len; i++)
    {
        printf("0x%02X ", data[i]);
    }

    HAL_UART_Transmit(&huart2, data, len, 1000);
    HAL_Delay(1000);
}

void Int_Modbus_WriteCoil(uint8_t id, uint16_t addr, uint8_t value)
{
    uint8_t cmd[8];

    // 写入线圈 功能码:0x05
    // 数据格式:id 功能码 地址高 地址低 数据高 数据低 校验低 校验高

    cmd[0] = id; // modbus_id

    cmd[1] = 0x05; // 功能码

    cmd[2] = (addr >> 8) & 0xFF; // 地址高
    cmd[3] = (addr >> 0) & 0xFF; // 地址低

    cmd[4] = ((value == 0) ? 0x00 : 0xFF); // 数据高
    cmd[5] = 0x00;                       // 数据低

    uint16_t crcvalue = usMBCRC16(cmd, 6);
    cmd[6] = (crcvalue >> 0) & 0xFF; // 校验低
    cmd[7] = (crcvalue >> 8) & 0xFF; // 校验高

    Int_Modbus_Send(cmd, 8); // 发送数据
}

void Int_Modbus_ReadCoil(uint8_t id, uint16_t addr)
{
    uint8_t cmd[8];

    // 读取线圈 功能码:0x01
    // 数据格式:id 功能码 地址高 地址低 数据高 数据低 校验低 校验高

    cmd[0] = id; // modbus_id

    cmd[1] = 0x01; // 功能码

    cmd[2] = (addr >> 8) & 0xFF; // 地址高
    cmd[3] = (addr >> 0) & 0xFF; // 地址低

    cmd[4] = 0x00; // 数据高
    cmd[5] = 0x01; // 数据低

    uint16_t crcvalue = usMBCRC16(cmd, 6);
    cmd[6] = (crcvalue >> 0) & 0xFF; // 校验低
    cmd[7] = (crcvalue >> 8) & 0xFF; // 校验高

    Int_Modbus_Send(cmd, 8); // 发送数据
}

void Int_Modbus_WriteHOLD(uint8_t id, uint16_t addr, uint16_t value)
{
    uint8_t cmd[8];

    // 写入保持寄存器 功能码:0x06
    // 数据格式:id 功能码 地址高 地址低 数据高 数据低 校验低 校验高

    cmd[0] = id; // modbus_id

    cmd[1] = 0x06; // 功能码

    cmd[2] = (addr >> 8) & 0xFF; // 地址高
    cmd[3] = (addr >> 0) & 0xFF; // 地址低

    cmd[4] = (value >> 8) & 0xFF; // 数据高
    cmd[5] = (value >> 0) & 0xFF; // 数据低

    uint16_t crcvalue = usMBCRC16(cmd, 6);
    cmd[6] = (crcvalue >> 0) & 0xFF; // 校验低
    cmd[7] = (crcvalue >> 8) & 0xFF; // 校验高

    Int_Modbus_Send(cmd, 8); // 发送数据
}

void Int_Modbus_ReadINPUT(uint8_t id,uint16_t addr)
{
    uint8_t cmd[8];

    // 读取输入寄存器 功能码:0x04
    // 数据格式:id 功能码 地址高 地址低 数据高 数据低 校验低 校验高

    cmd[0] = id; // modbus_id

    cmd[1] = 0x04; // 功能码

    cmd[2] = (addr >> 8) & 0xFF; // 地址高
    cmd[3] = (addr >> 0) & 0xFF; // 地址低

    cmd[4] = 0x00; // 数据高
    cmd[5] = 0x01; // 数据低

    uint16_t crcvalue = usMBCRC16(cmd, 6);
    cmd[6] = (crcvalue >> 0) & 0xFF; // 校验低
    cmd[7] = (crcvalue >> 8) & 0xFF; // 校验高

    Int_Modbus_Send(cmd, 8); // 发送数据
}

void Int_Modbus_ReadDISC(uint8_t id,uint16_t addr)
{
    uint8_t cmd[8];

    // 读取离散量 功能码:0x02
    // 数据格式:id 功能码 地址高 地址低 数据高 数据低 校验低 校验高

    cmd[0] = id; // modbus_id

    cmd[1] = 0x02; // 功能码

    cmd[2] = (addr >> 8) & 0xFF; // 地址高
    cmd[3] = (addr >> 0) & 0xFF; // 地址低

    cmd[4] = 0x00; // 数据高
    cmd[5] = 0x01; // 数据低

    uint16_t crcvalue = usMBCRC16(cmd, 6);
    cmd[6] = (crcvalue >> 0) & 0xFF; // 校验低
    cmd[7] = (crcvalue >> 8) & 0xFF; // 校验高

    Int_Modbus_Send(cmd, 8); // 发送数据
}
```

#### #include "Int_Modbus.h"

```c
#ifndef __INT_MODBUS_H__
#define __INT_MODBUS_H__

#include "usart.h"	// 串口
#include "mbcrc.h"	// CRC校验

/**
 * @brief Modbus 发送数据
 * 
 * @param data 要发送的数据
 * @param len 数据长度
 */
void Int_Modbus_Send(uint8_t *data, uint16_t len);

/**
 * @brief Modbus 写线圈 功能码:0x05
 * 
 * @param id modbus_id
 * @param addr 写入线圈的地址
 * @param value 要写入的值 [0:关闭 1:开启]
 */
void Int_Modbus_WriteCoil(uint8_t id,uint16_t addr,uint8_t value);

/**
 * @brief Modbus 读线圈 功能码:0x01
 * 
 * @param id modbus_id
 * @param addr 读取线圈的地址
 */
void Int_Modbus_ReadCoil(uint8_t id,uint16_t addr);

/**
 * @brief Modbus 写保持寄存器 功能码:0x06
 * 
 * @param id modbus_id
 * @param addr 写入保持寄存器的地址
 * @param value 要写入的值
 */
void Int_Modbus_WriteHOLD(uint8_t id,uint16_t addr,uint16_t value);

/**
 * @brief Modbus 读输入寄存器 功能码:0x04
 * 
 * @param id modbus_id
 * @param addr 读取输入寄存器的地址
 */
void Int_Modbus_ReadINPUT(uint8_t id,uint16_t addr);

/**
 * @brief Modbus 读离散量 功能码:0x02
 * 
 * @param id modbus_id
 * @param addr 读取离散量的地址
 */
void Int_Modbus_ReadDISC(uint8_t id,uint16_t addr);

#endif /* __INT_MODBUS_H__ */
```

### 发送数据后获得的响应信息

#### #include "App_Task.c"

```c
#include "App_Task.h"

// 1. 启动任务的配置
#define START_TASK_NAME "start_task"
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 14
TaskHandle_t start_task_handle;
void start_task(void *pvParameters); // 启动任务函数

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

// Modbus 任务的执行函数
uint8_t modbus_rx_buffer[256];
uint16_t modbus_rx_len;
void modbus_task(void *pvParameters)
{
    DEBUG_PRINTLN("Modbus 任务开始执行...");
    // 使用串口接收不定长数据(接收一次会关闭,需要再次打开)
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

    // 使用串口接收不定长数据(接收一次会关闭,需要再次打开)
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, modbus_rx_buffer, 256);
}
```

#### #include "App_Task.h"

```c
#ifndef __APP_TASK_H__
#define __APP_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "Com_Debug.h"
#include "Int_Modbus.h"

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

```

#### #include "Int_Modbus.c" 添加函数

```c

// Modbus响应结构体
typedef struct
{
    uint8_t modbus_id;
    uint8_t *status;
    uint8_t *measure;
}Modbus_Response_t;
void Int_Modbus_ResponseParse(uint8_t *data,uint16_t len)
{
    // 创建Modbus响应结构体
    Modbus_Response_t modbus_response;
    uint8_t message_str[100];   // 保存格式化后的字符串

    // 对接收到的数据进行 CRC校验
    uint16_t crcvalue = usMBCRC16(data, len - 2);
    uint16_t crcvalue_recv = (data[len - 1] << 8) | data[len - 2];
    if (crcvalue != crcvalue_recv)
    {
        DEBUG_PRINTLN("Modbus 响应数据校验失败!!!");
        return;
    }
    
	// 获取数据并作出处理 --------------------------------------------------------------
    // 获取 modbus_id
    uint8_t modbus_id = data[0];
    DEBUG_PRINTLN("modbus_id = %02X",modbus_id);
    modbus_response.modbus_id = modbus_id;      //响应结构体赋值

    // 判断功能码
    switch (data[1])
    {
    case 0x01:
        // 读取线圈 响应数据格式:id 功能码 数据字节数 数据 校验低 校验高
        if (data[3] == 0x00)
        {
            DEBUG_PRINTLN("电机是停止状态");
            modbus_response.measure = "motor is STOP status";
            modbus_response.status = "STOP";
        }
        else
        {
            DEBUG_PRINTLN("电机是运行状态");
            modbus_response.measure = "motor is RUN status";
            modbus_response.status = "RUN";
        }
        
        break;
    case 0x06:
        // 写入保持寄存器 响应数据格式:id 功能码 地址高 地址低 数据高 数据低 校验低 校验高
        DEBUG_PRINTLN("设置的电机转速为:%d",((data[4] << 8) | data[5]) - 500);
        sprintf((char *)message_str,"set motor speed is %d",((data[4] << 8) | data[5]) - 500);
        modbus_response.measure = message_str;
        modbus_response.status = "SET";

        break;
    case 0x04:
        // 读取输入寄存器 响应数据格式:id 功能码 数据字节数 数据 校验低 校验高
        DEBUG_PRINTLN("电机的实际转速为:%d",((data[3] << 8) | data[4]) - 500);
        sprintf((char *)message_str,"motor speed is %d",((data[3] << 8) | data[4]) - 500);
        modbus_response.measure = message_str;
        modbus_response.status = "READ";

        break;
    case 0x02:
        // 读取离散量 响应数据格式:id 功能码 数据字节数 数据 校验低 校验高
        if (data[3] == 0x00)
        {
            DEBUG_PRINTLN("电机是逆时针旋转");
            modbus_response.measure = "motor is anticlockwise rotation";
            modbus_response.status = "ANTI";

        }
        else
        {
            DEBUG_PRINTLN("电机是顺时针旋转");
            modbus_response.measure = "motor is clockwise rotation";
            modbus_response.status = "CLOCK";
        }

        break;
    case 0x05:
        // 写入线圈 响应数据格式:id 功能码 地址高 地址低 数据高 数据低 校验低 校验高
        if (data[4] == 0x00)
        {
            DEBUG_PRINTLN("关闭电机");
            modbus_response.measure = "close motor";
            modbus_response.status = "CLOSE";
        }
        else
        {
            DEBUG_PRINTLN("打开电机");
            modbus_response.measure = "open motor";
            modbus_response.status = "OPEN";
        }
        break;
    }

    // DEBUG_PRINTLN("modbus_id: %d ,status: %s ,measure: %s",modbus_response.modbus_id,modbus_response.status,modbus_response.measure);
    // 获取数据并作出处理 --------------------------------------------------------------
}
```

#### #include "Int_Modbus.c" 添加函数原型

```c
/**
 * @brief 接收到 Modbus信息 响应
 * 
 * @param data 响应数据
 * @param len 数据长度
 */
void Int_Modbus_ResponseParse(uint8_t *data,uint16_t len);
```

## 接收方

**需要移植FreeModbus源文件,详情:FreeModbus移植文件**

### 接收信息具象化(串口打印出来)

#### mbrtu.c 添加打印

```c
eMBErrorCode
eMBRTUReceive(UCHAR *pucRcvAddress, UCHAR **pucFrame, USHORT *pusLength)
{
    BOOL xFrameReceived = FALSE;
    eMBErrorCode eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION();
    assert(usRcvBufferPos < MB_SER_PDU_SIZE_MAX);

    // 开发者代码 start -------------------------
    // 最后需要关闭(影响接收结果)
    // debug_printfln("\n\n接收到了modbus协议数据.....");

    // for (uint16_t i = 0; i < usRcvBufferPos; i++)
    // {
    //     printf("%02X \t", ucRTUBuf[i]);
    // }
    // printf("\r\n");
    // 开发者代码 end -------------------------

    /* Length and CRC check */
    if ((usRcvBufferPos >= MB_SER_PDU_SIZE_MIN) && (usMBCRC16((UCHAR *)ucRTUBuf, usRcvBufferPos) == 0))
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = ucRTUBuf[MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = (USHORT)(usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC);

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = (UCHAR *)&ucRTUBuf[MB_SER_PDU_PDU_OFF];
        xFrameReceived = TRUE;
    }
    else
    {
        eStatus = MB_EIO;
    }

    EXIT_CRITICAL_SECTION();
    return eStatus;
}
```

### 添加 port.c 文件

```c
#include "mb.h"
#include "mbport.h"

// 发送和接收方通过这些缓冲区读取和写入信息

// 准备使用[2]标示当前转速
//  声明输入寄存器缓冲区，用于存储十路输入寄存器的值
uint16_t REG_INPUT_BUF[REG_INPUT_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 准备使用[2]标示目标转速
//  声明保持寄存器缓冲区，用于存储十路保持寄存器的值
uint16_t REG_HOLD_BUF[REG_HOLD_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 准备使用[2]标示电机的启动和停止
//  定义十路线圈的大小
uint8_t REG_COILS_BUF[REG_COILS_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 准备使用[2]标示电机的方向
//  声明离散量缓冲区，并初始化，用于存储十路离散量的状态
uint8_t REG_DISC_BUF[REG_DISC_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * @brief CMD4命令处理回调函数
 *
 * 该函数用于处理MODBUS协议中的CMD4命令，即读取输入寄存器。
 * 它将指定地址范围内的输入寄存器的值复制到缓冲区中。
 *
 * @param pucRegBuffer 指向用于存储寄存器值的缓冲区的指针。
 * @param usAddress 要读取的起始寄存器地址。
 * @param usNRegs 要读取的寄存器数量。
 *
 * @return 返回执行结果的错误代码。
 */
eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    // 计算寄存器索引，从0开始
    USHORT usRegIndex = usAddress - 1;

    // 非法检测：检查访问范围是否超出寄存器缓冲区大小
    if ((usRegIndex + usNRegs) > REG_INPUT_SIZE)
    {
        return MB_ENOREG;
    }

    // 循环读取寄存器值并写入缓冲区
    while (usNRegs > 0)
    {
        // 将寄存器的高8位写入缓冲区
        *pucRegBuffer++ = (unsigned char)(REG_INPUT_BUF[usRegIndex] >> 8);
        // 将寄存器的低8位写入缓冲区
        *pucRegBuffer++ = (unsigned char)(REG_INPUT_BUF[usRegIndex] & 0xFF);
        usRegIndex++;
        usNRegs--;
    }

    return MB_ENOERR;
}

/**
 * @brief CMD6、3、16命令处理回调函数
 *
 * 该函数用于处理Modbus协议中的 Holding Registers 读写请求。
 * 它根据请求的模式（读或写）对指定的寄存器进行相应的操作。
 *
 * @param pucRegBuffer 寄存器数据缓冲区，用于读取或写入寄存器数据。
 * @param usAddress 请求访问的起始寄存器地址。
 * @param usNRegs 请求访问的寄存器数量。
 * @param eMode 访问模式，可以是 MB_REG_WRITE（写寄存器）或 MB_REG_READ（读寄存器）。
 *
 * @return 返回执行结果，如果成功则返回 MB_ENOERR，否则返回相应的错误代码。
 */
eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    // 计算寄存器索引，Modbus 地址从 1 开始，数组索引从 0 开始，因此需要减 1。
    USHORT usRegIndex = usAddress - 1;

    // 非法检测：检查访问范围是否超出寄存器缓冲区大小。
    if ((usRegIndex + usNRegs) > REG_HOLD_SIZE)
    {
        return MB_ENOREG;
    }

    // 写寄存器
    if (eMode == MB_REG_WRITE)
    {
        // 循环将每个寄存器的数据从缓冲区写入到寄存器中。
        while (usNRegs > 0)
        {
            REG_HOLD_BUF[usRegIndex] = (pucRegBuffer[0] << 8) | pucRegBuffer[1];
            pucRegBuffer += 2;
            usRegIndex++;
            usNRegs--;
        }
    }
    // 读寄存器
    else
    {
        // 循环将每个寄存器的数据从寄存器中读取到缓冲区。
        while (usNRegs > 0)
        {
            *pucRegBuffer++ = (unsigned char)(REG_HOLD_BUF[usRegIndex] >> 8);
            *pucRegBuffer++ = (unsigned char)(REG_HOLD_BUF[usRegIndex] & 0xFF);
            usRegIndex++;
            usNRegs--;
        }
    }

    return MB_ENOERR;
}

/**
 * @brief CMD1、5、15命令处理回调函数
 *
 * 该函数用于处理Modbus协议中的CMD1、CMD5和CMD15命令。
 * 它主要负责读取或写入寄存器中的位数据。
 *
 * @param pucRegBuffer 指向寄存器缓冲区的指针，用于读取或写入数据。
 * @param usAddress 要操作的寄存器起始地址。
 * @param usNCoils 要操作的位数。
 * @param eMode 操作模式，可以是读或写。
 *
 * @return 返回操作结果，如果成功则返回MB_ENOERR，否则返回相应的错误代码。
 */
eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode)
{
    debug_printfln("线圈.....");
    if (eMode == MB_REG_WRITE)
    {
        // ......
        debug_printf("写入到线圈的命令:%d", REG_COILS_BUF[2]);
    }
    else
    {
        // ......
        debug_printfln("接收到读线圈的命令:%d", REG_COILS_BUF[2]);
    }
    // 计算寄存器索引
    USHORT usRegIndex = usAddress - 1;
    // 用于位操作的变量
    UCHAR ucBits = 0;
    // 用于存储位状态的变量
    UCHAR ucState = 0;
    // 用于循环操作的变量
    UCHAR ucLoops = 0;

    // 非法检测：检查访问范围是否超出寄存器缓冲区大小。
    if ((usRegIndex + usNCoils) > REG_COILS_SIZE)
    {
        return MB_ENOREG;
    }

    // 根据操作模式执行相应的操作
    if (eMode == MB_REG_WRITE)
    {
        // 计算需要循环的次数
        ucLoops = (usNCoils - 1) / 8 + 1;
        // 写操作
        while (ucLoops != 0)
        {
            // 获取当前寄存器的状态
            ucState = *pucRegBuffer++;
            // 位操作
            ucBits = 0;
            // 遍历每个位
            while (usNCoils != 0 && ucBits < 8)
            {
                // 将状态写入寄存器缓冲区
                REG_COILS_BUF[usRegIndex++] = (ucState >> ucBits) & 0X01;
                // 更新剩余位数
                usNCoils--;
                // 更新位索引
                ucBits++;
            }
            // 更新循环次数
            ucLoops--;
        }
    }
    else
    {
        // 计算需要循环的次数
        ucLoops = (usNCoils - 1) / 8 + 1;
        // 读操作
        while (ucLoops != 0)
        {
            // 初始化状态变量
            ucState = 0;
            // 位操作
            ucBits = 0;
            // 遍历每个位
            while (usNCoils != 0 && ucBits < 8)
            {
                // 根据寄存器缓冲区的状态更新状态变量
                if (REG_COILS_BUF[usRegIndex])
                {
                    ucState |= (1 << ucBits);
                }
                // 更新剩余位数
                usNCoils--;
                // 更新寄存器索引
                usRegIndex++;
                // 更新位索引
                ucBits++;
            }
            // 将状态写入寄存器缓冲区
            *pucRegBuffer++ = ucState;
            // 更新循环次数
            ucLoops--;
        }
    }

    return MB_ENOERR;
}

/**
 * @brief CMD2命令处理回调函数
 *
 * 该函数用于处理MODBUS协议中的CMD2命令，主要负责读取离散输入寄存器的值。
 *
 * @param pucRegBuffer 指向存放寄存器数据的缓冲区。
 * @param usAddress 寄存器的起始地址。
 * @param usNDiscrete 要读取的离散输入寄存器的数量。
 *
 * @return 返回执行结果，如果成功则返回MB_ENOERR，否则返回相应的错误代码。
 */
eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    // 计算寄存器索引，从0开始
    USHORT usRegIndex = usAddress - 1;
    // 用于处理位操作的变量
    UCHAR ucBits = 0;
    // 用于存储当前寄存器状态的变量
    UCHAR ucState = 0;
    // 用于控制循环次数的变量
    UCHAR ucLoops = 0;

    // 非法检测：检查访问范围是否超出寄存器缓冲区大小
    if ((usRegIndex + usNDiscrete) > REG_DISC_SIZE)
    {
        return MB_ENOREG;
    }

    // 计算需要循环的次数，每次循环处理最多8个离散输入
    ucLoops = (usNDiscrete - 1) / 8 + 1;
    // 循环读取离散输入寄存器的值
    while (ucLoops != 0)
    {
        ucState = 0;
        ucBits = 0;
        // 读取每个寄存器的值，并将其状态更新到ucState变量中
        while (usNDiscrete != 0 && ucBits < 8)
        {
            if (REG_DISC_BUF[usRegIndex])
            {
                ucState |= (1 << ucBits);
            }
            usNDiscrete--;
            usRegIndex++;
            ucBits++;
        }
        // 将读取到的状态值存入缓冲区中
        *pucRegBuffer++ = ucState;
        ucLoops--;
    }

    // 模拟离散量输入被改变，这里简单地将每个寄存器的状态取反
    for (usRegIndex = 0; usRegIndex < REG_DISC_SIZE; usRegIndex++)
    {
        REG_DISC_BUF[usRegIndex] = !REG_DISC_BUF[usRegIndex];
    }

    return MB_ENOERR;
}

/**
 * @brief 将 Modbus 库错误码映射为 Modbus 异常码
 *
 * @param error Modbus 库错误码（eMBErrorCode）
 * @return 对应的 Modbus 异常码
 */
uint8_t mapErrorToException(eMBErrorCode error)
{
    switch (error)
    {
    case MB_ENOREG:    // 非法寄存器地址
        return 0x02;   // 非法数据地址
    case MB_EINVAL:    // 非法参数
        return 0x03;   // 非法数据值
    case MB_ENORES:    // 资源不足
        return 0x04;   // 从站设备故障
    case MB_ETIMEDOUT: // 超时
        return 0x06;   // 从站设备忙
    case MB_EPORTERR:  // 端口错误
        return 0x04;   // 从站设备故障
    case MB_ENOERR:    // 无错误
        return 0x00;   // 无异常
    default:           // 未知错误
        return 0x04;   // 从站设备故障
    }
}
```

### port.h 添加 extern

```c
// 定义十路输入寄存器的大小
#define REG_INPUT_SIZE 10
// 声明输入寄存器缓冲区，用于存储十路输入寄存器的值
extern uint16_t REG_INPUT_BUF[REG_INPUT_SIZE];

// 定义十路保持寄存器的大小
#define REG_HOLD_SIZE 10
// 声明保持寄存器缓冲区，用于存储十路保持寄存器的值
extern uint16_t REG_HOLD_BUF[REG_HOLD_SIZE];

// 定义十路线圈的大小
#define REG_COILS_SIZE 10
// 声明线圈缓冲区，并初始化，用于存储十路线圈的状态
extern uint8_t REG_COILS_BUF[REG_COILS_SIZE];

// 定义十路离散量的大小
#define REG_DISC_SIZE 10
// 声明离散量缓冲区，并初始化，用于存储十路离散量的状态
extern uint8_t REG_DISC_BUF[REG_DISC_SIZE];
```

## 总结 

**发送方**可以用串口向接收方发送读取(读取缓冲区)/写入(写入缓冲区)的命令,并且,每当发送一条命令后接收方都会回复一个响应,发送方可以根据响应的内容判断发送是否成功.

**接收方**可以根据缓冲区的变化作出响应处理,还可以改变缓冲区的值改变发送方读取到的值.