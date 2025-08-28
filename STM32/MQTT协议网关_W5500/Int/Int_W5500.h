#ifndef __INT_W5500_H__
#define __INT_W5500_H__

#include "wizchip_conf.h"
#include "socket.h"
#include "Com_Debug.h"
#include "Com_Util.h"
#include "Com_Config.h"
#include "mqtt_interface.h"
#include "MQTTClient.h"
#include "cJSON.h"
#include "Int_Modbus.h"


/**
 * @brief W5500以太网模块 初始化

 * 
 */
void Int_W5500_Init(void);

/**
 * @brief 轮询接收MQTT的订阅的数据
 */
void Int_MQTT_Receive(void);

/**
 * @brief 检查 MQTT 是否连接,断开则重新连接
 * 
 */
void Int_MQTT_IsConnect(void);

/**
 * @brief 发送 Modbus数据到服务器
 * 
 * @param topic 主题
 * @param data 要发送的数据
 * @param len 数据长度
 */
void Int_MQTT_SendToServer(uint8_t *topic,uint8_t *data,uint16_t len);

/**
 * @brief W5500以太网模块 发送数据到服务器
 * 
 * @param data 要发送的数据
 * @param len 数据长度
 * @return uint8_t 发送状态 (0:成功;1:失败)
 */
// uint8_t Int_W5500_SendData(uint8_t *data, uint16_t len);

#endif /* __INT_W5500_H__ */
