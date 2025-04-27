# 03-contextswitch

## 项目介绍
本项目为 RISC-V 操作系统实验 —— **上下文切换实验**，通过实现基本的上下文切换功能，了解操作系统内核中任务调度和上下文管理的基本机制。实验的目标是成功创建和切换任务，并在 QEMU 模拟器中运行，输出任务切换信息。

---

## 环境配置

- **硬件平台**：QEMU RISC-V 32位模拟器
- **操作系统**：Ubuntu 20.04 LTS
- **编译器**：riscv64-unknown-elf-gcc （RV32G 指令集）
- **模拟器**：QEMU 6.2.0
- **代码仓库**：`riscv-operating-system-mooc/code/os/03-contextswitch`

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
```text
Hello, RVOS!
HEAP_START = 0x8000487c(aligned to 0x80005000), HEAP_SIZE = 0x07ffb784,
num of reserved pages = 8, num of pages to be allocated for heap = 32755
TEXT:   0x80000000 -> 0x80002f70
RODATA: 0x80002f70 -> 0x8000316c
DATA:   0x80004000 -> 0x80004000
BSS:    0x80004000 -> 0x8000487c
HEAP:   0x8000d000 -> 0x88000000
Task 0: Created!
Task 0: Running...
Task 0: Running...
Task 0: Running...
Task 0: Running...
Task 0: Running...
Task 0: Running...
...
```

这表明：
- 内核成功完成了任务创建

- 任务周期性执行正常

  

---

## 注意事项

- 确保使用的是 **riscv64-unknown-elf-gcc**，并指定 `-march=rv32g` 和 `-mabi=ilp32`。
- 如果运行时 QEMU 报缺少某些库（如 `libncursesw.so.5`），需根据提示手动修复或软链接。
- 本实验为 RV32 架构，注意 QEMU 选项 `-machine virt` 和 `-nographic`。