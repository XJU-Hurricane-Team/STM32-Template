/**
 * @file    rtos_tasks.c
 * @author  Deadline039
 * @brief   RTOS tasks.
 * @version 1.0
 * @date    2024-01-31
 */

#include "includes.h"

static TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

static TaskHandle_t task_alive_handle;
void task_alive(void *pvParameters);

static TaskHandle_t task_uart_test_handle;
void task_uart_test(void *pvParameters);

static TaskHandle_t task_key_test_handle;
void task_key_test(void *pvParameters);

/*****************************************************************************/

/**
 * @brief FreeRTOS start up.
 *
 */
void freertos_start(void) {
    xTaskCreate(start_task, "start_task", 128, NULL, 2, &start_task_handle);
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

    xTaskCreate(task_alive, "task_alive", 128, NULL, 2, &task_alive_handle);
    xTaskCreate(task_uart_test, "task_uart_test", 512, NULL, 2,
                &task_uart_test_handle);
    xTaskCreate(task_key_test, "task_key_test", 512, NULL, 2,
                &task_key_test_handle);

    taskEXIT_CRITICAL();
    vTaskDelete(start_task_handle);
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

    UART_HandleTypeDef *const uart = &huart1;
    uint8_t buf[20] = {0};

    while (1) {
        uint32_t len = uart_dmarx_read(uart, buf, sizeof(buf) - 1);
        if (len > 0) {
            buf[len] = '\0';
            uart_printf(uart, "Received: %s.\n", buf);
        } else {
            uart_printf(uart,
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

    key_press_t key = KEY_NO_PRESS;

    while (1) {
        key = key_scan(0);
        switch (key) {
            case WKUP_PRESS: {
                printf("Wake Up Pressed. \n");
            } break;

            case KEY0_PRESS: {
                printf("KEY0 Pressed. \n");
            } break;

            case KEY1_PRESS: {
                printf("KEY1 Pressed. \n");
            } break;

            case KEY2_PRESS: {
                printf("KEY2 Pressed. \n");
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
 * @param pcFile File name
 * @param ulLine File line
 */
void vAssertCalled(const char *pcFile, unsigned int ulLine) {
    fprintf(stderr, "FreeRTOS assert failed. File: %s, line: %u. \n", pcFile,
            ulLine);
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
