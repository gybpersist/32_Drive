### uart 配置

```c
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "esp_task.h"
// #include "Com_Define.h"
// #include "Com_Debug.h"

#define FPM383_TX_PIN GPIO_NUM_21
#define FPM383_RX_PIN GPIO_NUM_20

void App_main(void)
{
    // 串口配置
    const uart_config_t uart_config = {
        .baud_rate = 57600, // 可选其他波特率    // FPM383指纹模块默认波特率
        .data_bits = UART_DATA_8_BITS,         // 数据位长度
        .parity = UART_PARITY_DISABLE,         // 无校验
        .stop_bits = UART_STOP_BITS_1,         // 1停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // 禁用硬件流控制
        .source_clk = UART_SCLK_DEFAULT,       // UART 源时钟默认选择是 APB
    };
    uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, FPM383_TX_PIN, FPM383_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}
```

### uart 发送和接收数据

```c
// 定义接收缓冲区
uint8_t rx_buff[100] = {1};

/**
 * @brief 使用串口发送数据
 *
 * @param data 要发送的数据
 * @param len 发送数据的长度
 * @return Com_Status 发送状况
 */
static Com_Status Int_FPM383_SendData(uint8_t *data, uint32_t len)
{
    // 从给定的缓冲区和长度将数据发送到 UART 端口 ,返回发送数据的长度
    int tx_len = uart_write_bytes(UART_NUM_1, data, len);
    // 判断 发送数据的长度(tx_len) 和 写入的长度(len) 是否相同
    if (tx_len == len)
    {
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}

/**
 * @brief 通过串口接收数据
 *
 * @param data 接收到的数据
 * @param len 要接收数据的长度
 * @return Com_Status 接收状况
 */
static Com_Status Int_FPM383_ReceiveData(uint8_t *data, uint32_t len)
{
    // 接收前清空缓冲区
    memset(rx_buff, 1, 100);

    // UART 从 UART 缓冲区读取字节
    int rx_len = uart_read_bytes(UART_NUM_1, data, len, 1000);
    // 判断 接收到数据的长度(rx_len) 和 写入的长度(len) 是否相同
    if (rx_len == len)
    {
        return Com_OK;
    }
    else
    {
        return Com_ERROR;
    }
}
```

