#ifndef __INT_MODBUS_H__
#define __INT_MODBUS_H__

#include "usart.h"
#include "Com_Debug.h"
#include "mbcrc.h"
#include "cJSON.h"
#include <stdlib.h>  // 这个头文件包含了free函数的声明
#include "Int_W5500.h" 

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

/**
 * @brief 接收到 Modbus信息 响应
 * 
 * @param data 响应数据
 * @param len 数据长度
 */
void Int_Modbus_ResponseParse(uint8_t *data,uint16_t len);

#endif /* __INT_MODBUS_H__ */
