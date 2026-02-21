# MiniDB（C 语言命令行迷你数据库）

一个用于练习 C 语言结构体、指针、动态内存与链表操作的命令行小项目：在内存中维护“学生记录”，支持添加、查询、模糊搜索与按 ID 删除。

## 功能

- 添加记录（自动分配递增 ID）
- 查看全部记录
- 按 ID 查找
- 按姓名关键字模糊查找（`strstr`）
- 按 ID 删除

程序入口菜单（见 `main.c`）：

1. 添加
2. 查看全部
3. 按 Id 查找
4. 按姓名查找
5. 按 Id 删除
6. 退出

## 目录结构

- `main.c`：交互式菜单与主循环
- `db.c` / `db.h`：链表数据库核心逻辑（`Database`、`Record` 及 CRUD）
- `utils.c` / `utils.h`：输入校验（姓名长度、年龄范围、成绩范围、ID 合法性）
- `config.h`：项目配置（如 `MAX_NAME_LEN`）与 `DEBUG_PRINT` 宏
- `io.c` / `io.h`：I/O 模块占位（当前基本为空）
- `Makefile`：构建脚本（生成 `program.exe`）
- `minidb.md`：更详细的设计/迭代说明文档

## 构建（Windows / MinGW）

当前项目在 Windows 下使用 `mingw32-make` + `gcc` 进行构建，产物为 `program.exe`。

1) 编译：

```powershell
mingw32-make
```

2) 清理：

```powershell
mingw32-make clean
```

说明：

- `Makefile` 里的 `clean` 使用的是 Windows 的 `del` 命令（适用于 `cmd.exe` 语义）。如果你在 MSYS2 的 bash 环境里运行 `make clean`，可能需要把 `del` 改回 `rm -f`。

## 运行

在 PowerShell 中运行：

```powershell
.\program.exe
```

根据菜单提示输入数字并按回车即可。

## 备注 / 已知点

- 当前数据只保存在内存中，程序退出后不会持久化到文件。
- `io.c`/`io.h` 目前是占位模块；若后续要做文件读写/持久化，可以在此处扩展。

