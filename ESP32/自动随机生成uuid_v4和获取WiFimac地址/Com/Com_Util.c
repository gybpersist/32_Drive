#include "Com_Util.h"

char *mac_addr = NULL;
char *uuid = NULL;

/**
 * 获取WIFI的 mac地址
 */
char *Com_Get_MAC(void)
{
    // 如果已经有mac地址了直接返回
    if (mac_addr != NULL)
    {
        return mac_addr;
    }

    // 没有就获取
    uint8_t eth_mac[6] = {0};
    // WIFI_IF_STA：表示 Station 模式接口，即 ESP32 作为客户端连接到其他 Wi-Fi 热点
    esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
    mac_addr = heap_caps_malloc(18, MALLOC_CAP_SPIRAM);
    snprintf(mac_addr,18,
             "%02X:%02X:%02X:%02X:%02X:%02X",
             eth_mac[0], eth_mac[1], eth_mac[2], eth_mac[3], eth_mac[4], eth_mac[5]);

    return mac_addr;
}

/**
 * @brief 获取一个UUID
 *
 * @return char* 获取到的UUID
 */
char *Com_Get_UUID(void)
{
    if (uuid != NULL)
    {
        return uuid;
    }

    uuid = heap_caps_malloc(37, MALLOC_CAP_SPIRAM);

    // 从FLASH中查询UUID是否存在
    if (Dri_nvs_FindKey(UUID_KEY) == ESP_OK)
    {
        size_t len = 37;
        Dri_nvs_GetKey(UUID_KEY, uuid, &len);
        return uuid;
    }

    // 如果不存在,则随机生成一个UUID
    unsigned char bytes[16];
    for (int i = 0; i < 16; i++)
    {
        bytes[i] = rand() % 256;
    }

    // 设置版本号（版本4）
    bytes[6] = (bytes[6] & 0x0F) | 0x40; // 设置版本为4
    bytes[8] = (bytes[8] & 0x3F) | 0x80; // 设置变体为10

    // 格式化为UUID字符串
    sprintf(uuid,
            "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            bytes[0], bytes[1], bytes[2], bytes[3],
            bytes[4], bytes[5], bytes[6], bytes[7],
            bytes[8], bytes[9], bytes[10], bytes[11],
            bytes[12], bytes[13], bytes[14], bytes[15]);

    // 将生成的UUID保存到FLASH中
    Dri_nvs_AddKey(UUID_KEY, uuid);

    // 返回结果
    return uuid;
}
