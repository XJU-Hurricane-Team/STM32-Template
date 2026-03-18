/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.h
  * @brief   This file contains all the function prototypes for
  *          the fdcan.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FDCAN_H__
#define __FDCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern FDCAN_HandleTypeDef hfdcan1;

extern FDCAN_HandleTypeDef hfdcan2;

extern FDCAN_HandleTypeDef hfdcan3;

/* USER CODE BEGIN Private defines */

/**
 * @brief Select which CAN will be used.
 */
typedef enum {
    can1_selected = 0U, /*!< Select CAN1 */
    can2_selected,      /*!< Select CAN2 */
    can3_selected       /*!< Select CAN3 */
} can_selected_t;

/* Wait for can tx mailbox empty times. */
#define CAN_SEND_TIMEOUT        1000

/* 需要手动说明CAN的使能状态 */
#define CAN1_ENABLE 0   
#define CAN2_ENABLE 0   
#define CAN3_ENABLE 0

#define FDCAN1_ENABLE 1
#define FDCAN2_ENABLE 1
#define FDCAN3_ENABLE 1

/* USER CODE END Private defines */

void MX_FDCAN1_Init(void);
void MX_FDCAN2_Init(void);
void MX_FDCAN3_Init(void);

/* USER CODE BEGIN Prototypes */

/*****************************************************************************
 * @defgroup Public functions of CAN.
 * @{
 */

/* Compatibility with CAN Classic. */
#define can_send_message fdcan_send_message
#define can_send_remote  fdcan_send_remote
#define CAN_ID_STD       FDCAN_STANDARD_ID
#define CAN_ID_EXT       FDCAN_EXTENDED_ID
#define CAN_RTR_DATA     FDCAN_DATA_FRAME
#define CAN_RTR_REMOTE   FDCAN_REMOTE_FRAME

FDCAN_HandleTypeDef *fdcan_get_handle(can_selected_t can_selected);
uint8_t fdcan_send_message(can_selected_t can_selected, uint32_t can_ide,
                           uint32_t id, uint8_t len, const uint8_t *msg);
uint8_t fdcan_send_remote(can_selected_t can_selected, uint32_t can_ide,
                          uint32_t id, uint8_t len, const uint8_t *msg);

/**
 * @}
 */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H__ */

