#ifndef __APP_W5500_H__
#define __APP_W5500_H__

#include "Int_W5500.h"

/**
 * @brief 初始化 W5500
 * 
 */
void App_W5500_Init(void);

/**
 * @brief 接收 MQTT 消息
 * 
 */
void App_MQTT_Receive(void);

#endif /* __APP_W5500_H__ */
