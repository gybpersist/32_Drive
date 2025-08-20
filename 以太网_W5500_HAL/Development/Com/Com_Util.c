#include "Com_Util.h"

void Com_Delay_us(uint16_t us)
{
    volatile uint32_t count = 72 * us / 9;
    while(count--)
    {
        __NOP();
    }
}

void Com_Delay_ms(uint16_t ms)
{
    HAL_Delay(ms);
}

void Com_UTC2LocalTime(uint8_t *utc_time, uint8_t *local_time)
{
    struct tm utc_tm;       //定义 UTC 时间结构体 ,保存 UTC 时间
    struct tm *local_tm;     //定义本地时间结构体指针 ,保存本地时间

    // 将 UTC 时间字符串转换为 UTC 时间结构体
    sscanf((char *)utc_time, "%04d-%02d-%02d %02d:%02d:%02d", &utc_tm.tm_year, &utc_tm.tm_mon, &utc_tm.tm_mday, &utc_tm.tm_hour, &utc_tm.tm_min, &utc_tm.tm_sec);
    // 调整年份和月份
    utc_tm.tm_year -= 1900;
    utc_tm.tm_mon -= 1;
    // 将 UTC 时间结构体转换为时间戳
    time_t timestamp = mktime(&utc_tm);
    // 将时间戳加上 8 小时 ,变成本地时间的时间戳
    timestamp += 8 * 60 * 60;
    // 将时间戳转换为本地时间结构体
    local_tm = localtime(&timestamp);
    // 将本地时间结构体转换为字符串  YYYY-MM-DD HH:MM:SS 格式
    sprintf((char *)local_time, "%04d-%02d-%02d %02d:%02d:%02d", local_tm->tm_year + 1900, local_tm->tm_mon + 1, local_tm->tm_mday, local_tm->tm_hour, local_tm->tm_min, local_tm->tm_sec);
}
