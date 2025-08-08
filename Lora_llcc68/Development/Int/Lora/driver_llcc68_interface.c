/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_llcc68_interface_template.c
 * @brief     driver llcc68 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-04-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/04/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_llcc68_interface.h"

/* 开发者代码 start */
// 定义宏: 拉低 CS 片选引脚
#define LORA_CS_LOW HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET)
// 定义宏: 拉高 CS 片选引脚
#define LORA_CS_HIGH HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET)
/* 开发者代码 end */

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t llcc68_interface_spi_init(void)
{
    // main.c 已经调用了 MX_SPI_Init() 函数
    return 0;
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t llcc68_interface_spi_deinit(void)
{
    /* 开发者代码 start */
    return HAL_SPI_DeInit(&hspi1) == HAL_OK ? 0 : 1;
    /* 开发者代码 end */

    // return 0;
}

/**
 * @brief      interface spi bus write read
 * @param[in]  *in_buf points to a input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to a output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t llcc68_interface_spi_write_read(uint8_t *in_buf, uint32_t in_len,
                                        uint8_t *out_buf, uint32_t out_len)
{
    /* 开发者代码 start */
    LORA_CS_LOW; // 拉低 CS 片选引脚

    // 如果 (in_len 大于 0) 代表有发送
    if (in_len > 0)
    {
        // 单片机发送数据 即 llcc68 接收数据
        if (HAL_SPI_Transmit(&hspi1, in_buf, in_len, HAL_MAX_DELAY) != HAL_OK)
        {
            LORA_CS_HIGH; // 拉高 CS 片选引脚
            return 1;
        }
    }

    // 如果 (out_len 大于 0) 代表有接收
    if (out_len > 0)
    {
        // 单片机接收数据 即 llcc68 发送数据
        if (HAL_SPI_Receive(&hspi1, out_buf, out_len, HAL_MAX_DELAY) != HAL_OK)
        {
            LORA_CS_HIGH; // 拉高 CS 片选引脚
            return 1;
        }
    }

    LORA_CS_HIGH; // 拉高 CS 片选引脚
    /* 开发者代码 end */

    return 0;
}

/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t llcc68_interface_reset_gpio_init(void)
{
    // HAL 库已经初始化了 GPIO 引脚
    return 0;
}

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t llcc68_interface_reset_gpio_deinit(void)
{
    // 省略
    return 0;
}

/**
 * @brief     interface reset gpio write
 * @param[in] data is the written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t llcc68_interface_reset_gpio_write(uint8_t data)
{
    /* 开发者代码 start */
    // 低电平复位
    HAL_GPIO_WritePin(LORA_RST_GPIO_Port, LORA_RST_Pin, data == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    /* 开发者代码 end */

    return 0;
}

/**
 * @brief  interface busy gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t llcc68_interface_busy_gpio_init(void)
{
    // HAL 库已经初始化了 busy GPIO 引脚
    return 0;
}

/**
 * @brief  interface busy gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t llcc68_interface_busy_gpio_deinit(void)
{
    // 省略
    return 0;
}

/**
 * @brief      interface busy gpio read
 * @param[out] *value points to a value buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t llcc68_interface_busy_gpio_read(uint8_t *value)
{
    /* 开发者代码 start */
    // 读取 busy 引脚状态
    *value = HAL_GPIO_ReadPin(LORA_BUSY_GPIO_Port, LORA_BUSY_Pin);
    /* 开发者代码 end */

    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void llcc68_interface_delay_ms(uint32_t ms)
{
    /* 开发者代码 start */
    HAL_Delay(ms);
    /* 开发者代码 end */
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void llcc68_interface_debug_print(const char *const fmt, ...)
{
    /* 开发者代码 start */
    va_list args;// 声明一个 va_list 类型的变量 args，用于存储可变参数列表
    va_start(args, fmt);// 初始化 args，使其指向可变参数列表的第一个参数，fmt 是最后一个固定参数
    vprintf(fmt, args);// 使用 vprintf 函数输出可变参数列表中的参数，fmt 是格式化字符串
    va_end(args);// 结束可变参数的处理，将 args 指针置为 NULL，防止内存泄漏
    /* 开发者代码 end */
}

/**
 * @brief     interface receive callback
 * @param[in] type is the receive callback type
 * @param[in] *buf points to a buffer address
 * @param[in] len is the buffer length
 * @note      none
 */
void llcc68_interface_receive_callback(uint16_t type, uint8_t *buf, uint16_t len)
{
    switch (type)
    {
    case LLCC68_IRQ_TX_DONE:
    {
        llcc68_interface_debug_print("llcc68: irq tx done.\n");

        break;
    }
    case LLCC68_IRQ_RX_DONE:
    {
        llcc68_interface_debug_print("llcc68: irq rx done.\n");

        break;
    }
    case LLCC68_IRQ_PREAMBLE_DETECTED:
    {
        llcc68_interface_debug_print("llcc68: irq preamble detected.\n");

        break;
    }
    case LLCC68_IRQ_SYNC_WORD_VALID:
    {
        llcc68_interface_debug_print("llcc68: irq valid sync word detected.\n");

        break;
    }
    case LLCC68_IRQ_HEADER_VALID:
    {
        llcc68_interface_debug_print("llcc68: irq valid header.\n");

        break;
    }
    case LLCC68_IRQ_HEADER_ERR:
    {
        llcc68_interface_debug_print("llcc68: irq header error.\n");

        break;
    }
    case LLCC68_IRQ_CRC_ERR:
    {
        llcc68_interface_debug_print("llcc68: irq crc error.\n");

        break;
    }
    case LLCC68_IRQ_CAD_DONE:
    {
        llcc68_interface_debug_print("llcc68: irq cad done.\n");

        break;
    }
    case LLCC68_IRQ_CAD_DETECTED:
    {
        llcc68_interface_debug_print("llcc68: irq cad detected.\n");

        break;
    }
    case LLCC68_IRQ_TIMEOUT:
    {
        llcc68_interface_debug_print("llcc68: irq timeout.\n");

        break;
    }
    default:
    {
        llcc68_interface_debug_print("llcc68: unknown code.\n");

        break;
    }
    }
}
