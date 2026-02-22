/*
 * utils.c - MiniDB 工具函数实现
 * 阶段六：高级特性 — 位操作、国际化、数学支持
 */

#include "utils.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

/*
 * validate_id_range - 验证 ID 范围
 * 检查 ID 是否为正数
 */
bool validate_id_range(int id_num) {
    if (id_num < 1) {
        printf("错误：ID 必须为正整数！\n");
        return false;
    }
    return true;
}

/*
 * validate_name - 验证姓名
 * 检查姓名非空且长度不超过限制
 */
bool validate_name(const char *name) {
    if (name == NULL || name[0] == '\0') {
        printf("错误：姓名不能为空！\n");
        return false;
    }
    if (strlen(name) > MAX_NAME_LEN) {
        printf("错误：姓名长度不能超过 %d 个字符！\n", MAX_NAME_LEN);
        return false;
    }
    return true;
}

/*
 * validate_age - 验证年龄
 * 检查年龄是否在合理范围内 (1-150)
 */
bool validate_age(int age) {
    if (age < 1 || age > 150) {
        printf("错误：年龄必须在 1 到 150 之间！\n");
        return false;
    }
    return true;
}

/*
 * validate_score - 验证成绩
 * 检查成绩是否在 0-100 之间
 */
bool validate_score(double score) {
    if (score < 0.0 || score > 100.0) {
        printf("错误：成绩必须在 0 到 100 之间！\n");
        return false;
    }
    return true;
}

/*
 * read_int - 读取并验证整数输入
 * 参数：prompt - 提示信息
 *       value - 存储读取到的值
 * 返回值：1 表示成功，0 表示失败
 */
int read_int(const char *prompt, int *value) {
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        printf("错误：请输入有效的整数！\n");
        clear_input_buffer();
        return 0;
    }
    return 1;
}

/*
 * read_double - 读取并验证浮点数输入
 * 参数：prompt - 提示信息
 *       value - 存储读取到的值
 * 返回值：1 表示成功，0 表示失败
 */
int read_double(const char *prompt, double *value) {
    printf("%s", prompt);
    if (scanf("%lf", value) != 1) {
        printf("错误：请输入有效的数字！\n");
        clear_input_buffer();
        return 0;
    }
    return 1;
}

/*
 * clear_input_buffer - 清空输入缓冲区
 * 用于处理输入错误后的清理
 */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
