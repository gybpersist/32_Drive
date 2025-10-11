#include "Dri_ntp.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "esp_log.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "NTP_STANDALONE";

// 时间同步完成回调函数
static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "NTP 时间同步成功");
    // 同步后立即打印一次时间
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &timeinfo);
    ESP_LOGI(TAG, "当前时间: %s", time_str);
}

// 初始化 NTP 客户端
static void ntp_init(void)
{
    // 设置 NTP 工作模式为轮询
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    // 配置主备 NTP 服务器（提高可靠性）
    sntp_setservername(0, (char *)NTP_SERVER);
    sntp_setservername(1, (char *)NTP_SERVER2);

    // 注册时间同步回调函数
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);

    // 启动 NTP 客户端
    sntp_init();
}

// 配置本地时区
static void configure_timezone(void)
{
    // 格式："UTC±[小时]:[分钟]:[秒]"（东八区为 UTC+8:00:00）
    char tz_str[32];
    snprintf(tz_str, sizeof(tz_str), "UTC-%d:00:00", TZ_OFFSET_SEC / 3600);
    setenv("TZ", tz_str, 1); // 设置时区环境变量
    tzset();                 // 应用时区设置
}

/**
 * @brief 初始化 NTP 时间同步
 *
 */
void Dri_ntp_Init(void)
{
    // 注意：此处假设 WiFi 已通过外部程序初始化并连接成功
    ESP_LOGI(TAG, "开始 NTP 时间同步（依赖外部 WiFi 配置）");

    // 配置时区
    configure_timezone();

    // 初始化 NTP
    ntp_init();

    // 等待首次时间同步（最多等待10秒）
    time_t now = 0;
    struct tm timeinfo = {0};
    int retry = 0;
    const int max_retry = 10;
    while (timeinfo.tm_year < (2020 - 1900) && retry < max_retry)
    {
        ESP_LOGI(TAG, "等待 NTP 同步... (%d/%d)", retry + 1, max_retry);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
        retry++;
    }

    if (timeinfo.tm_year < (2020 - 1900))
    {
        ESP_LOGE(TAG, "NTP 同步超时，请检查网络连接");
    }
}
