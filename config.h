#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdint.h>   // C99 定宽类型：uint8_t, int32_t 等
#include <stdbool.h>  // C99 布尔类型：bool, true, false

#define MAX_NAME_LEN 64  // 姓名最大长度

/* 文件名称常量 */
#define DB_FILENAME   "minidb.dat"   // 二进制数据库文件
#define CSV_FILENAME  "minidb.csv"   // CSV 导出文件

/* 调试模式开关 */
#ifdef DEBUG
    #define DEBUG_PRINT(fmt,...) fprintf(stderr,"[DEBUG] "fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt,...)
#endif

#endif