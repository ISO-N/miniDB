/*
 * db.c - MiniDB 数据库核心实现
 * 阶段五：文件 I/O + 排序 + 错误处理
 */

#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

Database *db_create(void){
    Database *db;
    db = malloc(sizeof(Database));
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
    db->head = NULL;  /* 防止悬空指针 */
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
    db->next_id++;  // 为下一条记录准备 ID

    printf("记录完成！学生 ID：%d\n", new_record->id);
}

void db_delete(Database *db,int id){
    // 检查空链表
    if (db->head == NULL || db->head->next == NULL) {
        printf("删除失败：数据库为空！\n");
        return;
    }

    // 验证 ID 有效性
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
        printf("删除失败：未找到 ID 为%d的记录！\n", id);
        return;
    }

    // 找到匹配节点，执行删除
    prev->next = curr->next;
    free(curr);
    db->count--;

    printf("删除成功！已删除 ID 为%d的记录。\n", id);
}

void db_list_all(const Database *db){
    // 检查空链表：头节点的 next 为 NULL 表示没有数据节点
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
    // 检查空链表：头节点的 next 为 NULL 表示没有数据节点
    if (db->head == NULL || db->head->next == NULL) {
        printf("暂无学生记录。\n");
        return;
    }

    int target_id;
    printf("请输入你要查找的学生 ID：\n");
    scanf("%d", &target_id);

    // 基本验证：ID 必须为正数
    if (!validate_id_range(target_id)) {
        return;
    }

    // 遍历链表查找匹配的 ID（从头节点之后开始）
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
    // 检查空链表：头节点的 next 为 NULL 表示没有数据节点
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

/*
 * print_record - 打印单个学生记录
 */
void print_record(const Record *record) {
    printf("-----------------\n");
    printf("学生 ID：%d\n", record->id);
    printf("姓名：%s\n", record->name);
    printf("年龄：%d 岁\n", record->age);
    printf("成绩：%.2f 分\n", record->score);
}

/*
 * ==================== 排序功能实现 ====================
 * 使用 qsort 对链表进行排序
 * 策略：链表 -> 数组 -> qsort -> 重建链表
 */

/* 比较函数：按 ID 排序 */
static int compare_by_id(const void *a, const void *b) {
    const Record *ra = *(const Record **)a;
    const Record *rb = *(const Record **)b;
    return ra->id - rb->id;
}

/* 比较函数：按姓名排序（字典序） */
static int compare_by_name(const void *a, const void *b) {
    const Record *ra = *(const Record **)a;
    const Record *rb = *(const Record **)b;
    return strcmp(ra->name, rb->name);
}

/* 比较函数：按年龄排序 */
static int compare_by_age(const void *a, const void *b) {
    const Record *ra = *(const Record **)a;
    const Record *rb = *(const Record **)b;
    return ra->age - rb->age;
}

/* 比较函数：按成绩排序 */
static int compare_by_score(const void *a, const void *b) {
    const Record *ra = *(const Record **)a;
    const Record *rb = *(const Record **)b;
    if (ra->score < rb->score) return -1;
    if (ra->score > rb->score) return 1;
    return 0;
}

/*
 * db_sort - 按指定字段排序数据库记录
 * 参数：db - 数据库指针
 *       field - 排序字段（SORT_BY_ID / SORT_BY_NAME / SORT_BY_AGE / SORT_BY_SCORE）
 */
void db_sort(Database *db, int field) {
    if (db == NULL || db->head->next == NULL) {
        printf("数据库为空，无需排序！\n");
        return;
    }

    /* 将链表转为指针数组 */
    Record **records = malloc(sizeof(Record *) * db->count);
    if (records == NULL) {
        printf("错误：内存不足！\n");
        return;
    }

    Record *p = db->head->next;
    for (int i = 0; i < db->count; i++) {
        records[i] = p;
        p = p->next;
    }

    /* 选择比较函数 */
    int (*compare)(const void *, const void *);
    switch (field) {
        case SORT_BY_ID:
            compare = compare_by_id;
            break;
        case SORT_BY_NAME:
            compare = compare_by_name;
            break;
        case SORT_BY_AGE:
            compare = compare_by_age;
            break;
        case SORT_BY_SCORE:
            compare = compare_by_score;
            break;
        default:
            printf("错误：未知的排序字段！\n");
            free(records);
            return;
    }

    /* 使用 qsort 排序 */
    qsort(records, db->count, sizeof(Record *), compare);

    /* 重建链表 */
    for (int i = 0; i < db->count - 1; i++) {
        records[i]->next = records[i + 1];
    }
    records[db->count - 1]->next = NULL;
    db->head->next = records[0];

    free(records);

    printf("排序完成！\n");
}
