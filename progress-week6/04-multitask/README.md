# 04-multitask

## 项目介绍
本项目为 RISC-V 操作系统编写模块 —— **协作式多任务模块**，通过实现多任务的创建、调度和切换功能，深入了解操作系统内核中任务管理的基本机制。该项目的目标是成功创建多个任务，并实现任务之间的切换，在 QEMU 模拟器中运行，输出任务切换信息。

---

## 环境配置

- **硬件平台**：QEMU RISC-V 32位模拟器
- **操作系统**：Ubuntu 20.04 LTS
- **编译器**：riscv64-unknown-elf-gcc （RV32G 指令集）
- **模拟器**：QEMU 6.2.0
- **代码仓库**：`04-multitask`

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
Task 1: Created!
Task 0: Running...
Task 1: Running...
Task 0: Running...
Task 1: Running...
...
```

这表明：
- 内核成功完成了多任务创建。
- 多个任务能够按调度策略正常切换和运行。

---

## 实验原理

### 任务创建

通过 `task_create()` 函数创建任务，为每个任务分配独立的栈空间和上下文结构体：
```c
task_create(user_task0);
task_create(user_task1);
```

### 任务调度

采用简单的循环调度算法，按顺序切换任务：
```c
void schedule()
{
    if (_top <= 0) {
        panic("Num of task should be greater than zero!");
        return;
    }

    _current = (_current + 1) % _top; // 选择下一个任务
    struct context *next = &(ctx_tasks[_current]);
    switch_to(next); // 切换到下一个任务
}
```

### 上下文切换

通过汇编实现的 `switch_to()` 函数保存当前任务上下文并恢复新任务上下文：
```assembly
switch_to:
    csrrw   t6, mscratch, t6    # 交换 t6 和 mscratch 的值
    beqz    t6, 1f              # 如果是首次调用（mscratch 为 0），跳转到 1f

    # 保存前一个任务上下文
    reg_save t6

    # 保存特殊寄存器 t6 到上下文
    mv      t5, t6              # t5 指向当前任务上下文
    csrr    t6, mscratch        # 从 mscratch 读取值
    STORE   t6, 30*SIZE_REG(t5) # 保存 t6 到上下文

1:
    # 切换到新任务上下文
    csrw    mscratch, a0        # 更新 mscratch 为新任务上下文指针

    # 恢复新任务的通用寄存器
    mv      t6, a0
    reg_restore t6

    # 恢复程序计数器，开始执行新任务
    ret
```

---

## 注意事项

- 确保使用的是 **riscv64-unknown-elf-gcc**，并指定 `-march=rv32g` 和 `-mabi=ilp32`。
- 如果运行时 QEMU 报缺少某些库（如 `libncursesw.so.5`），需根据提示手动修复或软链接。
- 本实验为 RV32 架构，注意 QEMU 选项 `-machine virt` 和 `-nographic`。
- 在任务中避免使用过大的局部变量，防止栈溢出。
- 如果需要调试，可以在 `uart_puts` 中添加更多调试信息。

