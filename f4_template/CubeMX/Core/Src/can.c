/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_4TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

    CAN_FilterTypeDef can_filter_config;

    can_filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_config.FilterIdHigh = 0x0000;
    can_filter_config.FilterIdLow = 0x0000;
    can_filter_config.FilterMaskIdHigh = 0x0000;
    can_filter_config.FilterMaskIdLow = 0x0000;
    can_filter_config.FilterActivation = CAN_FILTER_ENABLE;
    can_filter_config.SlaveStartFilterBank = 14;

    /* CAN1 Filter Configuration */
    can_filter_config.FilterBank = 0;
    can_filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    if (HAL_CAN_ConfigFilter(&hcan1, &can_filter_config) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan1,
                                     CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }
    
    can_filter_config.FilterBank = 1;
    can_filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    if (HAL_CAN_ConfigFilter(&hcan1, &can_filter_config) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan1,
                                     CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }



    if (HAL_CAN_Start(&hcan1) != HAL_OK) {
        Error_Handler();
    }

  /* USER CODE END CAN1_Init 2 */

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 3;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = ENABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */
    CAN_FilterTypeDef can_filter_config;

    can_filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_config.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_config.FilterIdHigh = 0x0000;
    can_filter_config.FilterIdLow = 0x0000;
    can_filter_config.FilterMaskIdHigh = 0x0000;
    can_filter_config.FilterMaskIdLow = 0x0000;
    can_filter_config.FilterActivation = CAN_FILTER_ENABLE;
    can_filter_config.SlaveStartFilterBank = 14;

    /* CAN2 Filter Configuration */
    can_filter_config.FilterBank = 14;
    can_filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    if (HAL_CAN_ConfigFilter(&hcan2, &can_filter_config) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_CAN_ActivateNotification(&hcan2,
                                     CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }

    can_filter_config.FilterBank = 15;
    can_filter_config.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    if (HAL_CAN_ConfigFilter(&hcan2, &can_filter_config) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_CAN_ActivateNotification(&hcan2,
                                     CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_CAN_Start(&hcan2) != HAL_OK) {
        Error_Handler();
    }
  /* USER CODE END CAN2_Init 2 */

}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN2 interrupt Init */
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_6);

    /* CAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief Get the CAN handle with specified CAN.
 *
 * @param can_selected Specific which can will to get.
 * @return The handle of CAN. return NULL which the CAN doesn't exist.
 */
static inline CAN_HandleTypeDef *can_get_handle(can_selected_t can_selected) {
    switch (can_selected) {

#if CAN1_ENABLE
        case can1_selected:
            return &hcan1;
#endif /* CAN1_ENABLE */

#if CAN2_ENABLE
        case can2_selected:
            return &hcan2;
#endif /* CAN2_ENABLE */

#if CAN3_ENABLE
        case can3_selected:
            return &hcan3;
#endif /* CAN3_ENABLE */

        default:
            return NULL;
    }
}

/**
 * @brief CAN send message.
 *
 * @param can_selected Specific which CAN to send message.
 * @param can_ide Specific standard ID or Extend ID.
 * @param id Specific message id.
 * @param len Specific message length.
 * @param msg Specific message content.
 * @return Send status.
 * @retval - 0: Success.
 * @retval - 1: Send error.
 * @retval - 2: Timeout.
 * @retval - 3: Parameter invalid.
 * @retval - 4: This CAN is not initialized.
 */
uint8_t can_send_message(can_selected_t can_selected, uint32_t can_ide,
                         uint32_t id, uint8_t len, const uint8_t *msg) {
    CAN_HandleTypeDef *can_handle = can_get_handle(can_selected);
    if (can_handle == NULL) {
        return 3;
    }

    if (len > 8) {
        return 3;
    }

    if (HAL_CAN_GetState(can_handle) == HAL_CAN_STATE_RESET) {
        return 4;
    }

    uint16_t wait_time = 0;
    uint32_t tx_mail_box = CAN_TX_MAILBOX0;

    CAN_TxHeaderTypeDef tx_header = {0};
    tx_header.IDE = can_ide;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = len;
    if (can_ide == CAN_ID_STD) {
        tx_header.StdId = id;
    } else {
        tx_header.ExtId = id;
    }

    while (HAL_CAN_GetTxMailboxesFreeLevel(can_handle) == 0) {
        /* Wait to all mailbox is empty. */
        ++wait_time;
        if (wait_time > CAN_SEND_TIMEOUT) {
            return 2;
        }
    }

    if (HAL_CAN_AddTxMessage(can_handle, &tx_header, msg, &tx_mail_box) !=
        HAL_OK) {
        return 1;
    }
    return 0;
}

/**
 * @brief CAN send remote message.
 *
 * @param can_selected Specific which CAN to send message.
 * @param can_ide Specific standard ID or Extend ID.
 * @param id Specific message id.
 * @param len Specific message length.
 * @param msg Specific message content.
 * @return Send status.
 * @retval - 0: Success.
 * @retval - 1: Send error.
 * @retval - 2: Timeout.
 * @retval - 3: Parameter invalid.
 * @retval - 4: This CAN is not initialized.
 */
uint8_t can_send_remote(can_selected_t can_selected, uint32_t can_ide,
                        uint32_t id, uint8_t len, const uint8_t *msg) {
    CAN_HandleTypeDef *can_handle = can_get_handle(can_selected);
    if (can_handle == NULL) {
        return 3;
    }

    if (len > 8) {
        return 3;
    }

    if (HAL_CAN_GetState(can_handle) == HAL_CAN_STATE_RESET) {
        return 4;
    }

    uint16_t wait_time = 0;
    uint32_t tx_mail_box = CAN_TX_MAILBOX0;

    CAN_TxHeaderTypeDef tx_header = {0};
    tx_header.IDE = can_ide;
    tx_header.RTR = CAN_RTR_REMOTE;
    tx_header.DLC = len;
    if (can_ide == CAN_ID_STD) {
        tx_header.StdId = id;
    } else {
        tx_header.ExtId = id;
    }

    while (HAL_CAN_GetTxMailboxesFreeLevel(can_handle) == 0) {
        /* Wait to all mailbox is empty. */
        ++wait_time;
        if (wait_time > CAN_SEND_TIMEOUT) {
            return 2;
        }
    }

    if (HAL_CAN_AddTxMessage(can_handle, &tx_header, msg, &tx_mail_box) !=
        HAL_OK) {
        return 1;
    }

    return 0;
}

/* USER CODE END 1 */

