// static void cJSON_to_string(void)
// {
    
// }

#include "cJSON.h"

// 结构体定义：区分字符串和数值类型
typedef struct {
    uint8_t data1[7];  // 字符串："aabbcc"
    uint8_t data2[7];  // 字符串："ddeeff"
    uint32_t data3;    // 数值：112233
    uint32_t data4;    // 数值：445566
} Data_Info;

// 结构体初始化
Data_Info data_info = {
    "aabbcc",  // data1（字符串）
    "ddeeff",  // data2（字符串）
    112233,    // data3（整数）
    445566     // data4（整数）
};

/**
 * @brief 将结构体数据转换为JSON字符串
 * 
 * @param data_info 指向需要转换的结构体
 * @return char* 转换后的JSON字符串（需调用者用free释放）
 */
static char* data_to_cJSONstr(Data_Info *data_info)
{
    // 1. 创建cJSON对象并添加字段
    cJSON *cjson_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_obj, "data1", (char*)data_info->data1);
    cJSON_AddStringToObject(cjson_obj, "data2", (char*)data_info->data2);
    cJSON_AddNumberToObject(cjson_obj, "data3", data_info->data3);
    cJSON_AddNumberToObject(cjson_obj, "data4", data_info->data4);
    
    // 2. 直接在函数内将cJSON对象转换为cJSON字符串 (带缩进和换行)
    // char *json_str = cJSON_Print(cjson_obj);
    // 2 直接在函数内将cJSON对象转换为cJSON字符串 (紧凑型字符串)
    char *json_str = cJSON_PrintUnformatted(cjson_obj);
    
    // 3. 转换完成后立即释放cJSON对象（避免内存泄漏）
    cJSON_Delete(cjson_obj);
    
    // 4. 返回JSON字符串（调用者需负责释放）
    return json_str;
}

int main() {
    // 调用函数直接获取JSON字符串
    char *json_str = data_to_cJSONstr(&data_info);
    
    // 打印结果
    printf("转换结果：\n%s\n", json_str);
    
    // 释放字符串内存（由调用者负责）
    free(json_str);
    
    return 0;
}
