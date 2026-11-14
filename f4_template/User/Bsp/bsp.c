/**
 * @file    bsp.c
 * @author  Deadline039
 * @brief   Bsp layer initialize.
 * @version 1.0
 * @date    2024-09-18
 */

#include <bsp.h>

#include "FreeRTOS.h"

/**
 * @brief Bsp layer initiallize.
 *
 */
void bsp_init(void) {
    uint8_t can_list_state;

    uart_ex_init();

    /* 扩展 ID 表长度为 0 时接收查表会除零，标准表和扩展表都要建出来 */
    can_list_state = can_list_add_can(can1_selected, 4, 4);
    configASSERT(can_list_state == 0);
}

#ifdef USE_FULL_ASSERT

#include <stdio.h>

/**
 * @brief HAL assert failed.
 *
 * @param file File name.
 * @param line Line.
 */
void assert_failed(uint8_t *file, uint32_t line) {
    fprintf(stderr, "HAL assert failed. In file: %s, line: %u. \n", file, line);
    Error_Handler();
}

#endif /* USE_FULL_ASSERT */