/*
 * db.h - MiniDB 数据库核心头文件
 * 定义数据结构、枚举和数据库操作接口
 * 阶段四：多文件工程重构
 */

#ifndef DB_H
#define DB_H

#include "config.h"

/*
 * 记录结构体
 * 使用链表存储，每个节点代表一条学生记录
 */
typedef struct Record {
    int id;                     // 学生ID
    char name[MAX_NAME_LEN];    // 姓名
    int age;                    // 年龄
    double score;               // 成绩
    struct Record *next;        // 指向下一个节点的指针
} Record;

/*
 * 数据库结构体
 * 管理整个链表和状态信息
 */
typedef struct Database {
    Record *head;   // 链表头节点（哨兵节点）
    int count;      // 记录总数
    int next_id;    // 下一个可用的ID
} Database;

/*
 * 菜单命令枚举
 * 对应主菜单中的选项
 */
typedef enum Command {
    CMD_ADD = 1,        // 添加记录
    CMD_LIST,           // 查看全部记录
    CMD_FIND_ID,        // 按ID查找
    CMD_FIND_NAME,      // 按姓名查找
    CMD_DELETE,         // 按ID删除
    CMD_QUIT            // 退出程序
} Command;

/*
 * ==================== 数据库操作接口 ====================
 * 所有函数都通过指针操作数据库，避免全局变量
 */

/*
 * 创建与销毁
 */
Database *db_create(void);               // 创建新数据库
void db_destroy(Database *db);          // 销毁数据库，释放所有内存

/*
 * 增删改查操作
 */
void db_add(Database *db);              // 添加新记录（交互式输入）
void db_delete(Database *db, int id);   // 删除指定ID的记录
void db_list_all(const Database *db);   // 列出所有记录
void db_find_by_id(const Database *db); // 按ID查找记录（交互式）
void db_find_by_name(const Database *db); // 按姓名模糊查找（交互式）

/*
 * 辅助函数
 */
void print_record(const Record *record); // 打印单条记录

/*
 * 注意：输入验证函数（validate_*）将在 utils.h 中声明
 * 文件I/O函数将在 io.h 中声明（阶段五实现）
 */

#endif /* DB_H */