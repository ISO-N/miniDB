#include "utils.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

// 输入验证函数实现

// 验证ID范围：检查ID是否为正数
// 用于查找操作，链表ID没有上限限制，只需检查是否为正数
int validate_id_range(int id_num) {
    if (id_num < 1) {
        printf("错误：ID必须为正整数！\n");
        return 0; // 验证失败
    }
    return 1; // 验证成功
}

// 验证姓名：检查长度和非空
int validate_name(const char *name) {
    if (name[0] == '\0') {
        printf("错误：姓名不能为空！\n");
        return 0;
    }
    if (strlen(name) > MAX_NAME_LEN) {
        printf("错误：姓名长度不能超过%d个字符！\n", MAX_NAME_LEN);
        return 0;
    }
    return 1;
}

// 验证年龄：合理范围检查
int validate_age(int age) {
    if (age < 1 || age > 150) {
        printf("错误：年龄必须在1到150之间！\n");
        return 0;
    }
    return 1;
}

// 验证成绩：0-100分
int validate_score(double score) {
    if (score < 0.0 || score > 100.0) {
        printf("错误：成绩必须在0到100之间！\n");
        return 0;
    }
    return 1;
}