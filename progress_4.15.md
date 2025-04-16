# 从零开始构建一个最小 RISC-V 内核并实现 "Hello World" 输出

## 一、环境准备

### 1. 安装依赖工具链
```bash
# 在 Ubuntu/Debian 上安装所需工具
sudo apt update
sudo apt install gcc-riscv64-unknown-elf
sudo apt install qemu-system-riscv64
```

### 2. 验证安装
```bash
qemu-system-riscv64 --version  # 确认 QEMU 版本 ≥ 5.0
riscv64-unknown-elf-gcc --version  # 确认 GCC 工具链存在
```

## 二、项目结构

创建以下目录和文件：
```plaintext
my_os/
├── kernel/
│   ├── boot.S       # 启动汇编代码
│   └── main.c       # C 语言入口
├── drivers/
│   └── uart.c       # UART 串口驱动
├── scripts/
│   └── linker.ld    # 链接脚本
└── Makefile         # 构建脚本
```

## 三、代码实现

### 1. 启动汇编代码 (`kernel/boot.S`)
```assembly
.section .text.init
.global _start
_start:
    la sp, _stack_end  # 设置栈指针（栈从高地址向低地址增长）
    call main           # 跳转到 C 语言入口 main 函数
    j .                 # 无限循环（防止 main 返回）

# 栈空间定义（4KB）
.section .bss
.align 4
_stack_start:
    .space 4096
_stack_end:
```

### 2. UART 驱动 (`drivers/uart.c`)
```c
// QEMU virt 机器的 UART 地址
#define UART_BASE 0x10000000

// 发送一个字符
void uart_putc(char c) {
    volatile unsigned int *uart = (unsigned int *)UART_BASE;
    while (*uart & 0x80000000);  // 等待发送缓冲区空闲
    *uart = c;
}

// 发送字符串
void uart_puts(const char *str) {
    while (*str) {
        uart_putc(*str++);
    }
}
```

### 3. C 语言入口 (`kernel/main.c`)
```c
// 声明外部函数
extern void uart_puts(const char *str);

// 内核入口
void main() {
    uart_puts("Hello, RISC-V World!\n");
    while (1);  // 无限循环
}
```

### 4. 链接脚本 (`scripts/linker.ld`)
```ld
ENTRY(_start)

SECTIONS {
    . = 0x80000000;  /* QEMU virt 内存起始地址 */

    .text : {
        *(.text.init)  /* 启动代码 */
        *(.text .text.*)  /* 所有代码段 */
        *(.rodata .rodata.*)  /* 只读数据紧接代码段 */
    }

    .data : { *(.data*) }
    .bss : { *(.bss*) }

    /* 栈空间定义 */
    . = ALIGN(16);
    _stack_start = .;
    . += 4096;
    _stack_end = .;
}
```

## 四、编译与链接

### 1. Makefile 编写
```makefile
CC = riscv64-unknown-elf-gcc
CFLAGS = -march=rv64gc -mabi=lp64 -nostdlib -ffreestanding -Iinclude -mcmodel=medany
LD = riscv64-unknown-elf-ld
LDFLAGS = -T scripts/linker.ld

OBJS = kernel/boot.o kernel/main.o drivers/uart.o

.PHONY: all clean run

all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) kernel.elf

run: kernel.elf
	qemu-system-riscv64 -machine virt -nographic -bios none -kernel kernel.elf
```

### 2. 编译与运行
```bash
cd my_os
make        # 编译生成 kernel.elf
make run    # 启动 QEMU
```

**预期输出**：  
终端显示 `Hello, RISC-V World!` 并挂起（因内核进入无限循环）。

![image-20250415223208126](C:\Users\leglon\AppData\Roaming\Typora\typora-user-images\image-20250415223208126.png)

## 五、调试技巧

### 1. 使用 GDB 调试
```bash
# 终端1：启动 QEMU 调试服务器
qemu-system-riscv64 -machine virt -nographic -bios none -kernel kernel.elf -S -s

# 终端2：连接 GDB
gdb-multiarch kernel.elf -ex "target remote :1234"
```
在 GDB 中可设置断点（如 `b main`）、单步执行（`si`/`ni`）、查看寄存器（`info registers`）等。

### 2. 查看反汇编
```bash
riscv64-unknown-elf-objdump -D kernel.elf > disassembly.txt
```

## 六、核心原理说明

1. **启动流程**  
   - QEMU 虚拟机的 CPU 从 `0x80000000` 地址开始执行代码。
   - `boot.S` 设置栈指针后跳转到 C 语言的 `main` 函数。

2. **UART 通信**  
   - QEMU 虚拟机的 UART 设备映射到 `0x10000000` 地址。
   - 向该地址写入字符数据即可通过 QEMU 虚拟串口输出。

3. **内存布局**  
   - 链接脚本 (`linker.ld`) 确保代码被加载到正确地址。
   - `.text` 段起始于 `0x80000000`，栈空间位于 `.bss` 段末尾。

## 七、常见问题解决

1. **无输出或乱码**  
   - 检查 UART 基地址是否为 `0x10000000`。
   - 确认编译时指定了 `-march=rv64gc`（启用所有标准扩展）。

2. **链接错误**  
   - 确保链接脚本中的入口符号 `_start` 与汇编代码中的全局符号一致。

3. **QEMU 无法启动**  
   - 使用 `-nographic` 参数禁用图形界面。
   - 确认 `-bios none` 参数未加载外部 Bootloader。

## 八、扩展建议

通过这个最小内核，你已经掌握了 RISC-V 操作系统开发的 **启动流程、交叉编译、设备驱动** 等核心技能。下一步可以扩展以下功能：
- 实现中断处理（如时钟中断）
- 添加内存分页管理
- 支持多核启动
- 构建用户态程序加载机制

