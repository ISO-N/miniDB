/*
 * MiniDB - 迷你数据库系统（阶段三：链表重构 + 结构体设计）
 * 这是一个基于链表的动态学生记录管理系统
 * 功能：添加、查看全部、按ID查找、按姓名模糊搜索、退出
 * 设计目标：帮助C语言新手理解结构体、指针、动态内存管理和链表操作
 */

#include <stdio.h>  // 标准输入输出库，包含printf和scanf函数
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LEN 64  // 姓名最大长度
typedef struct Record{
    int id;
    char name[MAX_NAME_LEN];
    int age;
    double score;
    struct Record *next;
} Record;

typedef struct Database{
    Record *head;
    int count;
    int next_id;
} Database;

typedef enum Command{
    CMD_ADD = 1,
    CMD_LIST ,
    CMD_FIND_ID ,
    CMD_FIND_NAME ,
    CMD_DELETE ,
    CMD_QUIT
}Command;

/*
 * 数据结构说明：
 * 使用链表结构动态存储学生信息，每个节点包含完整的学生记录
 * Record结构体：包含id、姓名、年龄、成绩和指向下一个节点的指针
 * Database结构体：管理链表头节点、记录总数和下一个可用的ID
 *
 * 优势：
 * - 动态内存分配，不限制记录数量
 * - 支持高效的插入操作
 * - 无需预先分配固定大小的数组
 */
// 函数原型声明（带参数）
Database *db_create(void);
void db_add(Database *db);
void db_list_all(const Database *db);
void db_find_by_id(const Database *db);
void db_find_by_name(const Database *db);
void db_destroy(Database *db);
void db_delete(Database *db,int id);

// 辅助函数原型
void print_record(const Record *record);

// 输入验证函数原型
int validate_id_range(int id_num);  // 检查ID是否为正数
int validate_name(const char *name);
int validate_age(int age);
int validate_score(double score);

/*
 * main函数 - 程序入口点
 * 返回值：0表示成功执行，非0表示错误
 */
int main(void) {

    Database *db = db_create();
    if (db == NULL) {
        printf("错误：无法创建数据库！\n");
        return 1;  // 返回非零表示错误
    }

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
        printf("5.按id删除    6.退出\n");
        printf("---------------\n");

        // 获取用户输入
        // 注意：这里没有错误处理，如果输入非数字会导致问题
        scanf("%d", &choice);

        /*
         * switch语句：根据用户选择执行不同操作
         * 相当于多个if-else的简洁写法
         */
        switch ((Command)choice) {

            /* 情况1：添加新学生记录 */
            case CMD_ADD: {
                db_add(db);
                break;  // 跳出switch，回到循环开始
            }

            /* 情况2：查看所有学生记录 */
            case CMD_LIST: {
                db_list_all(db);
                break;
            }

            /* 情况3：按ID查找学生 */
            case CMD_FIND_ID: {
                db_find_by_id(db);
                break;
            }

            case CMD_FIND_NAME: {
                db_find_by_name(db);
                break;
            }

            case CMD_DELETE: {
                printf("请输入你想删除的id：\n");
                int id;
                scanf("%d",&id);
                db_delete(db,id);
                break;
            }

            /* 情况5：退出程序 */
            case CMD_QUIT: {
                db_destroy(db);
                printf("程序退出！感谢使用MiniDB。\n");
                return 0;  // 直接退出main函数，结束程序
            }

            /* 默认情况：处理无效输入 */
            default: {
                printf("错误：请输入1-6之间的数字！\n");
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

Database *db_create(void){
    Database *db;
    db = malloc(sizeof(struct Database));
    if(db==NULL){
        printf("内存分配失败！\n");
        return NULL;
    }
    // 创建头节点（不存储实际数据）
    db->head = malloc(sizeof(Record));
    if (db->head == NULL) {
        printf("内存分配失败！\n");
        free(db);
        return NULL;
    }
    db->head->next = NULL;
    // 头节点的数据字段可初始化为默认值（可选）
    db->head->id = 0;
    db->head->name[0] = '\0';
    db->head->age = 0;
    db->head->score = 0.0;
    db->count = 0;
    db->next_id = 1;
    return db;
}

void db_destroy(Database *db)
{
    Record *p = db->head;
    Record *q;
    while(p!=NULL){
        q=p;
        p=p->next;
        free(q);
    }
    free(db);
}

void db_add(Database *db)
{
    Record *new_record = malloc(sizeof(Record));
    if (new_record == NULL) {
        printf("内存分配失败！\n");
        return;
    }

    new_record->id = db->next_id;

    // 输入并验证姓名
    while (1) {
        printf("请输入学生姓名：\n");
        scanf("%63s", new_record->name); // 注意：假设输入不包含空格
        if (validate_name(new_record->name)) {
            break; // 姓名有效
        }
        printf("请重新输入。\n");
    }

    // 输入并验证年龄
    while (1) {
        printf("请输入学生年龄：\n");
        scanf("%d", &new_record->age);
        if (validate_age(new_record->age)) {
            break; // 年龄有效
        }
        printf("请重新输入。\n");
    }

    // 输入并验证成绩
    while (1) {
        printf("请输入学生成绩：\n");
        scanf("%lf", &new_record->score);
        if (validate_score(new_record->score)) {
            break; // 成绩有效
        }
        printf("请重新输入。\n");
    }

    // 头插法：新节点插入到头节点之后
    new_record->next = db->head->next;
    db->head->next = new_record;

    db->count++;
    db->next_id++;  // 为下一条记录准备ID

    printf("记录完成！学生ID：%d\n", new_record->id);
}

void db_delete(Database *db,int id){
    // 检查空链表
    if (db->head == NULL || db->head->next == NULL) {
        printf("删除失败：数据库为空！\n");
        return;
    }

    // 验证ID有效性
    if (!validate_id_range(id)) {
        return;
    }

    Record *prev = db->head;      // 前驱节点，初始为头节点
    Record *curr = db->head->next; // 当前节点，从第一个数据节点开始

    // 遍历查找匹配的节点
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }

    // 未找到匹配节点
    if (curr == NULL) {
        printf("删除失败：未找到ID为%d的记录！\n", id);
        return;
    }

    // 找到匹配节点，执行删除
    prev->next = curr->next;
    free(curr);
    db->count--;

    printf("删除成功！已删除ID为%d的记录。\n", id);
}

void db_list_all(const Database *db){
    // 检查空链表：头节点的next为NULL表示没有数据节点
    if (db->head == NULL || db->head->next == NULL) {
        printf("暂无学生记录。\n");
        return;
    }

    printf("=== 所有学生记录 ===\n");
    Record *p = db->head->next;
    while(p != NULL){
        print_record(p);
        p = p->next;
    }
}

void db_find_by_id(const Database *db){
    // 检查空链表：头节点的next为NULL表示没有数据节点
    if (db->head == NULL || db->head->next == NULL) {
        printf("暂无学生记录。\n");
        return;
    }

    int target_id;
    printf("请输入你要查找的学生ID：\n");
    scanf("%d", &target_id);

    // 基本验证：ID必须为正数
    if (!validate_id_range(target_id)) {
        return;
    }

    // 遍历链表查找匹配的ID（从头节点之后开始）
    Record *p = db->head->next;
    while(p != NULL){
        if(p->id == target_id){
            printf("=== 学生信息 ===\n");
            print_record(p);
            return;
        }
        p = p->next;
    }
    printf("学生不存在！\n");
}

void db_find_by_name(const Database *db)
{
    // 检查空链表：头节点的next为NULL表示没有数据节点
    if (db->head == NULL || db->head->next == NULL) {
        printf("暂无学生记录。\n");
        return;
    }

    printf("请输入要查找的姓名或部分姓名：\n");
    char keyword[MAX_NAME_LEN+1];
    scanf("%s", keyword);  // 读取搜索关键字

    int found = 0;

    Record *p = db->head->next;
    while(p != NULL){
        if(strstr(p->name, keyword) != NULL){
            if(found == 0){
                printf("=== 找到以下匹配的学生 ===\n");
            }
            print_record(p);
            found = 1;
        }
        p = p->next;
    }
    if(found == 0){
        printf("未找到包含\"%s\"的学生记录。\n", keyword);
    }
}

// 辅助函数实现

/*
 * print_record - 打印单个学生记录
 * 参数：id - 学生ID
 *       name - 学生姓名
 *       age - 学生年龄
 *       grade - 学生成绩
 */
void print_record(const Record *record) {
    printf("-----------------\n");
    printf("学生ID：%d\n", record->id);
    printf("姓名：%s\n", record->name);
    printf("年龄：%d岁\n", record->age);
    printf("成绩：%.2f分\n", record->score);
}

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


/*
 * 代码设计说明（给新手的学习提示）：
 *
 * 1. 链表数据结构：
 *    - Record结构体：包含学生数据和指向下一个节点的指针
 *    - Database结构体：管理链表头节点、记录计数和下一个ID
 *    - 使用头插法插入新节点，时间复杂度O(1)
 *
 * 2. 动态内存管理：
 *    - 使用malloc动态分配每个Record节点
 *    - 使用free在程序退出时释放所有内存
 *    - db_create创建数据库，db_destroy清理所有资源
 *
 * 3. 阶段三已完成的功能：
 *    - 使用链表替代固定数组，支持无限记录（受内存限制）
 *    - 实现动态添加记录（db_add）
 *    - 实现遍历和搜索功能（db_list_all, db_find_by_id, db_find_by_name）
 *    - 完整的结构体设计，为阶段四的多文件工程做准备
 *
 * 4. 当前设计优势：
 *    - 无记录数量限制（动态内存分配）
 *    - 支持高效的插入操作（头插法）
 *    - 内存使用与记录数量成正比
 *
 * 5. 待改进/下一阶段（阶段四）方向：
 *    - 支持删除记录功能
 *    - 拆分为多文件工程（db.h/db.c, utils.h/utils.c等）
 *    - 添加枚举类型定义菜单命令和状态
 *    - 实现文件持久化保存/加载
 *
 * 6. 当前安全注意事项：
 *    - 已添加内存分配失败检查
 *    - 输入验证保留但部分验证函数可能不再适用
 *    - 姓名输入使用%s，如果输入包含空格会出问题（设计限制）
 *    - 部分scanf调用仍缺少错误检查
 */