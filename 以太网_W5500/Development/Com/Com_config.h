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
