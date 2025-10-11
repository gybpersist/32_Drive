#ifndef __DRI_NTP_H__
#define __DRI_NTP_H__

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "esp_log.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// NTP 服务器配置（国内可用）
#define NTP_SERVER "ntp.aliyun.com"       // 阿里云NTP服务器
#define NTP_SERVER2 "time1.cloud.tencent.com"  // 腾讯云备用服务器
// 时区偏移（东八区：UTC+8 = 28800秒）
#define TZ_OFFSET_SEC (8 * 3600)
#define DST_OFFSET_SEC 0  // 无夏令时

/**
 * @brief 初始化 NTP 时间同步
 * 
 */
void Dri_ntp_Init(void);

#endif /* __DRI_NTP_H__ */
