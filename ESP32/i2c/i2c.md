### i2c 配置

```c
#include "driver/gpio.h"
#include "driver/i2c.h"

#define SC12B_SCL_PIN GPIO_NUM_1
#define SC12B_SDA_PIN GPIO_NUM_2

void App_main(void)
{
	// 1 I2C配置
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,             // 配置主模式
        .sda_io_num = SC12B_SDA_PIN,         // SDA引脚
        .scl_io_num = SC12B_SCL_PIN,         // SCL引脚
        .sda_pullup_en = GPIO_PULLUP_ENABLE, // 上拉
        .scl_pullup_en = GPIO_PULLUP_ENABLE, // 上拉
        .master.clk_speed = 400000,          // 速度 400KHZ
    };
    i2c_param_config(0, &conf);
    // 2 安装启动I2C
    i2c_driver_install(0, conf.mode, 0, 0, 0);
}
```

### i2c 读取和接收

```c
#define SC12B_DEV_ADDR 0x42

/**
 * @brief SC12B通过I2C读取数据
 *
 * @param reg_addr SC12B中的寄存器地址
 * @param data 读取的数据
 * @return esp_err_t 错误信息
 */
static esp_err_t Int_SC12B_read(uint8_t reg_addr, uint8_t *data)
{
    return i2c_master_write_read_device(0, SC12B_DEV_ADDR, &reg_addr, 1, data, 1, 1000);
}

/**
 * @brief SC12B通过I2C写入数据
 *
 * @param reg_addr SC12B中要写入的寄存器
 * @param data 写入的数据
 * @return esp_err_t 错误信息
 */
static esp_err_t Int_SC12B_write(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};
    ret = i2c_master_write_to_device(0, SC12B_DEV_ADDR, write_buf, sizeof(write_buf), 1000);

    return ret;
}
```

