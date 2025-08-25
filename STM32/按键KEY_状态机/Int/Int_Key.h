#ifndef __INT_KEY_H__
#define __INT_KEY_H__

#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"

#define KEY_DEBOUNCE_TIME 10 // 消抖时间 10ms

// 当前key的状态
typedef enum
{
    KEY_STATE_FREE = 0,  // 按键空闲
    KEY_STATE_DEBOUNCE,  // 消抖
    KEY_STATE_PRESSED,   // 按键按下
} Key_State;

// 当前key的ID
typedef enum
{
    KEY_ID_1 = 0, // 按键1
    KEY_ID_2,     // 按键2
    KEY_ID_3,     // 按键3
    KEY_ID_4,     // 按键4
    KEY_COUNT, // 按键数量
} Key_ID;

// 当前key按下的事件 (哪个key按下)
typedef enum
{
    KEY_EVENT_NONE = -1, // 无事件
    KEY_EVENT_KEY1,      // 按键1按下
    KEY_EVENT_KEY2,      // 按键2按下
    KEY_EVENT_KEY3,      // 按键3按下
    KEY_EVENT_KEY4,      // 按键4按下
} Key_Event;

// 当前key的信息
typedef struct 
{
    GPIO_TypeDef *port;     // 当前key的port
    uint16_t pin;           // 当前key的pin
    Key_State key_state;    // 当前key的状态
    Key_ID key_id;          // 当前key的ID
    Key_Event key_event;    // 当前key按下的事件 (哪个key按下)
    uint32_t key_press_time;// 当前key按下的时间
} Key_Info;

/**
 * @brief 按键扫描
 * 
 * @return Key_Event 按键事件
 * @note 按键按下时,返回按键事件(哪个key按下),按键未按下时,返回KEY_EVENT_NONE
 */
Key_Event Int_Key_Scan(void);

#endif /* __INT_KEY_H__ */
