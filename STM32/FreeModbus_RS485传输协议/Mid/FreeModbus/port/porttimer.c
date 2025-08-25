/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    // 开发者代码 start --------------
    return TRUE;
    // 开发者代码 end --------------

    // return FALSE;
}

inline void
vMBPortTimersEnable()
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    // 开发者代码 start --------------
    // 清空定时器
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    // 使能定时器
    HAL_TIM_Base_Start_IT(&htim3);
    // 开发者代码 end --------------
}

inline void
vMBPortTimersDisable()
{
    /* Disable any pending timers. */
    // 开发者代码 start --------------
    // 关闭定时器
    HAL_TIM_Base_Stop_IT(&htim3);
    // 开发者代码 end --------------
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR(void)
{
    (void)pxMBPortCBTimerExpired();
}

// 开发者代码 start --------------
// 4. 中断服务程序
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
// 开发者代码 end --------------
