/*
 * MiniDB - 迷你数据库系统（阶段三：链表重构 + 结构体设计）
 * 这是一个基于链表的动态学生记录管理系统
 * 功能：添加、查看全部、按ID查找、按姓名模糊搜索、退出
 * 设计目标：帮助C语言新手理解结构体、指针、动态内存管理和链表操作
 */

#include <stdio.h>  // 标准输入输出库，包含printf和scanf函数
#include <string.h>
#include <stdlib.h>
#include "db.h"
#include "utils.h"

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