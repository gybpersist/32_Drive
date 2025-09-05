### 未启用中断 gpio_int_disable

```c
#include "driver/gpio.h"
#include "esp_task.h"

#define BDR6120_INA_PIN    GPIO_NUM_4
#define BDR6120_INB_PIN    GPIO_NUM_5
    
    
    // 1 GPIO 配置
    // 初始化GPIO配置结构
    gpio_config_t io_conf = {};
    // 禁用中断
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // 设置为输出模式
    io_conf.mode = GPIO_MODE_OUTPUT;
    // 要设置的引脚的位掩码
    io_conf.pin_bit_mask = (1ULL << BDR6120_INA_PIN) | (1ULL << BDR6120_INB_PIN);
    // 禁用下拉模式
    io_conf.pull_down_en = 0;
    // 禁用上拉模式
    io_conf.pull_up_en = 0;
    // 配置GPIO与给定的设置
    gpio_config(&io_conf);

    // 2 初始化INA INB为低电平
    gpio_set_level(BDR6120_INA_PIN, 0);
    gpio_set_level(BDR6120_INB_PIN, 0);
```

### 启用中断

```c
#define SC12B_INT_PIN  GPIO_NUM_0  


// 配置 INT 中断
    // 初始化配置结构
    gpio_config_t io_conf = {};
    // GPIO 中断类型 (上升沿)
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    // 引脚的位掩码
    io_conf.pin_bit_mask = (1 << SC12B_INT_PIN);
    // 设置为输入模式
    io_conf.mode = GPIO_MODE_INPUT;
    // 设置下拉模式
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);
    // 启动中断服务(参数0 表示使用默认配置)
    gpio_install_isr_service(0);
    // 为相应的 GPIO 引脚添加 ISR 中断处理程序
    gpio_isr_handler_add(SC12B_INT_PIN, gpio_isr_handler, (void *)NULL);


/**
 * @brief INT 的中断服务程序
 * 
 * @param arg 
 */
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    IsTouch = 1;
}
```

