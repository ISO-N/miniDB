/*
 * utils.h - MiniDB 工具函数头文件
 * 阶段五：文件 I/O + 排序 + 错误处理
 */

#ifndef UTILS_H
#define UTILS_H

/* 输入验证函数原型 */
int validate_id_range(int id_num);   /* 检查 ID 是否为正数 */
int validate_name(const char *name); /* 检查姓名有效性 */
int validate_age(int age);           /* 检查年龄范围 */
int validate_score(double score);    /* 检查成绩范围 */

/* 输入辅助函数 */
int read_int(const char *prompt, int *value);        /* 读取并验证整数输入 */
int read_double(const char *prompt, double *value);  /* 读取并验证浮点数输入 */
void clear_input_buffer(void);                       /* 清空输入缓冲区 */

#endif /* UTILS_H */
