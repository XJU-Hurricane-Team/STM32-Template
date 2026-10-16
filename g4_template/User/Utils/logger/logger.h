/**
 * @file    logger.h
 * @author  Deadline039 Jackrainman
 * @brief   日志记录系统
 * @version 1.3
 * @date    2026-10-15
 * @see     https://www.bilibili.com/video/av1250963900/?p=89
 *
 ******************************************************************************
 *    Date    | Version |   Author    | Version Info
 * -----------+---------+-------------+----------------------------------------
 * 2024-04-01 |   1.0   | Deadline039 | 初版
 * 2024-04-04 |   1.1   | Deadline039 | 更改 LOG_ENABLE 宏, 如果关闭此宏开关,
 *            |         |             | 日志函数会变为空函数
 * 2525-03-14 |   1.2   | Deadline039 | 修改输出接口和获取时间接口, 缓冲区加信号量保护,
 *            |         |             | 修改部分宏定义, 移除 RTC 与 FATFS.
 * 2026-10-15 |   1.3   | Jackrainman | 拆分聚合头依赖, 输出渠道使用命名宏,
 *            |         |             | 统一缓冲区输出接口.
 */

#ifndef __LOGGER_H
#define __LOGGER_H

/** 日志系统配置 *************************************************************/

#define LOG_OUTPUT_ITM  0
#define LOG_OUTPUT_RTT  1
#define LOG_OUTPUT_UART 2

#define LOG_ENABLE      1 /* 是否输出日志 */
#define LOG_OUTPUT_TYPE LOG_OUTPUT_UART

/******************************************************************************
 * @defgroup 输出相关配置
 * @{
 */

/* 输出缓冲区大小，建议至少保留64字节，避免日志长度过长 */
#define LOG_BUFFER_SIZE       128
/* 输出末尾换行符 */
#define LOG_OUTPUT_NEWLINE    "\n"

/* 共享缓冲区需要信号量保护，初始化后从运行中的任务调用日志。 */
#define LOG_USE_RTOS          1
/* 使用互斥信号量，日志不在中断中调用。 */
#define LOG_USE_MUTEX         1

/**
 * @}
 */

/******************************************************************************
 * @defgroup 时间显示相关配置
 * @{
 */

/* 是否显示运行时间 */
#define LOG_SHOW_RUNNING_TIME 1

#if LOG_SHOW_RUNNING_TIME
/* 获取时间函数头文件 */
#define LOG_GET_RUNNING_TIME_HEADER_FILE "main.h"
/* 获取运行时间，单位为毫秒 */
#define LOG_GET_RUNNING_TIME()           HAL_GetTick()
#endif /* LOG_SHOW_RUNNING_TIME */

/**
 * @}
 */

/**
 * @brief 日志级别
 */
typedef enum {
    LOG_DEBUG,   /*!< 调试信息 */
    LOG_INFO,    /*!< 一般信息 */
    LOG_WARNING, /*!< 警告信息 */
    LOG_ERROR,   /*!< 错误信息 */
    LOG_FATAL,   /*!< 致命错误 */
    LOG_UNKNOW,  /*!< 未知信息 */
    LOG_FULL     /*!< 内部超长提示，不作为过滤等级使用 */
} log_level_t;

/** 初始化日志系统；在其他任务使用日志前调用一次。 */
void log_init(log_level_t init_level);
/** 打印日志；RTOS 下仅在允许阻塞的任务中调用。 */
/* 正文超长时整条替换为带原时间戳的 FULL 提示。 */
void log_message(log_level_t level, const char *format, ...);
/** 设置最低输出等级；无效等级不修改当前设置。 */
void log_set_level(log_level_t level);

log_level_t log_get_level(void);

#endif /* __LOGGER_H */
