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

/* �����ߴ��� start */
// �����: ���� CS Ƭѡ����
#define LORA_CS_LOW HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET)
// �����: ���� CS Ƭѡ����
#define LORA_CS_HIGH HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET)
/* �����ߴ��� end */

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t llcc68_interface_spi_init(void)
{
    // main.c �Ѿ������� MX_SPI_Init() ����
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
    /* �����ߴ��� start */
    return HAL_SPI_DeInit(&hspi1) == HAL_OK ? 0 : 1;
    /* �����ߴ��� end */

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
    /* �����ߴ��� start */
    LORA_CS_LOW; // ���� CS Ƭѡ����

    // ��� (in_len ���� 0) �����з���
    if (in_len > 0)
    {
        // ��Ƭ���������� �� llcc68 ��������
        if (HAL_SPI_Transmit(&hspi1, in_buf, in_len, HAL_MAX_DELAY) != HAL_OK)
        {
            LORA_CS_HIGH; // ���� CS Ƭѡ����
            return 1;
        }
    }

    // ��� (out_len ���� 0) �����н���
    if (out_len > 0)
    {
        // ��Ƭ���������� �� llcc68 ��������
        if (HAL_SPI_Receive(&hspi1, out_buf, out_len, HAL_MAX_DELAY) != HAL_OK)
        {
            LORA_CS_HIGH; // ���� CS Ƭѡ����
            return 1;
        }
    }

    LORA_CS_HIGH; // ���� CS Ƭѡ����
    /* �����ߴ��� end */

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
    // HAL ���Ѿ���ʼ���� GPIO ����
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
    // ʡ��
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
    /* �����ߴ��� start */
    // �͵�ƽ��λ
    HAL_GPIO_WritePin(LORA_RST_GPIO_Port, LORA_RST_Pin, data == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
    /* �����ߴ��� end */

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
    // HAL ���Ѿ���ʼ���� busy GPIO ����
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
    // ʡ��
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
    /* �����ߴ��� start */
    // ��ȡ busy ����״̬
    *value = HAL_GPIO_ReadPin(LORA_BUSY_GPIO_Port, LORA_BUSY_Pin);
    /* �����ߴ��� end */

    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void llcc68_interface_delay_ms(uint32_t ms)
{
    /* �����ߴ��� start */
    HAL_Delay(ms);
    /* �����ߴ��� end */
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void llcc68_interface_debug_print(const char *const fmt, ...)
{
    /* �����ߴ��� start */
    va_list args;// ����һ�� va_list ���͵ı��� args�����ڴ洢�ɱ�����б�
    va_start(args, fmt);// ��ʼ�� args��ʹ��ָ��ɱ�����б�ĵ�һ��������fmt �����һ���̶�����
    vprintf(fmt, args);// ʹ�� vprintf ��������ɱ�����б��еĲ�����fmt �Ǹ�ʽ���ַ���
    va_end(args);// �����ɱ�����Ĵ����� args ָ����Ϊ NULL����ֹ�ڴ�й©
    /* �����ߴ��� end */
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
