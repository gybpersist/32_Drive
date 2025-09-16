#include "Com_Util.h"

// 定义 可变缓冲区
struct mutable_buffer
{
    char *data;
    size_t data_len;
};

/**
 * @brief 创建 可变缓冲区对象
 *
 * @return mutable_buffer_t* 可变缓冲区对象
 */
mutable_buffer_t *Com_MutableBuffer_Creat(void)
{
    // 创建 可变缓冲区对象
    mutable_buffer_t *mutable_buffer = (mutable_buffer_t *)heap_caps_malloc(sizeof(mutable_buffer_t), MALLOC_CAP_SPIRAM);
    if (mutable_buffer != NULL)
    {
        mutable_buffer->data = NULL;
        mutable_buffer->data_len = 0;
    }

    // 返回 可变缓冲区对象
    return mutable_buffer;
}

void Com_MutableBuffer_AddData(mutable_buffer_t *mutable_buffer, char *data, size_t data_len)
{
    // 1. 输入参数有效性检查（避免非法输入）
    assert(mutable_buffer);
    // 若 data 为 NULL，data_len 必须为 0（否则无数据可拷贝）
    if (data == NULL && data_len > 0)
    {
        MY_LOGE("Invalid input: data is NULL but data_len > 0");
        return;
    }

    char *new_data = NULL;
    size_t new_len = mutable_buffer->data_len + data_len; // 新的有效数据总长度

    // 2. 分配/重分配内存
    if (mutable_buffer->data == NULL)
    {
        // 首次使用：分配 "新数据长度 + 1" 的内存
        new_data = heap_caps_malloc(new_len + 1, MALLOC_CAP_SPIRAM);
    }
    else
    {
        // 非首次：扩展内存（保留原有数据，新大小 = 原有数据 + 新数据 + 1）
        new_data = heap_caps_realloc(
            mutable_buffer->data, // 原有内存指针（realloc 会自动处理原有数据拷贝）
            new_len + 1,          // 修正：新内存大小 = 总有效数据 + \0
            MALLOC_CAP_SPIRAM);
    }

    // 3. 关键：检查内存分配是否成功（避免空指针）
    if (new_data == NULL)
    {
        // 分配失败：保留原有缓冲区数据，不覆盖（避免数据丢失）
        MY_LOGE("Failed to allocate SPI RAM: new_mem_size = %zu", new_len + 1);
        return;
    }

    // 4. 拷贝新数据到缓冲区末尾（仅当有数据可拷贝时）
    if (data_len > 0)
    {
        memcpy(
            new_data + mutable_buffer->data_len, // 目标地址：原有数据的末尾
            data,                                // 源地址：传入的新数据
            data_len                             // 拷贝长度：本次新数据的长度
        );
    }

    // 5. 更新缓冲区状态（确保数据完整和安全）
    mutable_buffer->data = new_data;      // 指向新分配的内存
    mutable_buffer->data_len = new_len;   // 更新有效数据长度
    mutable_buffer->data[new_len] = '\0'; // 添加字符串结束符（避免越界访问）
}

void *Com_MutableBuffer_GetData(mutable_buffer_t *mutable_buffer)
{
    assert(mutable_buffer);
    return mutable_buffer->data;
}

void Com_MutableBuffer_Free(mutable_buffer_t *mutable_buffer)
{
    // 检查缓冲区指针有效性
    assert(mutable_buffer);

    // 释放数据缓冲区（使用 heap_caps_free 匹配分配函数）
    if (mutable_buffer->data != NULL)
    {
        heap_caps_free(mutable_buffer->data); // 正确释放 SPI RAM 内存
        mutable_buffer->data = NULL;          // 置空指针，避免野指针
        mutable_buffer->data_len = 0;         // 重置长度
    }

    // 不释放 mutable_buffer 本身（由调用者管理其生命周期）
}
