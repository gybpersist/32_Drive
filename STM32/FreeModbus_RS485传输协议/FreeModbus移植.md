# FreeModbus移植

## 1 使用 UART2 串口

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F08%2F22%2F21-09-41-13cb88f6ac811fcd47d8ba1433bf34e9-image%20-1--5c4aed.png)

## 2 配置 UART2 串口

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F08%2F22%2F20-52-19-fbf3ecf36968adfa1421cc2ab0f33ea3-image%20-1--ed04fc.png)

## 3 UART2中断配置

打开 UART2串口 中断,并且要**关闭**HAL库中断代码生成。

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F08%2F22%2F21-12-18-42ba571e71d2605bed01bcdabff8c4bd-20250822211217668-94d8af.png)

## 4 **定时器中断配置**

根据**Modbus**的时序可知，需要使用一个定时器判断传输数据是否超过3.5字符的时间，这里使用**TIM3**。**官方推荐定时器单次计数时间为50us**，所以**PSC**为**3599**，串口波特率大于等于**19200**时，可以直接使用固定**35**次来代替3.5字符的时间。

打开 定时器TIM3 中断,并且要**关闭**HAL库中断代码生成。

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F08%2F22%2F21-16-50-7d95b08025663ee293886a7b6dc5e484-20250822211650337-848f6b.png)

## 5 port.c移植

创建文件port.c用于实现从机接收命令的回调函数。

```c
#include "mb.h"
#include "mbport.h"

// 准备使用[2]标示当前转速
//  声明输入寄存器缓冲区，用于存储十路输入寄存器的值
uint16_t REG_INPUT_BUF[REG_INPUT_SIZE]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 准备使用[2]标示目标转速
//  声明保持寄存器缓冲区，用于存储十路保持寄存器的值
uint16_t REG_HOLD_BUF[REG_HOLD_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 准备使用[2]标示电机的启动和停止
//  定义十路线圈的大小
uint8_t REG_COILS_BUF[REG_COILS_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 准备使用[2]标示电机的方向
//  声明离散量缓冲区，并初始化，用于存储十路离散量的状态
uint8_t REG_DISC_BUF[REG_DISC_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
/**
 * @brief CMD4命令处理回调函数
 * 
 * 该函数用于处理MODBUS协议中的CMD4命令，即读取输入寄存器。
 * 它将指定地址范围内的输入寄存器的值复制到缓冲区中。
 *
 * @param pucRegBuffer 指向用于存储寄存器值的缓冲区的指针。
 * @param usAddress 要读取的起始寄存器地址。
 * @param usNRegs 要读取的寄存器数量。
 *
 * @return 返回执行结果的错误代码。
 */
eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    // 计算寄存器索引，从0开始
    USHORT usRegIndex = usAddress - 1;

    // 非法检测：检查访问范围是否超出寄存器缓冲区大小
    if ((usRegIndex + usNRegs) > REG_INPUT_SIZE)
    {
        return MB_ENOREG;
    }

    // 循环读取寄存器值并写入缓冲区
    while (usNRegs > 0)
    {
        // 将寄存器的高8位写入缓冲区
        *pucRegBuffer++ = (unsigned char)(REG_INPUT_BUF[usRegIndex] >> 8);
        // 将寄存器的低8位写入缓冲区
        *pucRegBuffer++ = (unsigned char)(REG_INPUT_BUF[usRegIndex] & 0xFF);
        usRegIndex++;
        usNRegs--;
    }

    return MB_ENOERR;
}

/**
 * @brief CMD6、3、16命令处理回调函数
 * 
 * 该函数用于处理Modbus协议中的 Holding Registers 读写请求。
 * 它根据请求的模式（读或写）对指定的寄存器进行相应的操作。
 *
 * @param pucRegBuffer 寄存器数据缓冲区，用于读取或写入寄存器数据。
 * @param usAddress 请求访问的起始寄存器地址。
 * @param usNRegs 请求访问的寄存器数量。
 * @param eMode 访问模式，可以是 MB_REG_WRITE（写寄存器）或 MB_REG_READ（读寄存器）。
 *
 * @return 返回执行结果，如果成功则返回 MB_ENOERR，否则返回相应的错误代码。
 */
eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    // 计算寄存器索引，Modbus 地址从 1 开始，数组索引从 0 开始，因此需要减 1。
    USHORT usRegIndex = usAddress - 1;

    // 非法检测：检查访问范围是否超出寄存器缓冲区大小。
    if ((usRegIndex + usNRegs) > REG_HOLD_SIZE)
    {
        return MB_ENOREG;
    }

    // 写寄存器
    if (eMode == MB_REG_WRITE)
    {
        // 循环将每个寄存器的数据从缓冲区写入到寄存器中。
        while (usNRegs > 0)
        {
            REG_HOLD_BUF[usRegIndex] = (pucRegBuffer[0] << 8) | pucRegBuffer[1];
            pucRegBuffer += 2;
            usRegIndex++;
            usNRegs--;
        }
    }
    // 读寄存器
    else
    {
        // 循环将每个寄存器的数据从寄存器中读取到缓冲区。
        while (usNRegs > 0)
        {
            *pucRegBuffer++ = (unsigned char)(REG_HOLD_BUF[usRegIndex] >> 8);
            *pucRegBuffer++ = (unsigned char)(REG_HOLD_BUF[usRegIndex] & 0xFF);
            usRegIndex++;
            usNRegs--;
        }
    }

    return MB_ENOERR;
}

/**
 * @brief CMD1、5、15命令处理回调函数
 * 
 * 该函数用于处理Modbus协议中的CMD1、CMD5和CMD15命令。
 * 它主要负责读取或写入寄存器中的位数据。
 *
 * @param pucRegBuffer 指向寄存器缓冲区的指针，用于读取或写入数据。
 * @param usAddress 要操作的寄存器起始地址。
 * @param usNCoils 要操作的位数。
 * @param eMode 操作模式，可以是读或写。
 *
 * @return 返回操作结果，如果成功则返回MB_ENOERR，否则返回相应的错误代码。
 */
eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode)
{
    // 计算寄存器索引
    USHORT usRegIndex = usAddress - 1;
    // 用于位操作的变量
    UCHAR ucBits = 0;
    // 用于存储位状态的变量
    UCHAR ucState = 0;
    // 用于循环操作的变量
    UCHAR ucLoops = 0;

    
    // 非法检测：检查访问范围是否超出寄存器缓冲区大小。
    if ((usRegIndex + usNCoils) > REG_COILS_SIZE)
    {
        return MB_ENOREG;
    }

    // 根据操作模式执行相应的操作
    if (eMode == MB_REG_WRITE)
    {
        // 计算需要循环的次数
        ucLoops = (usNCoils - 1) / 8 + 1;
        // 写操作
        while (ucLoops != 0)
        {
            // 获取当前寄存器的状态
            ucState = *pucRegBuffer++;
            // 位操作
            ucBits = 0;
            // 遍历每个位
            while (usNCoils != 0 && ucBits < 8)
            {
                // 将状态写入寄存器缓冲区
                REG_COILS_BUF[usRegIndex++] = (ucState >> ucBits) & 0X01;
                // 更新剩余位数
                usNCoils--;
                // 更新位索引
                ucBits++;
            }
            // 更新循环次数
            ucLoops--;
        }
    }
    else
    {
        // 计算需要循环的次数
        ucLoops = (usNCoils - 1) / 8 + 1;
        // 读操作
        while (ucLoops != 0)
        {
            // 初始化状态变量
            ucState = 0;
            // 位操作
            ucBits = 0;
            // 遍历每个位
            while (usNCoils != 0 && ucBits < 8)
            {
                // 根据寄存器缓冲区的状态更新状态变量
                if (REG_COILS_BUF[usRegIndex])
                {
                    ucState |= (1 << ucBits);
                }
                // 更新剩余位数
                usNCoils--;
                // 更新寄存器索引
                usRegIndex++;
                // 更新位索引
                ucBits++;
            }
            // 将状态写入寄存器缓冲区
            *pucRegBuffer++ = ucState;
            // 更新循环次数
            ucLoops--;
        }
    }

    return MB_ENOERR;
}

/**
 * @brief CMD2命令处理回调函数
 * 
 * 该函数用于处理MODBUS协议中的CMD2命令，主要负责读取离散输入寄存器的值。
 *
 * @param pucRegBuffer 指向存放寄存器数据的缓冲区。
 * @param usAddress 寄存器的起始地址。
 * @param usNDiscrete 要读取的离散输入寄存器的数量。
 *
 * @return 返回执行结果，如果成功则返回MB_ENOERR，否则返回相应的错误代码。
 */
eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    // 计算寄存器索引，从0开始
    USHORT usRegIndex = usAddress - 1;
    // 用于处理位操作的变量
    UCHAR ucBits = 0;
    // 用于存储当前寄存器状态的变量
    UCHAR ucState = 0;
    // 用于控制循环次数的变量
    UCHAR ucLoops = 0;

    // 非法检测：检查访问范围是否超出寄存器缓冲区大小
    if ((usRegIndex + usNDiscrete) > REG_DISC_SIZE)
    {
        return MB_ENOREG;
    }

    // 计算需要循环的次数，每次循环处理最多8个离散输入
    ucLoops = (usNDiscrete - 1) / 8 + 1;
    // 循环读取离散输入寄存器的值
    while (ucLoops != 0)
    {
        ucState = 0;
        ucBits = 0;
        // 读取每个寄存器的值，并将其状态更新到ucState变量中
        while (usNDiscrete != 0 && ucBits < 8)
        {
            if (REG_DISC_BUF[usRegIndex])
            {
                ucState |= (1 << ucBits);
            }
            usNDiscrete--;
            usRegIndex++;
            ucBits++;
        }
        // 将读取到的状态值存入缓冲区中
        *pucRegBuffer++ = ucState;
        ucLoops--;
    }

    // 模拟离散量输入被改变，这里简单地将每个寄存器的状态取反
    for (usRegIndex = 0; usRegIndex < REG_DISC_SIZE; usRegIndex++)
    {
        REG_DISC_BUF[usRegIndex] = !REG_DISC_BUF[usRegIndex];
    }

    return MB_ENOERR;
}

/**
 * @brief 将 Modbus 库错误码映射为 Modbus 异常码
 * 
 * @param error Modbus 库错误码（eMBErrorCode）
 * @return 对应的 Modbus 异常码
 */
uint8_t mapErrorToException(eMBErrorCode error)
{
    switch (error)
    {
        case MB_ENOREG:     // 非法寄存器地址
            return 0x02;    // 非法数据地址
        case MB_EINVAL:     // 非法参数
            return 0x03;    // 非法数据值
        case MB_ENORES:     // 资源不足
            return 0x04;    // 从站设备故障
        case MB_ETIMEDOUT:  // 超时
            return 0x06;    // 从站设备忙
        case MB_EPORTERR:   // 端口错误
            return 0x04;    // 从站设备故障
        case MB_ENOERR:     // 无错误
            return 0x00;    // 无异常
        default:            // 未知错误
            return 0x04;    // 从站设备故障
    }
}
```

## 6 port.h移植

添加 头文件

```c
#include "usart.h
#include "tim.h"
```

## 7 portserial.c

```c
//1. 开启和关闭中断
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
    // 开启或关闭中断
    if (xRxEnable)
    {
        /* Enable the UART receive interrupt. */
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    }
    else
    {
        /* Disable the UART receive interrupt. */
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
    }
    if (xTxEnable)
    {
        /* Enable the UART transmit interrupt. */
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TXE);
    }
    else
    {
        /* Disable the UART transmit interrupt. */
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_TXE);
    }
}

//2. 初始化 把返回值改为true
BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
    // 初始化
    // MX_USART2_UART_Init();

    return TRUE;
}

//3. 发送数据
BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
    USART2->DR = ucByte & 0xFF; // 将字节写入数据寄存器
    return TRUE;
}

//4. 接收数据
BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
    *pucByte = (CHAR)(USART2->DR & 0xFF); // 从数据寄存器读取字节
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR(void)
{
    pxMBFrameCBByteReceived();
}

//5. 中断服务程序
void USART2_IRQHandler(void)
{
    /* Check if the interrupt is for the receive buffer not empty */
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET)
    {
        __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_RXNE); // Clear the RXNE flag
        /* Call the receive ISR */
        prvvUARTRxISR();
    }

    /* Check if the interrupt is for the transmit buffer empty */
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) != RESET)
    {
        __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TXE); // Clear the TXE flag
        /* Call the transmit ISR */
        prvvUARTTxReadyISR();
    }

    //可以清除其他中断
    HAL_UART_IRQHandler(&huart2); 
}
```

## 8 porttimer.c

```c
//1. 初始化 改返回值为true
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    return TRUE;
}


//2. 使能定时器
inline void
vMBPortTimersEnable()
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    // 清空计数器的值
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    // 使能定时器
    HAL_TIM_Base_Start_IT(&htim3);
}

//3. 关闭定时器
inline void
vMBPortTimersDisable()
{
    /* Disable any pending timers. */
    // 停止定时器
    HAL_TIM_Base_Stop_IT(&htim3);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR(void)
{
    (void)pxMBPortCBTimerExpired();
}

//4. 中断服务程序 
void TIM3_IRQHandler(void)
{
    /* Check if the interrupt is for the timer expired */
    if (__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // 清除更新中断标志
        /* Call the timer expired ISR */
        prvvTIMERExpiredISR();
    }

    HAL_TIM_IRQHandler(&htim3);
}
```

## 9 去除断言依赖

此时编译代码会报错：

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F08%2F23%2F13-59-01-48c4e25041fd7e8dfed7df167e4d52f2-image-e8a5c3.png)

进入keil软件中，在C/C++栏中添加**-DNDEBUG**字段，去除断言依赖：

```c
-DNDEBUG
```

![](https://cdn.jsdelivr.net/gh/gybpersist/imges/images%2F2025%2F08%2F23%2F13-59-21-88d83a9aec011d7da17a4c5cc46a8ef6-image%20-1--e4e70c.png)