# 02-memanagement

## 项目介绍
本项目为 RISC-V 操作系统实验 —— **内存管理实验**，通过实现内存页分配与管理，了解内核堆区的基本结构和页式管理机制。实验的目标是成功初始化堆区并在 QEMU 模拟器中运行，输出内存布局信息，并测试不同的内存分配与回收情况。

---

## 环境配置

- **硬件平台**：QEMU RISC-V 32位模拟器
- **操作系统**：Ubuntu 20.04 LTS
- **编译器**：riscv64-unknown-elf-gcc （RV32G 指令集）
- **模拟器**：QEMU 6.2.0
- **代码仓库**：`riscv-operating-system-mooc/code/os/02-memanagement`

---

## 准备工作

### 安装交叉编译器（riscv64-unknown-elf-gcc）

如果未安装，可参考如下方式安装交叉工具链：
```bash
sudo apt update
sudo apt install gcc-riscv64-unknown-elf
```
或者从 SiFive 官网下载预编译版本并配置环境变量。

### 安装 QEMU

推荐版本 6.2.0，可通过以下命令安装：
```bash
sudo apt install qemu-system-misc
```

---

## 编译与运行

1. 清理旧的编译文件
   ```bash
   make clean
   ```

2. 编译生成 `os.elf` 文件
   ```bash
   make
   ```

3. 启动 QEMU 模拟器运行实验系统
   ```bash
   make run
   ```

运行后，按下 **Ctrl+A**，再按 **X** 可以退出 QEMU。

---

## 实验输出示例

正常运行时，终端输出示例如下：
```
Hello, RVOS!
HEAP_START = 0x800043f4(aligned to 0x80005000), HEAP_SIZE = 0x07ffbc0c,
num of reserved pages = 8, num of pages to be allocated for heap = 32755
TEXT:   0x80000000 -> 0x80003010
RODATA: 0x80003010 -> 0x800033a5
DATA:   0x80004000 -> 0x80004000
BSS:    0x80004000 -> 0x800043f4
HEAP:   0x8000d000 -> 0x88000000
Page management initialized.
```

这表明：
- 内核成功完成了堆区初始化
- 内存段（TEXT、RODATA、DATA、BSS、HEAP）布局正常
- 页管理初始化完成

---

## 测试结果

### Test 1: 分配 1 页
```
Allocating 1 pages
Total pages available: 32755
Allocated address: 8000D000
Freed 1 page.
```

### Test 2: 分配 3 页
```
Allocating 3 pages
Total pages available: 32755
Allocated address: 8000D000
Freed 3 pages.
```

### Test 3: 分配过多页
```
Allocating 100000 pages
Total pages available: 32755
Not enough pages available.
Allocation failed as expected (NULL).
```

### Test 4: 多次分配与释放
```
Allocating 2 pages
Total pages available: 32755
Allocated 2 pages at: 8000D000
Allocating 2 pages
Total pages available: 32755
Allocated 2 pages at: 8000F000
Allocating 2 pages
Total pages available: 32755
Allocated 2 pages at: 80011000
Allocating 2 pages
Total pages available: 32755
Allocated 2 pages at: 80013000
Allocating 2 pages
Total pages available: 32755
Allocated 2 pages at: 80015000
Freed 2 pages at: 8000D000
Freed 2 pages at: 8000F000
Freed 2 pages at: 80011000
Freed 2 pages at: 80013000
Freed 2 pages at: 80015000

All tests completed.
```

所有测试都成功通过，验证了内存分配、回收以及错误处理功能。

---

## 注意事项

- 确保使用的是 **riscv64-unknown-elf-gcc**，并指定 `-march=rv32g` 和 `-mabi=ilp32`。
- 如果运行时 QEMU 报缺少某些库（如 `libncursesw.so.5`），需根据提示手动修复或软链接。
- 本实验为 RV32 架构，注意 QEMU 选项 `-machine virt` 和 `-nographic`。
