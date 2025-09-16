#ifndef __COM_UTIL_H__
#define __COM_UTIL_H__

#include "esp_heap_caps.h"
#include "string.h"
#include "stddef.h"
#include "Com_Debug.h"
#include "Dri_nvs.h"

#define UUID_KEY "UUID"

/**
 * 获取WIFI的 mac地址
 */
char *Com_Get_MAC(void);

/**
 * @brief 获取一个UUID 
 * 
 * @return char* 获取到的UUID
 */
char *Com_Get_UUID(void);

#endif /* __COM_UTIL_H__ */
