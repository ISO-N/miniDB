/*
 * MiniDB - 迷你数据库系统（阶段二：函数化重构）
 * 这是一个基于数组的简单学生记录管理系统
 * 功能：添加、查看全部、按ID查找、按姓名模糊搜索、退出
 * 设计目标：帮助C语言新手理解函数、参数传递、字符串处理和输入验证
 */

#include <stdio.h>  // 标准输入输出库，包含printf和scanf函数
#include <string.h>

// 定义常量：最大记录数
#define MAXN 100    // 最多存储100条学生记录

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
// 函数原型声明（带参数）
void add_record(int id[], char name[][MAXN+1], int age[], double grade[]);
void list_record(int id[], char name[][MAXN+1], int age[], double grade[]);
void search_id(int id[], char name[][MAXN+1], int age[], double grade[]);
void find_by_name(int id[], char name[][MAXN+1], int age[], double grade[]);

// 输入验证函数原型
int validate_id(int id_num, int id_array[]);
int validate_name(const char *name);
int validate_age(int age);
int validate_score(double score);

/*
 * main函数 - 程序入口点
 * 返回值：0表示成功执行，非0表示错误
 */
int main(void) {

    /*
     * 数据结构定义（局部变量）
     * 使用四个并行数组来存储学生信息
     */
    int id[MAXN + 1] = {0};                 // 标记位数组：0=空位，1=有记录
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
        printf("3.按id查找    4.按姓名查找\n");
        printf("5.退出\n");
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
                add_record(id, name, age, grade);
                break;  // 跳出switch，回到循环开始
            }

            /* 情况2：查看所有学生记录 */
            case 2: {
                list_record(id, name, age, grade);
                break;
            }

            /* 情况3：按ID查找学生 */
            case 3: {
                search_id(id, name, age, grade);
                break;
            }

            case 4: {
                find_by_name(id, name, age, grade);
                break;
            }

            /* 情况5：退出程序 */
            case 5: {
                printf("程序退出！感谢使用MiniDB。\n");
                return 0;  // 直接退出main函数，结束程序
            }

            /* 默认情况：处理无效输入 */
            default: {
                printf("错误：请输入1-5之间的数字！\n");
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

void add_record(int id[], char name[][MAXN+1], int age[], double grade[]){
    int tmp_id;
    char tmp_name[MAXN+1];
    int tmp_age;
    double tmp_score;

    // 输入并验证ID
    while (1) {
        printf("请输入学生ID（1-%d）：\n", MAXN);
        scanf("%d", &tmp_id);
        if (validate_id(tmp_id, id)) {
            break; // ID有效
        }
        printf("请重新输入。\n");
    }

    // 输入并验证姓名
    while (1) {
        printf("请输入学生姓名：\n");
        scanf("%s", tmp_name); // 注意：假设输入不包含空格
        if (validate_name(tmp_name)) {
            break; // 姓名有效
        }
        printf("请重新输入。\n");
    }

    // 输入并验证年龄
    while (1) {
        printf("请输入学生年龄：\n");
        scanf("%d", &tmp_age);
        if (validate_age(tmp_age)) {
            break; // 年龄有效
        }
        printf("请重新输入。\n");
    }

    // 输入并验证成绩
    while (1) {
        printf("请输入学生成绩：\n");
        scanf("%lf", &tmp_score);
        if (validate_score(tmp_score)) {
            break; // 成绩有效
        }
        printf("请重新输入。\n");
    }

    // 所有验证通过，保存数据
    id[tmp_id] = 1;  // 标记该位置有记录
    strcpy(name[tmp_id], tmp_name); // 复制姓名
    age[tmp_id] = tmp_age;
    grade[tmp_id] = tmp_score;

    printf("记录完成！学生ID：%d\n", tmp_id);
}

void list_record(int id[], char name[][MAXN+1], int age[], double grade[]){
    printf("=== 所有学生记录 ===\n");

    int found = 0;

    // 遍历所有可能的ID（1到MAXN）
    for (int i = 1; i <= MAXN; i++) {
    // 检查该ID位置是否有记录
    if (id[i] == 1) {
        found = 1;
        printf("-----------------\n");
        printf("学生ID：%d\n", i);
        printf("姓名：%s\n", name[i]);
        printf("年龄：%d岁\n", age[i]);
        printf("成绩：%.2f分\n", grade[i]);  // %.2f保留两位小数
        }
    }

    if (!found) {  // 简单检查是否有任何记录（不完善）
        printf("暂无学生记录。\n");
    }
}

void search_id(int id[], char name[][MAXN+1], int age[], double grade[]){
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
}

void find_by_name(int id[], char name[][MAXN+1], int age[], double grade[]){
    printf("请输入要查找的姓名或部分姓名：\n");
    char keyword[MAXN+1];
    scanf("%s", keyword);  // 读取搜索关键字

    int found = 0; // 记录是否找到匹配

    // 遍历所有记录（ID从1到MAXN）
    for(int i = 1; i <= MAXN; i++){
        if(id[i] == 1){ // 该位置有记录
            // 使用strstr进行模糊搜索（子串匹配）
            if(strstr(name[i], keyword) != NULL){
                if(found == 0){
                    printf("=== 找到以下匹配的学生 ===\n");
                }
                printf("-----------------\n");
                printf("学生ID：%d\n", i);
                printf("姓名：%s\n", name[i]);
                printf("年龄：%d岁\n", age[i]);
                printf("成绩：%.2f分\n", grade[i]);
                found = 1;
            }
        }
    }

    if(found == 0){
        printf("未找到包含\"%s\"的学生记录。\n", keyword);
    }
}

// 输入验证函数实现

// 验证ID：检查范围(1-MAXN)和是否已被占用
int validate_id(int id_num, int id_array[]) {
    if (id_num < 1 || id_num > MAXN) {
        printf("错误：ID必须在1到%d之间！\n", MAXN);
        return 0; // 验证失败
    }
    if (id_array[id_num] == 1) {
        printf("错误：ID %d 已被占用！\n", id_num);
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
    if (strlen(name) > MAXN) {
        printf("错误：姓名长度不能超过%d个字符！\n", MAXN);
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
 * 4. 阶段二已完成的功能：
 *    - 所有功能拆分为独立函数（带参数传递）
 *    - 添加输入验证：ID范围、姓名长度、年龄、成绩
 *    - 支持按姓名模糊搜索（strstr）
 *    - 函数参数传递练习
 *
 * 5. 下一阶段（阶段三）改进方向：
 *    - 使用链表和动态内存替代固定数组
 *    - 支持动态添加/删除记录
 *    - 掌握指针和内存管理
 *
 * 6. 当前安全注意事项：
 *    - 已添加ID范围验证，防止数组越界
 *    - 姓名输入使用%s，如果输入包含空格会出问题（仍需改进）
 *    - 部分scanf调用仍缺少错误检查
 */