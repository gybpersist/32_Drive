### 4.5 LoRa 模块

#### ① 原理图

![5-lora-Schematic01.png](https://p.sda1.dev/25/92e0c2aef62f95c94edfd5aebd7d631d/5-lora-Schematic01.png)
![5-lora-Schematic02.png](https://p.sda1.dev/25/b9499b99c91b4f2cb7380ed5fb822597/5-lora-Schematic02.png)

#### ② CubeMX 配置

**SPI：**

![6-lora-hal1.png](https://p.sda1.dev/25/2b91b15f6ea9f5169d000287a2c367b9/6-lora-hal1.png)

**GPIO：**

![6-lora-hal2.png](https://p.sda1.dev/25/e1083c4a41db1bc4a1d6ef73687a9f60/6-lora-hal2.png)

#### ③ LoRa 通信概述

##### (1) 常见物联网通信

```
（1）远距离高速率的传输协议
典型协议包括蜂窝网络通信技术，如3G、4G、5G相关技术等，这是我们目前移动通信使用的典型技术。
（2）近距离高速率传输技术，
WiFi、传统蓝牙等，这些技术传输距离在几十到几百米级别，主要用在家庭环境和日常应用中，使用非常广泛。
（3）近距离低功耗传输技术
传统物联网中ZigBee、RFID、低功耗蓝牙。
```

```
上面三类技术大都要求较高的信噪比，对障碍的穿透性较小，无法在复杂环境中实现远距离低功耗传输。
低功耗广域网（LPWAN）技术填补这一技术空白。
```

![6-lora-%E6%A6%82%E8%BF%B01.png](https://p.sda1.dev/25/66a57bd1e9e015d262f0acb99b327b99/6-lora-概述1.png)

##### (2) 低功耗广域网 (LPWAN)

```
LPWAN（Low Power Wide Area Network）指的是低功耗广域网，其特点在于极低功耗，长距离以及海量连接，适用于物联网万物互联的场景。
LPWAN 不只是一种技术，而是代表了一族有着各种形式的低功耗广域网技术，其中LoRa使用的是一种扩频技术，NB-IoT使用的是窄带技术，这是两种有代表性的低功耗广域网技术。
```

![6-lora-%E6%A6%82%E8%BF%B02.png](https://p.sda1.dev/25/5d625371e0927c853037eaee8fdb75df/6-lora-概述2.png)

##### (3) LoRa 介绍

```
LoRa 是 Long Range Communication的简称，狭义上的LoRa指的是一种物理层的信号调制方式，是 Semtech (先科)公司定义的一种基于Chirp扩频技术的物理层调制方式，可达到-148 dBm的接收灵敏度，以偏小的数据速率（0.3-50kbps）换取更高的通讯距离（市内3km，郊区15km）和低功耗（电池供电在特定条件下可以工作长达10年）。

从系统角度看，LoRa也指由终端节点、网关、网络服务器、应用服务器所组成的一种网络系统架构：LoRa定义了不同设备在系统中的分工与作用，规定了数据在系统中流动与汇聚的方式。 

从应用角度看，LoRa为物联网应用提供了一种低成本、低功耗、远距离的数据传输服务：LoRa在使用10mW射频输出功率的情况下，可以提供超过15km传输距离，从而支持大量广域低功耗物联网应用。
```

##### (4) LoRa 特点

> **注意：**  标准 LoRa 通信技术采用**半双工（Half-Duplex）**模式 

```
1）远距离
在乡村地区能以长达 30 英里的距离为间隔连接设备，可穿透建筑物密集的市区或进深较长的室内环境。

2）低功耗
仅需极少能源，电池使用寿命长达 10 年，将电池更换成本降到最低。

3）安全
采用端到端 AES128 加密、双向认证等技术，实现完整性保护和保密性。

4）高性能
每个基站可处理逾百万条消息，完全满足公共网络运营商为大型市场提供服务的需求。

5）低成本
减少了基础设施的投资、电池更换支出和最终运营费用
```

##### (5) LoRa 应用

```
相比于Wi-Fi、蓝牙、ZigBee等传统无线局域网，LoRa可以实现更远距离的通信，有效扩展了网络的覆盖范围.

而相比于移动蜂窝网络，LoRa具有更低的硬件部署成本和更长的节点使用寿命，单个LoRa节点可以在电池供电的情况下连续工作数年。 

LoRa具有低数据率、远距离和低功耗的性质，因此非常适合与室外的传感器及其他物联网设备进行通信或数据交互。

LoRa在全球范围内进行了大量的应用部署，在智能仪表（如智能水表、智能电表）、智慧城市、智能交通数据采集、野生动物监控等众多物联网场景中都可以看到LoRa的应用。
```

##### (6) LoRa 组网架构

![6-lora-%E6%A6%82%E8%BF%B03.png](https://p.sda1.dev/25/91c2fc91bdfd2ea8d4ed27f229d31dee/6-lora-概述3.png)



#### ④ LLCC68 功能描述 

##### 功能框图

![6-lora-LLCC68-04.png](https://p.sda1.dev/25/95a5fb9240ec832f0dd6d1560ad29115/6-lora-LLCC68-04.png)

##### 引脚

![6-lora-LLCC68-01.png](https://p.sda1.dev/25/488269dce198349e60891a53c0cdc8ba/6-lora-LLCC68-01.png)
![6-lora-LLCC68-02.png](https://p.sda1.dev/25/2bec9333004334598c1c6b1bff67011e/6-lora-LLCC68-02.png)

##### SPI 接口

![6-lora-LLCC68-03.png](https://p.sda1.dev/25/40012ba832a59993ee43afe08c384427/6-lora-LLCC68-03.png)

#### ⑤ 移植 LLCC68 官方驱动

> LLCC68 官方驱动下载地址：https://github.com/libdriver/llcc68/  

(1) 把 下面 4个文件copy到项目的 `Interface/LoRa` 目录下, 并把文件 `driver_llcc68_interface_template.c` 改名为 `driver_llcc68_interface.c`

![6-lora-LLCC68-driver01.png](https://p.sda1.dev/25/1b1ef4613ee7d6c905e1b5fb78941c1d/6-lora-LLCC68-driver01.png)
![6-lora-LLCC68-driver02.png](https://p.sda1.dev/25/5328507bb9e4d73a65de37bd7220582a/6-lora-LLCC68-driver02.png)

(2) 把 `driver_llcc68_interface.c` 中的各个函数完成。

driver_llcc68_interface.h 中添加头文件引用

```c
#include <stdarg.h>
#include "spi.h"
```

`driver_llcc68_interface.c` 完善如下函数

```c
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

// 定义宏：片选信号拉低
#define LORA_CS_LOW HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_RESET);
// 定义宏：片选信号拉高
#define LORA_CS_HIGH HAL_GPIO_WritePin(LORA_CS_GPIO_Port, LORA_CS_Pin, GPIO_PIN_SET);

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
    // main.c 已经调用 MX_SPI_Init() 初始化了 SPI，因此这里不需要再次初始化
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

    // 片选信号拉低
    LORA_CS_LOW;

    // 如果 in_len 不为 0，则发送数据
    if (in_len > 0)
    {
        // 发送数据
        if (HAL_SPI_Transmit(&hspi1, in_buf, in_len, 1000) != HAL_OK)
        {
            // 片选信号拉高
            LORA_CS_HIGH;
            // 返回
            return 1;
        }
    }

    // 如果 out_len 不为 0，则接收数据
    if (out_len > 0)
    {
        // 接收数据
        if (HAL_SPI_Receive(&hspi1, out_buf, out_len, 1000) != HAL_OK)
        {
            // 片选信号拉高
            LORA_CS_HIGH;
            // 返回
            return 1;
        }
    }

    // 片选信号拉高
    LORA_CS_HIGH;

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
    // HAL 库已经初始化了 GPIO，因此这里不需要再次初始化
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

    // 修改 LORA_RST 引脚的状态
    HAL_GPIO_WritePin(LORA_RST_GPIO_Port, LORA_RST_Pin, data);

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
    // HAL 库已经初始化了 GPIO，因此这里不需要再次初始化
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

    // 读取 LORA_BUSY 引脚的状态
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
    va_list args;        // 定义一个 va_list 类型的变量来存储可变参数

    va_start(args, fmt); // 初始化 args，使其指向 fmt 之后的第一个参数

    // 使用 vprintf 来格式化输出（你也可以用 vsnprintf 等）
    vprintf(fmt, args);

    va_end(args); // 清理 args
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

```

(3) 给 `driver_llcc68.c` 添加一行代码，用来处理发送之后的状态。

![6-lora-LLCC68-driver03.png](https://p.sda1.dev/25/77c6ebc6f4bc6de8acfb2cbe92d50810/6-lora-LLCC68-driver03.png)

(4) 给 `driver_llcc68.h` 中结构体 `llcc68_handle_t` 添加一个成员 `receive_len` 表示接收到数据的长度。

![6-lora-LLCC68-driver04.png](https://p.sda1.dev/25/6c8537b24af0278e3c607e6ecb7b7bf2/6-lora-LLCC68-driver04.png)

(3) 给 `driver_llcc68.c` 中函数 `llcc68_irq_handler` 中添加一行代码用来给长度赋值

![6-lora-LLCC68-driver05.png](https://p.sda1.dev/25/40e0058a057949cce2904c019272d174/6-lora-LLCC68-driver05.png)

#### ⑤ 自定义代码

新建 `Int_LoRa.h` 文件和 `Int_LoRa.c` 文件

可以参考官方驱动中  `example/driver_llcc68_lora.h`和  `example/driver_llcc68_lora.c`

**Int_LoRa.h**

```c
#ifndef __INT_LLCC68_H__
#define __INT_LLCC68_H__

#include <stdint.h>
#include "driver_llcc68_interface.h"

/**
 * @brief llcc68 lora example default definition
 */
#define LLCC68_LORA_DEFAULT_STOP_TIMER_ON_PREAMBLE      LLCC68_BOOL_FALSE                 /**< disable stop timer on preamble */
#define LLCC68_LORA_DEFAULT_REGULATOR_MODE              LLCC68_REGULATOR_MODE_DC_DC_LDO   /**< only ldo */
#define LLCC68_LORA_DEFAULT_PA_CONFIG_DUTY_CYCLE        0x02                              /**< set +17dBm power */
#define LLCC68_LORA_DEFAULT_PA_CONFIG_HP_MAX            0x03                              /**< set +17dBm power */
#define LLCC68_LORA_DEFAULT_TX_DBM                      17                                /**< +17dBm */
#define LLCC68_LORA_DEFAULT_RAMP_TIME                   LLCC68_RAMP_TIME_10US             /**< set ramp time 10 us */
#define LLCC68_LORA_DEFAULT_SF                          LLCC68_LORA_SF_9                  /**< sf9 */
#define LLCC68_LORA_DEFAULT_BANDWIDTH                   LLCC68_LORA_BANDWIDTH_125_KHZ     /**< 125khz */
#define LLCC68_LORA_DEFAULT_CR                          LLCC68_LORA_CR_4_5                /**< cr4/5 */
#define LLCC68_LORA_DEFAULT_LOW_DATA_RATE_OPTIMIZE      LLCC68_BOOL_FALSE                 /**< disable low data rate optimize */
#define LLCC68_LORA_DEFAULT_RF_FREQUENCY                480000000U                        /**< 480000000Hz */
#define LLCC68_LORA_DEFAULT_SYMB_NUM_TIMEOUT            0                                 /**< 0 */

// 同步字，通信双方同步字必须一致
#define LLCC68_LORA_DEFAULT_SYNC_WORD                   0x3444U                           /**< public network */

#define LLCC68_LORA_DEFAULT_RX_GAIN                     0x94                              /**< common rx gain */
#define LLCC68_LORA_DEFAULT_OCP                         0x38                              /**< 140 mA */
#define LLCC68_LORA_DEFAULT_PREAMBLE_LENGTH             12                                /**< 12 */
#define LLCC68_LORA_DEFAULT_HEADER                      LLCC68_LORA_HEADER_EXPLICIT       /**< explicit header */
#define LLCC68_LORA_DEFAULT_BUFFER_SIZE                 255                               /**< 255 */
#define LLCC68_LORA_DEFAULT_CRC_TYPE                    LLCC68_LORA_CRC_TYPE_ON           /**< crc on */
#define LLCC68_LORA_DEFAULT_INVERT_IQ                   LLCC68_BOOL_FALSE                 /**< disable invert iq */
#define LLCC68_LORA_DEFAULT_CAD_SYMBOL_NUM              LLCC68_LORA_CAD_SYMBOL_NUM_2      /**< 2 symbol */
#define LLCC68_LORA_DEFAULT_CAD_DET_PEAK                24                                /**< 24 */
#define LLCC68_LORA_DEFAULT_CAD_DET_MIN                 10                                /**< 10 */
#define LLCC68_LORA_DEFAULT_START_MODE                  LLCC68_START_MODE_WARM            /**< warm mode */
#define LLCC68_LORA_DEFAULT_RTC_WAKE_UP                 LLCC68_BOOL_TRUE   


/**
 * @brief LLCC68 初始化
 * 
 */
void Int_LLCC68_Init(void);

/**
 * @brief 设置进入发送模式
 * 
 * @return uint8_t 0:success 1:fail
 */
uint8_t Int_LLCC68_SetSendMode(void);

/**
 * @brief 设置进入接收模式
 *
 * @return uint8_t 0:success 1:fail
 */
uint8_t Int_LLCC68_SetReceiveMode(void);

/**
 * @brief 发送数据
 * 
 * @param buf 要发送的数据
 * @param len 数据的长度
 * @return uint8_t 0:success 1:fail
 */
uint8_t Int_LLCC68_Send(uint8_t *buf, uint16_t len);

/**
 * @brief 接收数据
 *
 * @param rx_buf 接收的数据
 * @param rx_len 保存数据长度
 * @return uint8_t 0:success 1:fail
 */
uint8_t Int_LLCC68_Receive(uint8_t *rx_buf, uint16_t *rx_len);

#endif /* __INT_LLCC68_H__ */

```

**Int_LoRa.c**

```c
#include "Int_LLCC68.h"

static llcc68_handle_t gs_handle; /**< llcc68 handle */

/**
 * @brief LLCC68 初始化
 *
 */
void Int_LLCC68_Init(void)
{
    uint8_t res;
    uint32_t reg;
    uint8_t modulation;
    uint8_t config;

    /* link interface function */
    DRIVER_LLCC68_LINK_INIT(&gs_handle, llcc68_handle_t);
    DRIVER_LLCC68_LINK_SPI_INIT(&gs_handle, llcc68_interface_spi_init);
    DRIVER_LLCC68_LINK_SPI_DEINIT(&gs_handle, llcc68_interface_spi_deinit);
    DRIVER_LLCC68_LINK_SPI_WRITE_READ(&gs_handle, llcc68_interface_spi_write_read);
    DRIVER_LLCC68_LINK_RESET_GPIO_INIT(&gs_handle, llcc68_interface_reset_gpio_init);
    DRIVER_LLCC68_LINK_RESET_GPIO_DEINIT(&gs_handle, llcc68_interface_reset_gpio_deinit);
    DRIVER_LLCC68_LINK_RESET_GPIO_WRITE(&gs_handle, llcc68_interface_reset_gpio_write);
    DRIVER_LLCC68_LINK_BUSY_GPIO_INIT(&gs_handle, llcc68_interface_busy_gpio_init);
    DRIVER_LLCC68_LINK_BUSY_GPIO_DEINIT(&gs_handle, llcc68_interface_busy_gpio_deinit);
    DRIVER_LLCC68_LINK_BUSY_GPIO_READ(&gs_handle, llcc68_interface_busy_gpio_read);
    DRIVER_LLCC68_LINK_DELAY_MS(&gs_handle, llcc68_interface_delay_ms);
    DRIVER_LLCC68_LINK_DEBUG_PRINT(&gs_handle, llcc68_interface_debug_print);
    DRIVER_LLCC68_LINK_RECEIVE_CALLBACK(&gs_handle, llcc68_interface_receive_callback);

    /* init the llcc68 */
    res = llcc68_init(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: init failed.\n");

        return;
    }

    /* enter standby */
    res = llcc68_set_standby(&gs_handle, LLCC68_CLOCK_SOURCE_XTAL_32MHZ);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set standby failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set stop timer on preamble */
    res = llcc68_set_stop_timer_on_preamble(&gs_handle, LLCC68_LORA_DEFAULT_STOP_TIMER_ON_PREAMBLE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: stop timer on preamble failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set regulator mode */
    res = llcc68_set_regulator_mode(&gs_handle, LLCC68_LORA_DEFAULT_REGULATOR_MODE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set regulator mode failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set pa config */
    res = llcc68_set_pa_config(&gs_handle, LLCC68_LORA_DEFAULT_PA_CONFIG_DUTY_CYCLE, LLCC68_LORA_DEFAULT_PA_CONFIG_HP_MAX);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set pa config failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* enter to stdby xosc mode */
    res = llcc68_set_rx_tx_fallback_mode(&gs_handle, LLCC68_RX_TX_FALLBACK_MODE_STDBY_XOSC);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx tx fallback mode failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set dio irq */
    // res = llcc68_set_dio_irq_params(&gs_handle, 0x03FF, 0x03FF, 0x0000, 0x0000);
    // if (res != 0)
    // {
    //     llcc68_interface_debug_print("llcc68: set dio irq params failed.\n");
    //     (void)llcc68_deinit(&gs_handle);

    //     return;
    // }

    // /* clear irq status */
    // res = llcc68_clear_irq_status(&gs_handle, 0x03FF);
    // if (res != 0)
    // {
    //     llcc68_interface_debug_print("llcc68: clear irq status failed.\n");
    //     (void)llcc68_deinit(&gs_handle);

    //     return;
    // }

    /* set lora mode */
    res = llcc68_set_packet_type(&gs_handle, LLCC68_PACKET_TYPE_LORA);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set packet type failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set tx params */
    res = llcc68_set_tx_params(&gs_handle, LLCC68_LORA_DEFAULT_TX_DBM, LLCC68_LORA_DEFAULT_RAMP_TIME);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx params failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set lora modulation params */
    res = llcc68_set_lora_modulation_params(&gs_handle, LLCC68_LORA_DEFAULT_SF, LLCC68_LORA_DEFAULT_BANDWIDTH,
                                            LLCC68_LORA_DEFAULT_CR, LLCC68_LORA_DEFAULT_LOW_DATA_RATE_OPTIMIZE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora modulation params failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* convert the frequency */
    res = llcc68_frequency_convert_to_register(&gs_handle, LLCC68_LORA_DEFAULT_RF_FREQUENCY, (uint32_t *)&reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: convert to register failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set the frequency */
    res = llcc68_set_rf_frequency(&gs_handle, reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rf frequency failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set base address */
    res = llcc68_set_buffer_base_address(&gs_handle, 0x00, 0x00);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set buffer base address failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set lora symb num */
    res = llcc68_set_lora_symb_num_timeout(&gs_handle, LLCC68_LORA_DEFAULT_SYMB_NUM_TIMEOUT);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora symb num timeout failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* reset stats */
    res = llcc68_reset_stats(&gs_handle, 0x0000, 0x0000, 0x0000);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: reset stats failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* clear device errors */
    res = llcc68_clear_device_errors(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: clear device errors failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set the lora sync word */
    res = llcc68_set_lora_sync_word(&gs_handle, LLCC68_LORA_DEFAULT_SYNC_WORD);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora sync word failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* get tx modulation */
    res = llcc68_get_tx_modulation(&gs_handle, (uint8_t *)&modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }
    modulation |= 0x04;

    /* set the tx modulation */
    res = llcc68_set_tx_modulation(&gs_handle, modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set the rx gain */
    res = llcc68_set_rx_gain(&gs_handle, LLCC68_LORA_DEFAULT_RX_GAIN);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx gain failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* set the ocp */
    res = llcc68_set_ocp(&gs_handle, LLCC68_LORA_DEFAULT_OCP);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set ocp failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    /* get the tx clamp config */
    res = llcc68_get_tx_clamp_config(&gs_handle, (uint8_t *)&config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }
    config |= 0x1E;

    /* set the tx clamp config */
    res = llcc68_set_tx_clamp_config(&gs_handle, config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);

        return;
    }

    // 设置默认进入接收模式
    Int_LLCC68_SetReceiveMode();
}

/**
 * @brief 设置进入发送模式
 *
 * @return uint8_t 0:success 1:fail
 */
uint8_t Int_LLCC68_SetSendMode(void)
{
    // TXEN 引脚拉高
    HAL_GPIO_WritePin(LORA_TXEN_GPIO_Port, LORA_TXEN_Pin, GPIO_PIN_SET);
    // RXEN 引脚拉低
    HAL_GPIO_WritePin(LORA_RXEN_GPIO_Port, LORA_RXEN_Pin, GPIO_PIN_RESET);

    /* set dio irq */
    if (llcc68_set_dio_irq_params(&gs_handle, LLCC68_IRQ_TX_DONE | LLCC68_IRQ_TIMEOUT | LLCC68_IRQ_CAD_DONE | LLCC68_IRQ_CAD_DETECTED,
                                  LLCC68_IRQ_TX_DONE | LLCC68_IRQ_TIMEOUT | LLCC68_IRQ_CAD_DONE | LLCC68_IRQ_CAD_DETECTED,
                                  0x0000, 0x0000) != 0)
    {
        return 1;
    }

    /* clear irq status */
    if (llcc68_clear_irq_status(&gs_handle, 0x03FFU) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief 设置进入接收模式
 *
 * @return uint8_t 0:success 1:fail
 */
uint8_t Int_LLCC68_SetReceiveMode(void)
{
    // TXEN 引脚拉低
    HAL_GPIO_WritePin(LORA_TXEN_GPIO_Port, LORA_TXEN_Pin, GPIO_PIN_RESET);
    // RXEN 引脚拉高
    HAL_GPIO_WritePin(LORA_RXEN_GPIO_Port, LORA_RXEN_Pin, GPIO_PIN_SET);

    uint8_t setup;

    /* set dio irq */
    if (llcc68_set_dio_irq_params(&gs_handle, LLCC68_IRQ_RX_DONE | LLCC68_IRQ_TIMEOUT | LLCC68_IRQ_CRC_ERR | LLCC68_IRQ_CAD_DONE | LLCC68_IRQ_CAD_DETECTED,
                                  LLCC68_IRQ_RX_DONE | LLCC68_IRQ_TIMEOUT | LLCC68_IRQ_CRC_ERR | LLCC68_IRQ_CAD_DONE | LLCC68_IRQ_CAD_DETECTED,
                                  0x0000, 0x0000) != 0)
    {
        return 1;
    }

    /* clear irq status */
    if (llcc68_clear_irq_status(&gs_handle, 0x03FFU) != 0)
    {
        return 1;
    }

    /* set lora packet params */
    if (llcc68_set_lora_packet_params(&gs_handle, LLCC68_LORA_DEFAULT_PREAMBLE_LENGTH,
                                      LLCC68_LORA_DEFAULT_HEADER, LLCC68_LORA_DEFAULT_BUFFER_SIZE,
                                      LLCC68_LORA_DEFAULT_CRC_TYPE, LLCC68_LORA_DEFAULT_INVERT_IQ) != 0)
    {
        return 1;
    }

    /* get iq polarity */
    if (llcc68_get_iq_polarity(&gs_handle, (uint8_t *)&setup) != 0)
    {
        return 1;
    }

#if LLCC68_LORA_DEFAULT_INVERT_IQ == LLCC68_BOOL_FALSE
    setup |= 1 << 2;
#else
    setup &= ~(1 << 2);
#endif

    /* set the iq polarity */
    if (llcc68_set_iq_polarity(&gs_handle, setup) != 0)
    {
        return 1;
    }

    /* start receive */
    if (llcc68_continuous_receive(&gs_handle) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief 发送数据
 *
 * @param buf 要发送的数据
 * @param len 数据的长度
 * @return uint8_t 0:success 1:fail
 */
uint8_t Int_LLCC68_Send(uint8_t *buf, uint16_t len)
{
    // 进入发送模式
    Int_LLCC68_SetSendMode();

    /* send the data */
    if (llcc68_lora_transmit(&gs_handle, LLCC68_CLOCK_SOURCE_XTAL_32MHZ,
                             LLCC68_LORA_DEFAULT_PREAMBLE_LENGTH, LLCC68_LORA_DEFAULT_HEADER,
                             LLCC68_LORA_DEFAULT_CRC_TYPE, LLCC68_LORA_DEFAULT_INVERT_IQ,
                             (uint8_t *)buf, len, 0) != 0)
    {
        return 1;
    }

    // 再次恢复到接收模式
    Int_LLCC68_SetReceiveMode();

    return 0;
}

/**
 * @brief 接收数据
 *
 * @param rx_buf 接收的数据
 * @param rx_len 保存数据长度
 * @return uint8_t 0:success 1:fail
 */
uint8_t Int_LLCC68_Receive(uint8_t *rx_buf, uint16_t *rx_len)
{   
    // 调用函数接收数据
    if (llcc68_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }

    // 如果接收长度大于0，表示接收到数据
    if (gs_handle.receive_len > 0)
    {
        // 将接收到的数据复制到 rx_buf
        memcpy(rx_buf, gs_handle.receive_buf, gs_handle.receive_len);
        // 将接收长度保存到 rx_len
        *rx_len = gs_handle.receive_len;
        // 给 rx_buf 添加结束标记
        rx_buf[*rx_len] = '\0';

        // 清空接收缓冲区
        memset(gs_handle.receive_buf, 0, 256);
        gs_handle.receive_len = 0;

        return 0;
    }

    return 1;
}

```

**mian.c (演示)：**

```c
int main()
{
    // 初始化LLCC68
    Int_LLCC68_Init();
    
    while (1)
    {
        // 发送 Lora 数据
        Int_LLCC68_Send("Hello, LLCC68!", 14);
        // 延时2s
        Com_Delay_ms(2000);
    }
}
```



## 附录

#### 名词解释

```
NB-IoT: Narrow Band Internet of Things，窄带物联网
LoRa:	Long Range Radio，远距离无线电
LGA:	Land Grid Array，网格阵列封装

CASIC: China Aerospace Science and Industry Corporation, 中国航天科工集团; 也指代其开发的导航通信协议，用于高精度定位和数据传输
NMEA: National Marine Electronics Association, 美国国家海洋电子协会; 也代指其制定的标准通信协议，最初用于航海设备间的数据交换，现已成为GPS导航设备的通用数据格式

DHCP:	Dynamic Host Configuration Protocol，动态主机配置协议
DNS:	Domain Name System

PSM:	Power Saving Mode，节能模式
```

#### 相关单词

```
latitude	纬度
longitude	经度
Ethernet	以太网
Attention	注意；立正；关心
Attach		系上，贴上，连接；附上
```

#### 相关工具网址

```
TCP 服务测试工具: https://netlab.luatos.com
C语言标准库手册:  https://en.cppreference.com/w/c/header.html
经纬度定位：https://lbsyun.baidu.com/jsdemo/demo/yLngLatLocation.htm
```

