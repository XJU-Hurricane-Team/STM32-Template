/**
 * @file    includes.h
 * @author  Deadline039
 * @brief   Include files
 * @version 1.0
 * @date    2024-04-03
 */

#ifndef __INCLUDES_H
#define __INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <bsp.h>

#include "FreeRTOS.h"
#include "task.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void freertos_start(void);

/**
 * @brief 创建任务，失败时打印任务名和剩余堆并停机。
 *
 * @param entry       任务入口函数，同时作为任务名
 * @param stack_words 栈深度，单位是 StackType_t（字），不是字节
 * @param prio        任务优先级
 * @note  不返回句柄：需要时用 xTaskGetHandle("函数名") 事后查询，
 *        或直接调用 xTaskCreate 自己接句柄。
 *        只能在调度器启动前或任务上下文调用。
 */
#define APP_TASK_CREATE(entry, stack_words, prio)                              \
    do {                                                                       \
        if (xTaskCreate(entry, #entry, stack_words, NULL, prio, NULL) !=       \
            pdPASS) {                                                          \
            fprintf(stderr, "%s create failed, free heap=%u\n", #entry,        \
                    (unsigned int)xPortGetFreeHeapSize());                     \
            Error_Handler();                                                   \
        }                                                                      \
    } while (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCLUDES_H */
