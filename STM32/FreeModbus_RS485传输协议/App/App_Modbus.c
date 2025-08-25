#include "App_Modbus.h"

void App_Modbus_Init(void)
{
    // 初始化Modbus,此处的波特率不生效=> 直接使用的HAL库  最后一个参数是奇偶校验位
    eMBInit(MB_RTU, g_motor_id, 0, 115200, MB_PAR_NONE);
    // 启用Modbus协议栈
    eMBEnable();
}


void App_Modbus_Handle(void)
{
    // 轮训调用接受数据的方法
    eMBPoll();
}