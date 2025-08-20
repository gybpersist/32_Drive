#ifndef __INT_M24C02_H__
#define __INT_M24C02_H__

#include "i2c.h"	//使用 i2c2
#include "Com_Debug.h"
#include "FreeRTOS.h"
#include "task.h"

#define M24C02_ADDR 0xA0
#define M24C02_ADDR_W 0xA0
#define M24C02_ADDR_R 0xA1

/**
 * @brief 初始化
 */
void Int_M24C02_Init(void);

/**
 * @brief 写一个字节
 */
void Int_M24C02_WriteByte(uint8_t byte_addr, uint8_t data);

/**
 * @brief 写多个字节
 */
void Int_M24C02_WriteBytes(uint8_t byte_addr, uint8_t *data, uint8_t len);

/**
 * @brief 读一个字节
 */
void Int_M24C02_ReadByte(int8_t byte_addr, uint8_t *data);

/**
 * @brief 读多个字节
 */
void Int_M24C02_ReadBytes(uint8_t byte_addr, uint8_t *data, uint8_t len);

#endif /* __INT_M24C02_H__ */
