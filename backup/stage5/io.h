/*
 * io.h - MiniDB 文件 I/O 头文件
 * 阶段五：文件 I/O + 排序 + 错误处理
 */

#ifndef IO_H
#define IO_H

#include "db.h"

/*
 * ==================== 文件 I/O 接口 ====================
 */

/*
 * 二进制文件操作
 * 用于快速保存和加载整个数据库
 */
int io_save_binary(const Database *db, const char *filename);   // 保存数据库到二进制文件
int io_load_binary(Database *db, const char *filename);         // 从二进制文件加载数据库

/*
 * CSV 文件操作
 * 用于与其他程序交换数据
 */
int io_export_csv(const Database *db, const char *filename);    // 导出为 CSV 格式
int io_import_csv(Database *db, const char *filename);          // 从 CSV 导入数据

/*
 * 自动保存（用于 atexit 注册）
 */
void io_auto_save(void);        // 注册自动保存，退出时调用

/*
 * 设置自动保存的数据库指针
 */
void io_set_auto_save_db(Database *db);

#endif /* IO_H */
