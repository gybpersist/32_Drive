#include "Int_Key.h"

Key_Info key_info[KEY_COUNT] = {
    {KEY1_GPIO_Port, KEY1_Pin, KEY_STATE_FREE, KEY_ID_1, KEY_EVENT_KEY1, 0},
    {KEY2_GPIO_Port, KEY2_Pin, KEY_STATE_FREE, KEY_ID_2, KEY_EVENT_KEY2, 0},
    {KEY3_GPIO_Port, KEY3_Pin, KEY_STATE_FREE, KEY_ID_3, KEY_EVENT_KEY3, 0},
    {KEY4_GPIO_Port, KEY4_Pin, KEY_STATE_FREE, KEY_ID_4, KEY_EVENT_KEY4, 0},
};

/**
 * @brief 检测哪个key被按下
 * @param key_info 按键信息
 * @return Key_Event 按键事件
 * @note 按键按下时,返回按键事件(哪个key按下),按键未按下时,返回KEY_EVENT_NONE
 */
static Key_Event Int_Key_Process(Key_Info *key_info)
{
    // 1 每次进来获取当前时间
    //uint32_t current_time = xTaskGetTickCount();

    // 2 获得引脚的状态
    uint8_t pin_state = HAL_GPIO_ReadPin(key_info->port, key_info->pin);

    // 3 状态机
    switch (key_info->key_state)
    {
    case KEY_STATE_FREE:
        // 如果是空闲状态,可以切换消抖状态
        if (pin_state == GPIO_PIN_RESET)
        {
            // 切换消抖状态
            key_info->key_state = KEY_STATE_DEBOUNCE;
            // 记录按下时间
            //key_info->key_press_time = current_time;
        }
        break;
    case KEY_STATE_DEBOUNCE:
    /*     // 如果是消抖状态,可以切换按下状态 或者 切换空闲状态
        // 切换到空闲状态
        if (pin_state == GPIO_PIN_SET)
        {
            // 切换到空闲状态
            key_info->key_state = KEY_STATE_FREE;
        }
        // 切换到按下状态
        if ((current_time - key_info->key_press_time) > KEY_DEBOUNCE_TIME)
        {
            // 切换到按下状态
            key_info->key_state = KEY_STATE_PRESSED;
        } */
       
       // 如果轮询之后仍然为低电平,则判断为按键按下
        if (pin_state == GPIO_PIN_RESET)
        {
            // 切换到按下状态
            key_info->key_state = KEY_STATE_PRESSED;
        }else
        {
            // 切换到空闲状态
            key_info->key_state = KEY_STATE_FREE;
        }

        break;
    case KEY_STATE_PRESSED:
        // 切换到空闲状态 (按键松开)
        if (pin_state == GPIO_PIN_SET)
        {
            // 切换到空闲状态
            key_info->key_state = KEY_STATE_FREE;
            return key_info->key_event;
        }
    }

    // 4. 没有按键按下
    return KEY_EVENT_NONE;
}

Key_Event Int_Key_Scan(void)
{
    for (uint8_t i = 0; i < KEY_COUNT; i++)
    {
        // 1. 检测key事件
        Key_Event key_event = Int_Key_Process(&key_info[i]);
        if (key_event != KEY_EVENT_NONE)
        {
            // 返回按键事件
            return key_event;
        }
    }
    // 2. 没有按键按下
    return KEY_EVENT_NONE;
}
