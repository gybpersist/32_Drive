#include "Int_M24C02.h"

/**
 * @brief 初始化
 */
void Int_M24C02_Init(void) {}

/**
 * @brief 写一个字节
 */
void Int_M24C02_WriteByte(uint8_t byte_addr, uint8_t data)
{
    while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
        vTaskDelay(1); // 等待 I2C 就绪
    }
    HAL_I2C_Mem_Write(&hi2c2, M24C02_ADDR_W, byte_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
    vTaskDelay(10);
}

/**
 * @brief 写多个字节
 */
void Int_M24C02_WriteBytes(uint8_t byte_addr, uint8_t *data, uint8_t len)
{
    while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
    {
        vTaskDelay(1); // 等待 I2C 就绪
    }
    HAL_I2C_Mem_Write(&hi2c2, M24C02_ADDR_W, byte_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
    vTaskDelay(10);
}

/**
 * @brief 读一个字节
 */
void Int_M24C02_ReadByte(int8_t byte_addr, uint8_t *data)
{
    HAL_I2C_Mem_Read(&hi2c2, M24C02_ADDR_R, byte_addr, I2C_MEMADD_SIZE_8BIT, data, 1, 1000);
}

/**
 * @brief 读多个字节
 */
void Int_M24C02_ReadBytes(uint8_t byte_addr, uint8_t *data, uint8_t len)
{
    HAL_I2C_Mem_Read(&hi2c2, M24C02_ADDR_R, byte_addr, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
}
