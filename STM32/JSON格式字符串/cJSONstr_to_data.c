#include "cJSON.h"

// 结构体定义
typedef struct {
    uint8_t data1[7];  // 存储字符串"aabbcc"
    uint8_t data2[7];  // 存储字符串"ddeeff"
    uint32_t data3;    // 存储数值112233
    uint32_t data4;    // 存储数值445566
} Data_Info;

/**
 * @brief 从JSON字符串解析数据到结构体（简化版，无错误处理）
 * 
 * @param json_str 输入的JSON字符串
 * @param data 输出参数，用于存储解析后的数据
 */
static void cJSONstr_to_data(const char *json_str, Data_Info *data)
{
    // 1. 从JSON字符串解析出cJSON对象
    cJSON *cjson_obj = cJSON_Parse(json_str);
    
    // 2. 解析字符串字段（data1和data2）
    strncpy((char*)data->data1, cJSON_GetObjectItem(cjson_obj, "data1")->valuestring, 6);
    data->data1[6] = '\0';  // 添加字符串终止符
    
    strncpy((char*)data->data2, cJSON_GetObjectItem(cjson_obj, "data2")->valuestring, 6);
    data->data2[6] = '\0';
    
    // 3. 解析数值字段（data3和data4）
    data->data3 = (uint32_t)cJSON_GetObjectItem(cjson_obj, "data3")->valuedouble;
    data->data4 = (uint32_t)cJSON_GetObjectItem(cjson_obj, "data4")->valuedouble;
    
    // 4. 释放cJSON对象内存
    cJSON_Delete(cjson_obj);
}

// 调用示例
int main() {
    // 示例JSON字符串
    const char *json_str = "{\n    \"data1\": \"aabbcc\",\n    \"data2\": \"ddeeff\",\n    \"data3\": 112233,\n    \"data4\": 445566\n}";
    
    Data_Info data;
    // 从JSON字符串解析数据
    cJSON_str_to_data(json_str, &data);
    
    // 打印解析结果
    printf("解析结果：\n");
    printf("data1: %s\n", data.data1);
    printf("data2: %s\n", data.data2);
    printf("data3: %u\n", data.data3);
    printf("data4: %u\n", data.data4);
    
    return 0;
}

/*
{
    "data1": "aabbcc",
    "data2": "ddeeff",
    "data3": 112233,
    "data4": 445566
}

-->>

解析结果：
data1: aabbcc
data2: ddeeff
data3: 112233
data4: 445566
*/