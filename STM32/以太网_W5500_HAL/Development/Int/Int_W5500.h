#ifndef __INT_W5500_H__
#define __INT_W5500_H__

#include "wizchip_conf.h"
#include "socket.h"
#include "Com_Debug.h"
#include "Com_Util.h"
#include "Com_Config.h"

/**
 * @brief W5500以太网模块 初始化

 * 
 */
void Int_W5500_Init(void);

/**
 * @brief W5500以太网模块 发送数据到服务器
 * 
 * @param data 要发送的数据
 * @param len 数据长度
 * @return uint8_t 发送状态 (0:成功;1:失败)

 */
uint8_t Int_W5500_SendData(uint8_t *data, uint16_t len);

#endif /* __INT_W5500_H__ */
