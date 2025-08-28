# MQTT协议+W5500网关

## W5500初始化

### #include "Com_config.h"

```c
#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

#include <stdint.h>

// 定义结构体 保存本地 IP地址.MAC地址.子网掩码.网关地址
typedef struct
{
    uint8_t ip[4];  // IP地址
    uint8_t mac[6]; // MAC地址
    uint8_t snm[4]; // 子网掩码
    uint8_t gw[4];  // 网关地址
} Com_LocalConfig_t;

#endif /* __COM_CONFIG_H__ */
```

### #include "Int_W5500.c"初始化并添加订阅

```c
#include "Int_W5500.h"

// 定义宏 socket 编号 (可选 sn=1~7)
#define SOCKET_SN 0
// 定义宏 本地端口号
#define LOCAL_PORT 8457

// 定义宏 服务器端口号
#define SERVER_PORT 1883
// 定义 服务器 IP 地址
uint8_t server_ip[4] = {192,168,11,31};

// 定义 网络对象结构体指针
Network network;
// 定义 mqttclient 客户端
MQTTClient mqttclient;
// 定义 MQTT发送缓冲区
uint8_t mqtt_sendbuf[1024];
// 定义 MQTT接收缓冲区
uint8_t mqtt_readbuf[1024];

// 定义本地配置参数
Com_LocalConfig_t LocalConfig = {
    .ip = {192,168,11,100},                       //IP地址
    .mac = {122, 142, 162, 120, 144, 190},    //MAC地址
    .snm = {255,255,255,0},                      //子网掩码
    .gw = {192,168,11,1}                          //网关地址
};

void Int_W5500_Init(void)
{
    // 创建本地网络连接----------------------------------------------------------
    // 1 复位
    // 1.1 拉低复位引脚 W5500_RST
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
    Com_Delay_ms(10);       //至少需要保持 500 us
    // 1.3 拉高复位引脚 W5500_RST
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
    Com_Delay_ms(10);       //保持稳定

    // 2 注册函数
    development_reg_func();

    // 3 设置IP地址.MAC地址.子网掩码.网关地址
    setSIPR(LocalConfig.ip);    //设置IP地址
    setSHAR(LocalConfig.mac);   //设置MAC地址
    setSUBR(LocalConfig.snm);   //设置子网掩码
    setGAR(LocalConfig.gw);     //设置网关地址

    DEBUG_PRINTLN("W5500 初始化完成......");
    // 创建本地网络连接----------------------------------------------------------

    // 延时等待创建完成
    HAL_Delay(2000);

    // 创建TCP网络连接----------------------------------------------------------
    // 1 设置TCP网络连接
    // 1.1 创建一个网络对象
    NewNetwork(&network,SOCKET_SN);

    // 1.2 连接服务器
    int Connect_Result = ConnectNetwork(&network,server_ip,SERVER_PORT);
    if (Connect_Result != SOCK_OK)
    {
        DEBUG_PRINTLN("创建 TCP 连接服务器失败......");
        return ;
    }
    DEBUG_PRINTLN("创建 TCP 连接服务器成功......");
    // 创建TCP网络连接----------------------------------------------------------

    // 创建 MQTT 并连接---------------------------------------------------------
    // 1 MQTT连接
    // 1.1 创建 MQTT 客户端
    MQTTClientInit(&mqttclient,&network,5000,(unsigned char *)mqtt_sendbuf,sizeof(mqtt_sendbuf),(unsigned char *)mqtt_readbuf,sizeof(mqtt_readbuf));
    
    // 1.2 创建 MQTT 连接
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.cleansession = 1;               //清除会话
    options.clientID.cstring = "huihui";    //客户端ID
    options.keepAliveInterval = 60;         //保持连接时长
    options.MQTTVersion = 4;                //4: 3.1.1
    options.password.cstring = NULL;        //无密码
    options.username.cstring = NULL;        //无用户名
    options.willFlag = 0;                   //没有遗嘱
    int mqttresult = MQTTConnect(&mqttclient,&options);
    if(mqttresult != SUCCESSS)
    {
        DEBUG_PRINTLN("创建 MQTT 连接失败......");
        return ;
    }
    DEBUG_PRINTLN("创建 MQTT 连接成功......");
    // 创建 MQTT 并连接---------------------------------------------------------

    // 订阅
    MQTTSubscribe(&mqttclient,"aabbcc",QOS0,mqtt_aabbcc_callback);
}
```

### 订阅 aabbcc 的回调函数mqtt_aabbcc_callback()

```c
/**
 * 数据标示:
 * id: 1 设置电机的启停
 * id: 2 获取电机的启停
 * id: 3 设置电机的转速
 * id: 4 获取电机的实际转速
 * id: 5 获取电机的旋转方向
 */
// 定义 订阅 aabbcc 的回调函数
void mqtt_aabbcc_callback(MessageData* messagedata)
{
    DEBUG_PRINTLN("接收订阅 aabbcc 成功......");
    // debug_printfln("接收到数据:%s", messagedata->message->payload);

    // 1 将收到的 cjson字符串 解析为 cjson对象 
    cJSON *root = cJSON_ParseWithLength(messagedata->message->payload,messagedata->message->payloadlen);

    // 2 从解析到的 cjson对象 中解析每一个字段
    cJSON *id = cJSON_GetObjectItem(root,"id");
    cJSON *connect_type = cJSON_GetObjectItem(root,"connect_type");
    cJSON *modbus_id = cJSON_GetObjectItem(root,"modbus_id");
    

    // 3 从 cjson对象 中获取字符串
    int id_int = id->valueint;
    char *connect_type_str = connect_type->valuestring;
    int modbus_id_int = modbus_id->valueint;
    

    // 4 做类型的判断
    if (!cJSON_IsNumber(id) || !cJSON_IsString(connect_type) || !cJSON_IsNumber(modbus_id))
    {
        DEBUG_PRINTLN("id 或 connect_type 或 modbus_id 类型错误!!!");
        cJSON_Delete(root);
        return;
    }

    DEBUG_PRINTLN("id = %d,connect_type = %s,modbus_id = %d  ",id_int,connect_type_str,modbus_id_int);

    // 5 根据 id 做不同的处理
    if (id_int == 1)
    {
        // 设置电机的启停
        cJSON *on = cJSON_GetObjectItem(root,"on");
        int on_int = on->valueint;
        DEBUG_PRINTLN("设置的电机启停状态为:%d",on_int);

        Int_Modbus_WriteCoil(modbus_id_int,2,(on_int == 1) ? 1 : 0);
    }
    else if (id_int == 2)
    {
        // 获取电机的启停
        Int_Modbus_ReadCoil(modbus_id_int,2);
    }
    else if (id_int == 3)
    {
        // 设置电机的转速
        cJSON *speed = cJSON_GetObjectItem(root,"speed");
        int speed_int = speed->valueint;
        speed_int = speed_int + 500;
        Int_Modbus_WriteHOLD(modbus_id_int,2,speed_int);
    }
    else if (id_int == 4)
    {
        // 获取电机的实际转速
        Int_Modbus_ReadINPUT(modbus_id_int,2);
    }
    else if (id_int == 5)
    {
        // 获取电机的旋转方向
        Int_Modbus_ReadDISC(modbus_id_int,2);
    }
    else
    {
        DEBUG_PRINTLN("id 错误!!!");
    }

    // 5 释放 cjson对象
    cJSON_Delete(root);
}
```

### #include "Int_W5500.h"

```c
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

#endif /* __INT_W5500_H__ */
```

## 检查 MQTT 是否连接,断开则重新连接

### Int_MQTT_IsConnect()

```c
/**
 * @brief 检查 MQTT 是否连接,断开则重新连接
 * 
 */
void Int_MQTT_IsConnect(void)
{
    // 1 获取 socket 状态
    uint8_t connect_type = getSn_SR(SOCKET_SN);
    // 2 判断是否连接成功
    if (connect_type != SOCK_ESTABLISHED)
    {
        DEBUG_PRINTLN("MQTT 服务器断开连接...,正在重新连接......");
        Int_W5500_Init();
    }
```

## 轮询接收MQTT的订阅的数据

### Int_MQTT_Receive()

```c
/**
 * @brief 轮询接收MQTT的订阅的数据
 */
void Int_MQTT_Receive(void)
{
    // 检查 MQTT 是否连接,断开则重新连接
    Int_MQTT_IsConnect();

    // 2 轮询接收数据
    MQTTYield(&mqttclient, 50);
}
```

## 发送 Modbus数据到服务器

### Int_MQTT_SendToServer()

```c
/**
 * @brief 发送 Modbus数据到服务器
 * 
 * @param topic 主题
 * @param data 要发送的数据
 * @param len 数据长度
 */
void Int_MQTT_SendToServer(uint8_t *topic,uint8_t *data,uint16_t len)
{
    // 添加信息
    MQTTMessage message;
    message.qos = QOS0;
    message.payload = data;
    message.payloadlen = len;
    MQTTPublish(&mqttclient, (char *)topic, &message);
}
```

