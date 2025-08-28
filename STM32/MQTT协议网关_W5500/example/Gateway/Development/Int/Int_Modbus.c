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

    // 通过 cJSON 格式化数据
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "modbus_id", modbus_response.modbus_id);
    cJSON_AddStringToObject(json, "status", (char *)modbus_response.status);
    cJSON_AddStringToObject(json, "measure", (char *)modbus_response.measure);

    // 转换为字符串
    char *json_str = cJSON_Print(json);
    if (json_str != NULL)
    {
        // DEBUG_PRINTLN("JSON 字符串: %s", json_str);
        // 发送 JSON 字符串 到 MQTT 服务器
        Int_MQTT_SendToServer((uint8_t *)"112233",(uint8_t *)json_str, strlen(json_str));
        // 释放内存
        free(json_str);
        cJSON_Delete(json);
    }
}

