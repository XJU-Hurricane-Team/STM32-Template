/**
 * @file    rtos_tasks.c
 * @author  Deadline039
 * @brief   RTOS tasks.
 * @version 1.0
 * @date    2024-01-31
 */

#include "includes.h"

void start_task(void *pvParameters);

void task_alive(void *pvParameters);

void task_uart_test(void *pvParameters);

void task_key_test(void *pvParameters);

UART_HandleTypeDef *const test_uart = &huart1;
/* 正式使用模板时，请将test任务删除，它们占用了很多RAM */
/*****************************************************************************/

/**
 * @brief FreeRTOS start up.
 *
 */
void freertos_start(void) {
    APP_TASK_CREATE(start_task, 512, 2);
    vTaskStartScheduler();
}

/**
 * @brief Start up task.
 *
 * @param pvParameters Start parameters.
 */
void start_task(void *pvParameters) {
    UNUSED(pvParameters);
    taskENTER_CRITICAL();

    APP_TASK_CREATE(task_alive, 128, 2);
    APP_TASK_CREATE(task_uart_test, 512, 2);
    APP_TASK_CREATE(task_key_test, 512, 2);

    taskEXIT_CRITICAL();
    vTaskDelete(NULL);
}

/**
 * @brief LED heartbeat.
 *
 * @param pvParameters Start parameters.
 */
void task_alive(void *pvParameters) {
    UNUSED(pvParameters);

    LED0_OFF();
    LED1_ON();

    while (1) {
        LED0_TOGGLE();
        LED1_TOGGLE();
        vTaskDelay(1000);
    }
}

/**
 * @brief UART receive and periodic transmit test.
 *
 * @param pvParameters Start parameters.
 */
void task_uart_test(void *pvParameters) {
    UNUSED(pvParameters);

    uint8_t buf[20] = {0};

    while (1) {
        uint32_t len = uart_dmarx_read(test_uart, buf, sizeof(buf) - 1);
        if (len > 0) {
            buf[len] = '\0';
            uart_printf(test_uart, "Received: %s.\n", buf);
        } else {
            uart_printf(test_uart,
                "STM32G4xx UART test. Running time: %lu ms.\n",
                (unsigned long)xTaskGetTickCount());
        }
        vTaskDelay(1000);
    }
}

/**
 * @brief Key scan and event display test.
 *
 * @param pvParameters Start parameters.
 */
void task_key_test(void *pvParameters) {
    UNUSED(pvParameters);
    uint8_t buf[20] = {0};

    key_press_t key = KEY_NO_PRESS;

    while (1) {
        key = key_scan(0);
        switch (key) {
            case WKUP_PRESS: {
                uart_printf(test_uart, "Wake Up Pressed. \n", buf);
            } break;

            case KEY0_PRESS: {
                uart_printf(test_uart, "KEY0 Pressed. \n", buf);
            } break;

            case KEY1_PRESS: {
                uart_printf(test_uart, "KEY1 Pressed. \n", buf);
            } break;

            case KEY2_PRESS: {
                uart_printf(test_uart, "KEY2 Pressed. \n", buf);
            } break;

            default: {
            } break;
        }

        vTaskDelay(10);
    }
}

#ifdef configASSERT
/**
 * @brief FreeRTOS assert failed function.
 *
 * @note  断言失败后内核状态已经不可信，直接停机，不再返回调用点。
 *
 * @param pcFile File name
 * @param ulLine File line
 */
void vAssertCalled(const char *pcFile, unsigned int ulLine) {
    fprintf(stderr, "FreeRTOS assert failed. File: %s, line: %u. \n", pcFile,
            ulLine);
    Error_Handler();
}
#endif /* configASSERT */

#if configCHECK_FOR_STACK_OVERFLOW
/**
 * @brief The application stack overflow hook is called when a stack overflow is detected for a task.
 *
 * @param xTask the task that just exceeded its stack boundaries.
 * @param pcTaskName A character string containing the name of the offending task.
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    UNUSED(xTask);
    fprintf(stderr, "Stack overflow! Taskname: %s. \n", pcTaskName);
}
#endif /* configCHECK_FOR_STACK_OVERFLOW */

#if configUSE_MALLOC_FAILED_HOOK
/**
 * @brief This hook function is called when allocation failed.
 *
 */
void vApplicationMallocFailedHook(void) {
    fprintf(stderr, "FreeRTOS malloc failed! \n");
}
#endif /* configUSE_MALLOC_FAILED_HOOK */
