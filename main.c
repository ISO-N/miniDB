/*
 * main.c - MiniDB 主程序入口
 * 阶段五：文件 I/O + 排序 + 错误处理
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "io.h"
#include "utils.h"

/* 全局数据库指针，用于自动保存 */
static Database *g_db = NULL;

/*
 * 显示排序子菜单
 */
static void show_sort_menu(void) {
    printf("\n---------------\n");
    printf("   排序子菜单\n");
    printf("---------------\n");
    printf("1. 按 ID 排序\n");
    printf("2. 按姓名排序\n");
    printf("3. 按年龄排序\n");
    printf("4. 按成绩排序\n");
    printf("0. 返回主菜单\n");
    printf("---------------\n");
}

/*
 * 显示文件操作子菜单
 */
static void show_file_menu(void) {
    printf("\n---------------\n");
    printf("   文件操作菜单\n");
    printf("---------------\n");
    printf("1. 保存到二进制文件 (save)\n");
    printf("2. 从二进制文件加载 (load)\n");
    printf("3. 导出为 CSV (export)\n");
    printf("4. 从 CSV 导入 (import)\n");
    printf("0. 返回主菜单\n");
    printf("---------------\n");
}

/*
 * 处理排序子菜单
 */
static void handle_sort_menu(void) {
    show_sort_menu();
    int sort_choice;
    if (scanf("%d", &sort_choice) != 1) {
        printf("错误：请输入有效的数字！\n");
        clear_input_buffer();
        return;
    }

    if (sort_choice >= 1 && sort_choice <= 4) {
        db_sort(g_db, sort_choice);  /* 直接使用 1-based 索引 */
    } else if (sort_choice == 0) {
        /* 返回主菜单 */
    } else {
        printf("错误：无效的选择！\n");
    }
}

/*
 * 处理文件操作子菜单
 */
static void handle_file_menu(void) {
    show_file_menu();
    int file_choice;
    if (scanf("%d", &file_choice) != 1) {
        printf("错误：请输入有效的数字！\n");
        clear_input_buffer();
        return;
    }

    switch (file_choice) {
        case 1:
            io_save_binary(g_db, DB_FILENAME);
            break;
        case 2:
            io_load_binary(g_db, DB_FILENAME);
            break;
        case 3:
            io_export_csv(g_db, CSV_FILENAME);
            break;
        case 4:
            io_import_csv(g_db, CSV_FILENAME);
            break;
        case 0:
            /* 返回主菜单 */
            break;
        default:
            printf("错误：无效的选择！\n");
            break;
    }
}

int main(void) {
    /* 创建数据库 */
    g_db = db_create();
    if (g_db == NULL) {
        printf("错误：无法创建数据库！\n");
        return 1;
    }

    /* 注册自动保存函数 */
    io_set_auto_save_db(g_db);
    if (atexit(io_auto_save) != 0) {
        fprintf(stderr, "警告：无法注册自动保存函数！\n");
    }

    /* 尝试自动加载上次保存的数据 */
    FILE *test = fopen(DB_FILENAME, "rb");
    if (test != NULL) {
        fclose(test);
        printf("发现已保存的数据文件，正在加载...\n");
        io_load_binary(g_db, DB_FILENAME);
        printf("\n");
    }

    /* 主循环 */
    while (1) {
        int choice;

        /* 显示主菜单 */
        printf("\n=========== MiniDB 学生记录管理系统 ===========\n");
        printf("当前记录数：%d\n", g_db->count);
        printf("-------------------------------------------------\n");
        printf("1. 添加记录    2. 查看全部    3. 按 ID 查找\n");
        printf("4. 按姓名查找  5. 按 ID 删除  6. 排序记录\n");
        printf("7. 文件操作    8. 退出系统\n");
        printf("-------------------------------------------------\n");
        printf("请输入你的选择 (1-8): ");

        /* 带错误处理的输入 */
        if (scanf("%d", &choice) != 1) {
            printf("错误：请输入有效的数字！\n");
            clear_input_buffer();
            continue;
        }

        switch ((Command)choice) {
            case CMD_ADD:
                db_add(g_db);
                break;

            case CMD_LIST:
                db_list_all(g_db);
                break;

            case CMD_FIND_ID:
                db_find_by_id(g_db);
                break;

            case CMD_FIND_NAME:
                db_find_by_name(g_db);
                break;

            case CMD_DELETE: {
                int id;
                printf("请输入要删除的学生 ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("错误：请输入有效的数字！\n");
                    clear_input_buffer();
                } else {
                    db_delete(g_db, id);
                }
                break;
            }

            case CMD_SORT:
                handle_sort_menu();
                break;

            case CMD_FILE:
                handle_file_menu();
                break;

            case CMD_QUIT: {
                printf("感谢使用 MiniDB，再见！\n");
                db_destroy(g_db);
                g_db = NULL;  /* 防止 atexit 再次访问已释放的内存 */
                return 0;
            }

            default:
                printf("错误：请输入 1-8 之间的数字！\n");
                break;
        }
    }

    return 0;
}
