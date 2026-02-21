#ifndef UTILS_H
#define UTILS_H

// 输入验证函数原型
int validate_id_range(int id_num);  // 检查ID是否为正数
int validate_name(const char *name);
int validate_age(int age);
int validate_score(double score);

#endif