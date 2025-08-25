/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    can.c
 * @brief   This file provides code for the configuration
 *          of the CAN instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 36;
  hcan.Init.Mode = CAN_MODE_SILENT_LOOPBACK;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = ENABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (canHandle->Instance == CAN1)
  {
    /* USER CODE BEGIN CAN1_MspInit 0 */

    /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN CAN1_MspInit 1 */

    /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *canHandle)
{

  if (canHandle->Instance == CAN1)
  {
    /* USER CODE BEGIN CAN1_MspDeInit 0 */

    /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

    /* USER CODE BEGIN CAN1_MspDeInit 1 */

    /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// 开发者代码 start ------------------------------------------
void Can_Filter_Config(void)
{
  CAN_FilterTypeDef sFilterConfig;
  sFilterConfig.FilterBank = 0; // 过滤器0
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK; // 掩码模式
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; // 32位模式
  sFilterConfig.FilterIdHigh = 0x0000; // 标准ID
  sFilterConfig.FilterIdLow = 0x0000; // 标准ID低16位
  sFilterConfig.FilterMaskIdHigh = 0x0000; // 掩码ID高16位
  sFilterConfig.FilterMaskIdLow = 0x0000; // 掩码ID低16位
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; // 分配到FIFO0
  sFilterConfig.FilterActivation = ENABLE; // 使能过滤器
  HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
}

void Can_Send_Msg(uint32_t id, uint8_t *data, uint8_t len)
{
  // 等待发送邮箱有空闲
  while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0)
  {}
  
  // 填入数据帧信息
  CAN_TxHeaderTypeDef sTxHeader;
  uint32_t ulMailbox;   //没有用到,用来储存返回来的数据
  sTxHeader.StdId = id;
  sTxHeader.ExtId = 0x00; 
  sTxHeader.RTR = CAN_RTR_DATA; 
  sTxHeader.IDE = CAN_ID_STD; 
  sTxHeader.DLC = len;
  sTxHeader.TransmitGlobalTime = DISABLE;
  HAL_CAN_AddTxMessage(&hcan, &sTxHeader, data, &ulMailbox);
}

void Can_Receive_Msg(RxDataMsg_Info rxMsg[], uint8_t *msgCount)
{
  // 获取接收FIFO0中的消息数量
  uint32_t ulRxMessages = HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0);
  *msgCount = ulRxMessages;

  // 定义头结构体
  CAN_RxHeaderTypeDef sRxHeader;
  // 从FIFO0中读取消息
  for (uint32_t i = 0; i < ulRxMessages; i++)
  {
    HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &sRxHeader, rxMsg[i].data);
    rxMsg[i].id = sRxHeader.StdId;
    rxMsg[i].len = sRxHeader.DLC;
  }
}
// 开发者代码 end --------------------------------------------

/* USER CODE END 1 */
