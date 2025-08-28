#include "App_W5500.h"

void App_W5500_Init(void)
{
    // 1. 初始化 W5500
    Int_W5500_Init();
}

void App_MQTT_Receive(void)
{
    // 接收订阅信息
    Int_MQTT_Receive();
}
