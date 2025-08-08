// 自定义结构体类型 ,表示定位器要上传的数据
typedef struct {
    uint8_t uuid[25];  //使用STM32单片机全球芯片唯一代码,96位

    uint32_t steps;    //步数信息

    double lat;             //纬度信息
    uint8_t lat_dir[2];     //纬度方向   北纬 N   南纬 S
    double lon;             //经度信息
    uint8_t lon_dir[2];     //经度方向   东经 E   西经 W

    uint8_t datetime[21];   //日期时间信息 YYYY-MM-DD HH:MM:SS
} UploadData_t;

// 定义全局变量,用来保存要上传的数据
UploadData_t g_upload_data;

/**
 * @brief 把定位信息转换为 cJSON 字符串格式
 * 
 */
static void App_Uploader_LocationInfo2cJSON(void)
{
    cJSON *upload_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(upload_json,"lat",g_upload_data.lat);
    cJSON_AddStringToObject(upload_json,"lat_dir",(char *)g_upload_data.lat_dir);
    cJSON_AddNumberToObject(upload_json,"lon",g_upload_data.lon);
    cJSON_AddStringToObject(upload_json,"lon_dir",(char *)g_upload_data.lon_dir);
    cJSON_AddStringToObject(upload_json,"datetime",(char *)g_upload_data.datetime);

    // 2 把 cJSON 格式的数据 转换为字符串
    // char *upload_json_str = cJSON_Print(upload_json);
    // 2 把 cJSON 格式的数据 转换为紧凑型字符串
    char *upload_json_str = cJSON_PrintUnformatted(upload_json);

    // 3 将 cJSON 字符串保存到 全局变量中
    g_upload_json_str_len = strlen(upload_json_str);
    memcpy(g_upload_json_str,upload_json_str,g_upload_json_str_len);
    g_upload_json_str[g_upload_json_str_len] = '\0';    // 添加结束标记 '\0'

    // 4 释放 cJSON 字符串
    cJSON_Delete(upload_json);
    // 5 释放 cJSON 字符串内存
    free(upload_json_str);
}