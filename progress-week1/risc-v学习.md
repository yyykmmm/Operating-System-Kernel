# RISC-V 学习历程与操作系统开发实践

## 什么是 RISC-V？

RISC-V 是一个基于精简指令集（RISC）原则的开源指令集架构（ISA）。它具有模块化设计、可扩展性强、免授权费等特点，广泛应用于学术研究、嵌入式系统和定制化芯片设计。RISC-V 的开放性使其成为学习计算机体系结构和操作系统开发的理想平台。

---

## RISC-V 学习核心步骤

### 1. 理解 RISC-V 基础架构
- **指令集分类**：基础整数指令集（RV32I/RV64I）、乘除扩展（M）、原子操作扩展（A）等。
- **寄存器**：32 个通用寄存器（x0-x31），其中 x0 硬编码为 0。
- **ABI 规范**：了解寄存器用途（如 a0-a7 用于参数传递，ra 保存返回地址）。
- **特权架构**：学习机器模式（M-mode）、监督模式（S-mode）和用户模式（U-mode）的区别。

### 2. 搭建开发环境
#### 安装 RISC-V 工具链
使用预编译工具链或从源码编译（以 Ubuntu 为例）：
```bash
sudo apt install gcc-riscv64-unknown-elf gdb-multiarch qemu-system-riscv
```

#### 验证工具链
编写简单汇编程序 `hello.s`：
```assembly
.section .text
.globl _start
_start:
    li a0, 42        # 将立即数 42 加载到寄存器 a0
    addi a1, a0, 10  # a1 = a0 + 10
    j _start          # 循环
```

编译并生成可执行文件：
```bash
riscv64-unknown-elf-gcc -nostdlib -o hello.elf hello.s
```

### 3. 使用 QEMU 模拟运行
#### 启动 RISC-V 虚拟机
```bash
qemu-system-riscv64 -nographic -machine virt -kernel hello.elf -m 128M
```

#### 调试程序
启动 QEMU 并等待 GDB 连接：
```bash
qemu-system-riscv64 -s -S -nographic -machine virt -kernel hello.elf -m 128M
```

在另一终端连接 GDB：
```bash
gdb-multiarch -ex "target remote :1234" -ex "file hello.elf"
```

---

## 实践项目：实现简易内核

### 步骤 1：编写启动代码
创建 `boot.S`，设置栈指针并跳转到主函数：
```assembly
.section .text.init
.globl _start
_start:
    la sp, stack_top  # 初始化栈指针
    call main          # 调用主函数
    j .               # 无限循环

.section .bss
stack:
    .space 4096       # 分配 4KB 栈空间
stack_top:
```

### 步骤 2：实现主函数
编写 `main.c`，通过 MMIO 控制串口输出：
```c
#define UART_BASE 0x10000000

void uart_putc(char c) {
    volatile char *uart = (volatile char *)UART_BASE;
    *uart = c;
}

void main() {
    const char *msg = "Hello RISC-V!\n";
    for (int i = 0; msg[i]; i++) {
        uart_putc(msg[i]);
    }
    while (1);
}
```

### 步骤 3：编译与链接
使用链接脚本 `link.ld` 指定内存布局：
```ld
ENTRY(_start)
MEMORY {
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128K
}

SECTIONS {
    .text : { *(.text.init) *(.text) } > RAM
    .data : { *(.data) } > RAM
    .bss : { *(.bss) } > RAM
}
```

编译命令：
```bash
riscv64-unknown-elf-gcc -nostdlib -T link.ld -o kernel.elf boot.S main.c
```

### 步骤 4：运行与调试
启动 QEMU 并观察串口输出：
```bash
qemu-system-riscv64 -nographic -machine virt -kernel kernel.elf -m 128M
```

---

## 常见问题与解决

1. **QEMU 无法加载内核**  
   - 检查编译目标架构是否匹配（如 `-march=rv64gc`）。
   - 确认链接脚本的入口地址与 QEMU 的启动地址一致（通常为 `0x80000000`）。

2. **GDB 连接失败**  
   - 确保 QEMU 启动时添加 `-s -S` 参数。
   - 使用 `gdb-multiarch` 而非普通 GDB。

3. **串口无输出**  
   - 验证 MMIO 地址是否正确（QEMU Virt 机器的 UART 基地址为 `0x10000000`）。
   - 检查是否启用了正确的串口驱动参数（如 `-append "console=ttyS0"`）。

---

## 学习资源推荐

- **官方文档**：[RISC-V ISA Manual](https://riscv.org/technical/specifications/)
- **实践指南**：《RISC-V 体系结构编程与实践》
- **在线工具**：[Compiler Explorer](https://godbolt.org/)（选择 RISC-V gcc 编译器）

---

## 总结

通过 RISC-V 的学习，我掌握了指令集架构的设计思想、裸机程序开发流程以及 QEMU 调试技巧。下一步计划实现中断处理和内存管理模块，逐步构建一个功能完整的操作系统内核。RISC-V 的开放生态为系统级编程提供了极大的灵活性和实践价值。