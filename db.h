/*
 * db.h - MiniDB 数据库核心头文件
 * 定义数据结构、枚举和数据库操作接口
 * 阶段六：高级特性 — 位操作、国际化、数学支持
 */

#ifndef DB_H
#define DB_H

#include "config.h"

/*
 * 记录状态标志（位字段）
 * 使用 uint8_t 的低 4 位存储状态
 */
#define FLAG_READONLY  (1 << 0)  // 0x01 只读
#define FLAG_ARCHIVED  (1 << 1)  // 0x02 已归档
#define FLAG_VIP       (1 << 2)  // 0x04 VIP
#define FLAG_DELETED   (1 << 3)  // 0x08 软删除

/*
 * 记录结构体
 * 使用链表存储，每个节点代表一条学生记录
 */
typedef struct Record {
    int id;                     // 学生 ID
    char name[MAX_NAME_LEN];    // 姓名
    int age;                    // 年龄
    double score;               // 成绩
    uint8_t flags;              // 位字段状态（只读/归档/VIP/软删除）
    struct Record *next;        // 指向下一个节点的指针
} Record;

/*
 * 数据库结构体
 * 管理整个链表和状态信息
 */
typedef struct Database {
    Record *head;   // 链表头节点（哨兵节点）
    int count;      // 记录总数
    int next_id;    // 下一个可用的 ID
} Database;

/*
 * 菜单命令枚举
 * 对应主菜单中的选项
 */
typedef enum Command {
    CMD_ADD = 1,        // 添加记录
    CMD_LIST,           // 查看全部记录
    CMD_FIND_ID,        // 按 ID 查找
    CMD_FIND_NAME,      // 按姓名查找
    CMD_DELETE,         // 按 ID 删除
    CMD_SORT,           // 排序记录
    CMD_FILE,           // 文件操作
    CMD_STATS,          // 统计信息
    CMD_FLAG,           // 记录状态管理
    CMD_QUIT            // 退出程序
} Command;

/*
 * 排序字段枚举
 * 用于指定按哪个字段排序
 */
typedef enum SortField {
    SORT_BY_ID = 1,     // 按 ID 排序
    SORT_BY_NAME,       // 按姓名排序
    SORT_BY_AGE,        // 按年龄排序
    SORT_BY_SCORE       // 按成绩排序
} SortField;

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
void db_delete(Database *db, int id);   // 删除指定 ID 的记录
void db_list_all(const Database *db);   // 列出所有记录
void db_find_by_id(const Database *db); // 按 ID 查找记录（交互式）
void db_find_by_name(const Database *db); // 按姓名模糊查找（交互式）

/*
 * 排序操作
 */
void db_sort(Database *db, int field);  // 按指定字段排序

/*
 * 统计操作
 */
void db_stats(const Database *db);  // 输出统计信息

/*
 * 记录状态管理
 */
bool db_toggle_flag(Database *db, int id, uint8_t flag);  // 切换记录标志
void db_show_flags(const Database *db);                   // 显示所有记录的状态

/*
 * 辅助函数
 */
void print_record(const Record *record);  // 打印单条记录
void print_record_verbose(const Record *record);  // 打印单条记录（含状态）

#endif /* DB_H */
