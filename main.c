/*
 * MiniDB - 迷你数据库系统（阶段一原型）
 * 这是一个基于数组的简单学生记录管理系统
 * 功能：添加、查看全部、按ID查找、退出
 * 设计目标：帮助C语言新手理解数组、循环、条件语句和基本输入输出
 */

#include <stdio.h>  // 标准输入输出库，包含printf和scanf函数

// 定义常量：最大记录数
#define MAXN 100    // 最多存储100条学生记录

/*
 * main函数 - 程序入口点
 * 返回值：0表示成功执行，非0表示错误
 */
int main(void) {

    /*
     * 数据结构说明：
     * 使用四个并行数组来存储学生信息，通过相同的索引关联
     * 例如：id[5]=1, name[5]="张三", age[5]=20, grade[5]=85.5 表示ID为5的学生
     *
     * id数组：标记位，0表示该位置无记录，1表示有记录
     * name数组：存储学生姓名（二维字符数组）
     * age数组：存储学生年龄
     * grade数组：存储学生成绩（浮点数）
     */
    int id[MAXN + 1] = {0};                 // 初始化所有元素为0（无记录）
    char name[MAXN + 1][MAXN + 1];         // 姓名数组：最多MAXN+1个姓名，每个姓名最多MAXN个字符
    int age[MAXN + 1];                     // 年龄数组
    double grade[MAXN + 1];                // 成绩数组

    /*
     * 主循环：显示菜单并处理用户选择
     * while(1)创建无限循环，直到用户选择退出（break或return）
     */
    while (1) {

        int choice;  // 存储用户菜单选择

        // 显示菜单界面
        printf("---------------\n");
        printf("请输入你的操作：\n");
        printf("1.添加    2.查看全部\n");
        printf("3.按id查找    4.退出\n");
        printf("---------------\n");

        // 获取用户输入
        // 注意：这里没有错误处理，如果输入非数字会导致问题
        scanf("%d", &choice);

        /*
         * switch语句：根据用户选择执行不同操作
         * 相当于多个if-else的简洁写法
         */
        switch (choice) {

            /* 情况1：添加新学生记录 */
            case 1: {
                printf("请输入学生id（1-%d）：\n", MAXN);
                int tmp_id;  // 临时变量存储输入的ID
                scanf("%d", &tmp_id);

                // 标记该ID位置有记录
                id[tmp_id] = 1;

                printf("请输入学生姓名：\n");
                scanf("%s", name[tmp_id]);  // 注意：这里假设输入不包含空格

                printf("请输入学生年龄：\n");
                scanf("%d", &age[tmp_id]);

                printf("请输入学生成绩：\n");
                scanf("%lf", &grade[tmp_id]);  // %lf用于读取double类型

                printf("记录完成！\n");
                break;  // 跳出switch，回到循环开始
            }

            /* 情况2：查看所有学生记录 */
            case 2: {
                printf("=== 所有学生记录 ===\n");

                // 遍历所有可能的ID（1到MAXN）
                for (int i = 1; i <= MAXN; i++) {
                    // 检查该ID位置是否有记录
                    if (id[i] == 1) {
                        printf("-----------------\n");
                        printf("学生ID：%d\n", i);
                        printf("姓名：%s\n", name[i]);
                        printf("年龄：%d岁\n", age[i]);
                        printf("成绩：%.2f分\n", grade[i]);  // %.2f保留两位小数
                    }
                }

                if (id[1] == 0) {  // 简单检查是否有任何记录（不完善）
                    printf("暂无学生记录。\n");
                }

                break;
            }

            /* 情况3：按ID查找学生 */
            case 3: {
                int search_id;
                printf("请输入你要查找的学生ID：\n");
                scanf("%d", &search_id);

                // 检查该ID是否存在记录
                if (id[search_id] != 1) {
                    printf("学生不存在！\n");
                } else {
                    printf("=== 学生信息 ===\n");
                    printf("学生ID：%d\n", search_id);
                    printf("姓名：%s\n", name[search_id]);
                    printf("年龄：%d岁\n", age[search_id]);
                    printf("成绩：%.2f分\n", grade[search_id]);
                }
                break;
            }

            /* 情况4：退出程序 */
            case 4: {
                printf("程序退出！感谢使用MiniDB。\n");
                return 0;  // 直接退出main函数，结束程序
            }

            /* 默认情况：处理无效输入 */
            default: {
                printf("错误：请输入1-4之间的数字！\n");
                // 清空输入缓冲区（简单处理）
                while (getchar() != '\n');  // 读取并丢弃直到换行符
                break;
            }
        }

        // 添加空行使界面更清晰
        printf("\n");
    }

    return 0;  // 理论上不会执行到这里（因为while(1)循环）
}

/*
 * 代码设计说明（给新手的学习提示）：
 *
 * 1. 数组索引从0开始，但这里使用1-MAXN是为了让ID从1开始更自然
 * 2. id数组用作标记位：0=空位，1=有记录
 * 3. 这个设计有局限性：
 *    - 不能有重复ID
 *    - ID必须连续且在1-100之间
 *    - 删除记录困难（只能标记id[i]=0，但其他数组数据残留）
 *    - 内存浪费：即使只有几个学生，也分配了100个位置
 *
 * 4. 后续改进方向（阶段二）：
 *    - 使用结构体(struct)代替并行数组
 *    - 添加输入验证和错误处理
 *    - 将功能拆分为独立函数
 *    - 支持删除和修改记录
 *
 * 5. 安全注意事项：
 *    - 当前代码没有验证ID范围，输入0或>100会导致数组越界
 *    - 姓名输入使用%s，如果输入包含空格会出问题
 *    - 没有检查scanf是否成功读取
 */