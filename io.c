/*
 * io.c - MiniDB 文件 I/O 实现
 * 阶段五：文件 I/O + 排序 + 错误处理
 */

#include "io.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 用于自动保存的全局指针 */
static Database *auto_save_db = NULL;

/*
 * io_save_binary - 保存数据库到二进制文件
 * 参数：db - 数据库指针
 *       filename - 文件名
 * 返回值：0 表示成功，-1 表示失败
 *
 * 文件格式：
 * [count(4 字节)][next_id(4 字节)][记录 1][记录 2]...
 * 每条记录：[id(4 字节)][name(64 字节)][age(4 字节)][score(8 字节)]
 */
int io_save_binary(const Database *db, const char *filename) {
    if (db == NULL || filename == NULL) {
        fprintf(stderr, "错误：参数为空！\n");
        return -1;
    }

    /* 检查数据库是否有效 */
    if (db->head == NULL) {
        fprintf(stderr, "错误：数据库未初始化！\n");
        return -1;
    }

    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        fprintf(stderr, "错误：无法打开文件 '%s' 进行写入！\n", filename);
        perror("fopen");
        return -1;
    }

    /* 写入数据库元数据 */
    if (fwrite(&db->count, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "错误：写入 count 失败！\n");
        fclose(fp);
        return -1;
    }
    if (fwrite(&db->next_id, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "错误：写入 next_id 失败！\n");
        fclose(fp);
        return -1;
    }

    /* 遍历链表，写入每条记录 */
    Record *p = db->head->next;
    while (p != NULL) {
        /* 写入记录数据（不保存 next 指针） */
        if (fwrite(&p->id, sizeof(int), 1, fp) != 1 ||
            fwrite(p->name, sizeof(char), MAX_NAME_LEN, fp) != (size_t)MAX_NAME_LEN ||
            fwrite(&p->age, sizeof(int), 1, fp) != 1 ||
            fwrite(&p->score, sizeof(double), 1, fp) != 1) {
            fprintf(stderr, "错误：写入记录失败！\n");
            fclose(fp);
            return -1;
        }
        p = p->next;
    }

    fclose(fp);
    printf("成功保存 %d 条记录到 '%s'\n", db->count, filename);
    return 0;
}

/*
 * io_load_binary - 从二进制文件加载数据库
 * 参数：db - 数据库指针
 *       filename - 文件名
 * 返回值：0 表示成功，-1 表示失败
 */
int io_load_binary(Database *db, const char *filename) {
    if (db == NULL || filename == NULL) {
        fprintf(stderr, "错误：参数为空！\n");
        return -1;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "错误：无法打开文件 '%s' 进行读取！\n", filename);
        perror("fopen");
        return -1;
    }

    /* 读取数据库元数据 */
    int count, next_id;
    if (fread(&count, sizeof(int), 1, fp) != 1 ||
        fread(&next_id, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "错误：读取文件头失败！文件可能已损坏。\n");
        fclose(fp);
        return -1;
    }

    /* 清空现有数据（如果有） */
    Record *p = db->head->next;
    while (p != NULL) {
        Record *q = p;
        p = p->next;
        free(q);
    }
    db->head->next = NULL;
    db->count = 0;
    db->next_id = next_id;

    /* 逐条读取记录 */
    for (int i = 0; i < count; i++) {
        Record *new_record = malloc(sizeof(Record));
        if (new_record == NULL) {
            fprintf(stderr, "错误：内存不足！\n");
            fclose(fp);
            return -1;
        }

        /* 读取记录数据 */
        if (fread(&new_record->id, sizeof(int), 1, fp) != 1 ||
            fread(new_record->name, sizeof(char), MAX_NAME_LEN, fp) != (size_t)MAX_NAME_LEN ||
            fread(&new_record->age, sizeof(int), 1, fp) != 1 ||
            fread(&new_record->score, sizeof(double), 1, fp) != 1) {
            fprintf(stderr, "错误：读取第%d条记录失败！\n", i + 1);
            free(new_record);
            fclose(fp);
            return -1;
        }

        /* 头插法插入链表 */
        new_record->next = db->head->next;
        db->head->next = new_record;
        db->count++;
    }

    fclose(fp);
    printf("成功加载 %d 条记录 from '%s'\n", count, filename);
    return 0;
}

/*
 * io_export_csv - 导出数据库为 CSV 格式
 * 参数：db - 数据库指针
 *       filename - 文件名
 * 返回值：0 表示成功，-1 表示失败
 *
 * CSV 格式：
 * id,name,age,score
 * 1，张三，20,95.5
 */
int io_export_csv(const Database *db, const char *filename) {
    if (db == NULL || filename == NULL) {
        fprintf(stderr, "错误：参数为空！\n");
        return -1;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "错误：无法打开文件 '%s' 进行写入！\n", filename);
        perror("fopen");
        return -1;
    }

    /* 写入 CSV 表头 */
    fprintf(fp, "id,name,age,score\n");

    /* 遍历链表，写入每条记录 */
    Record *p = db->head->next;
    while (p != NULL) {
        fprintf(fp, "%d,%s,%d,%.2f\n", p->id, p->name, p->age, p->score);
        p = p->next;
    }

    fclose(fp);
    printf("成功导出 %d 条记录到 CSV 文件 '%s'\n", db->count, filename);
    return 0;
}

/*
 * io_import_csv - 从 CSV 文件导入数据
 * 参数：db - 数据库指针
 *       filename - 文件名
 * 返回值：0 表示成功，-1 表示失败
 */
int io_import_csv(Database *db, const char *filename) {
    if (db == NULL || filename == NULL) {
        fprintf(stderr, "错误：参数为空！\n");
        return -1;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "错误：无法打开文件 '%s' 进行读取！\n", filename);
        perror("fopen");
        return -1;
    }

    char line[256];
    int imported = 0;
    int line_num = 0;

    /* 逐行读取 CSV 文件 */
    while (fgets(line, sizeof(line), fp) != NULL) {
        line_num++;

        /* 跳过表头（第一行） */
        if (line_num == 1) {
            continue;
        }

        /* 解析 CSV 行 */
        Record *new_record = malloc(sizeof(Record));
        if (new_record == NULL) {
            fprintf(stderr, "错误：内存不足！\n");
            fclose(fp);
            return -1;
        }

        /* 使用 sscanf 解析 CSV 格式 */
        int parsed = sscanf(line, "%d,%63[^,],%d,%lf",
                           &new_record->id, new_record->name,
                           &new_record->age, &new_record->score);

        if (parsed != 4) {
            fprintf(stderr, "警告：第%d行格式错误，跳过。\n", line_num);
            free(new_record);
            continue;
        }

        /* 生成新 ID（使用数据库的 next_id） */
        new_record->id = db->next_id++;

        /* 头插法插入链表 */
        new_record->next = db->head->next;
        db->head->next = new_record;
        db->count++;
        imported++;
    }

    fclose(fp);
    printf("成功从 CSV 文件 '%s' 导入 %d 条记录\n", filename, imported);
    return 0;
}

/*
 * io_set_auto_save_db - 设置自动保存的数据库指针
 */
void io_set_auto_save_db(Database *db) {
    auto_save_db = db;
}

/*
 * io_auto_save - 自动保存函数（用于 atexit 注册）
 */
void io_auto_save(void) {
    if (auto_save_db != NULL && auto_save_db->head != NULL) {
        io_save_binary(auto_save_db, DB_FILENAME);
    }
}
