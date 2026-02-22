/*
 * utils.h - MiniDB 工具函数头文件
 * 阶段六：高级特性 — 位操作、国际化、数学支持
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>  // 使用 bool 类型

/* 输入验证函数原型 */
bool validate_id_range(int id_num);      /* 检查 ID 是否为正数 */
bool validate_name(const char *name);    /* 检查姓名有效性 */
bool validate_age(int age);              /* 检查年龄范围 */
bool validate_score(double score);       /* 检查成绩范围 */

/* 输入辅助函数 */
int read_int(const char *prompt, int *value);        /* 读取并验证整数输入 */
int read_double(const char *prompt, double *value);  /* 读取并验证浮点数输入 */
void clear_input_buffer(void);                       /* 清空输入缓冲区 */

#endif /* UTILS_H */
