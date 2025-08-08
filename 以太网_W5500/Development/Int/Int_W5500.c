#include "Int_W5500.h"

// 定义宏 socket 编号 (可选 sn=1~7)
#define SOCKET_SN 0
// 定义宏 本地端口号
#define LOCAL_PORT 5555

// 定义宏 服务器端口号
#define SERVER_PORT 8000

// 定义 服务器 IP 地址
uint8_t server_ip[4] = {192,168,11,36};

// 定义本地配置参数
Com_LocalConfig_t LocalConfig = {
    .ip = {192,168,11,100},                       //IP地址
    .mac = {0x7C,0x88,0xDC,0x12,0x2A,0x56},    //MAC地址
    .snm = {255,255,255,0},                      //子网掩码
    .gw = {192,168,11,1}                          //网关地址
};

void Int_W5500_Init(void)
{
    // 1 复位
    // 1.1 拉低复位引脚 W5500_RST
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
    Com_Delay_ms(10);       //至少需要保持 500 us
    // 1.3 拉高复位引脚 W5500_RST
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
    Com_Delay_ms(10);       //保持稳定

    // 2 注册函数
    development_reg_func();

    // 3 设置IP地址.MAC地址.子网掩码.网关地址
    setSIPR(LocalConfig.ip);    //设置IP地址
    setSHAR(LocalConfig.mac);   //设置MAC地址
    setSUBR(LocalConfig.snm);   //设置子网掩码
    setGAR(LocalConfig.gw);     //设置网关地址

    DEBUG_PRINTLN("W5500 初始化完成......");
}

uint8_t Int_W5500_SendData(uint8_t *data, uint16_t len)
{
    // 0 定义变量 保存 socket 的状态
    uint8_t status = 0;

    // 1 检查当前的连接状态 ,直到为 ESTABLISHED 状态
    do
    {
        // 1.1 获取 socket 状态 (关闭状态;处于YCP模式;断开请求)
        status = getSn_SR(SOCKET_SN);
        
        // 1.2 检查 socket 状态是否为关闭状态 ,创建 socket 客户端
        if (status == SOCK_CLOSED)
        {
            // 1.2.1 创建 socket 客户端
            uint8_t socket_status = socket(SOCKET_SN, Sn_MR_TCP, LOCAL_PORT, 0);
            if (socket_status == SOCKET_SN)
            {
                DEBUG_PRINTLN("socket 客户端创建成功......");
            }
            else
            {
                DEBUG_PRINTLN("socket 客户端创建失败......");
            }
        }

        // 1.3 检查 socket 状态是否为 INIT状态 ,如果是 ,则连接服务器
        if (status == SOCK_INIT)
        {
            // 1.3.1 连接服务器
            uint8_t connect_status = connect(SOCKET_SN, server_ip, SERVER_PORT);

            if (connect_status == SOCK_OK)
            {
                DEBUG_PRINTLN("socket 客户端连接服务器成功......");
            }
            else
            {
                DEBUG_PRINTLN("socket 客户端连接服务器失败......");

            }
        }

        // 1.4 检查 socket 状态是否为 CLOSE_WAIT 状态,如果是 ,则关闭连接服务器
        if (status == SOCK_CLOSE_WAIT)
        {
            // 1.4.1 关闭连接服务器
            uint8_t close_status = close(SOCKET_SN);
            if (close_status == SOCK_OK)
            {
                DEBUG_PRINTLN("socket 客户端关闭连接成功......");
            }
            else
            {
                DEBUG_PRINTLN("socket 客户端关闭连接失败......");
            }
        }
    } while (status != SOCK_ESTABLISHED);
    
    // 2 发送数据
    uint16_t send_status = send(SOCKET_SN, data, len);
    if (send_status == len)
    {
        DEBUG_PRINTLN("socket 客户端发送数据成功......");
        return 0;

    }
    else
    {
        DEBUG_PRINTLN("socket 客户端发送数据失败......");
        return 1;
    }
}
