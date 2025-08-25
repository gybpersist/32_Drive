#ifndef __APP_MODBUS_H__
#define __APP_MODBUS_H__

#include "Com_Debug.h"
#include "Com_Define.h"
#include "mb.h"
#include "port.h"

/**
 * @brief 初始化 Modbus 通信
 * 
 */
void App_Modbus_Init(void);

/**
 * @brief 处理 Modbus 通信
 * 
 */
void App_Modbus_Handle(void);

#endif /* __APP_MODBUS_H__ */
