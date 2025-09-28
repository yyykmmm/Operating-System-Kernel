# RISC-V OS 锁机制实验 README

这是一个基于 RISC-V 体系构建的内核锁机制实验项目，通过自旋锁实现任务同步与临界区保护。

## 目录结构

```
os/
├── Makefile
├── lock.c          // 锁机制实现
├── user.c          // 任务创建和关键区操作
...
```

## 一、环境配置

请先确保你已安装以下环境：

* Ubuntu Linux 或其他 Linux 可执行环境
* QEMU (riscv32)
* RISC-V GNU Toolchain (riscv64-unknown-elf)

安装示例：

```bash
sudo apt update
sudo apt install qemu-system-misc qemu-system-riscv gcc-riscv64-unknown-elf
```

## 二、编译项目

进入实验目录，执行编译命令：

```bash
make
```

如果你看到 `kernel.elf` 创建成功，则表示编译成功。

## 三、运行实验

执行以下命令：

```bash
make run
```

如果想结束 QEMU 模拟器：

1. 按 `Ctrl + A`
2. 释放，再按 `X`

## 四、切换是否使用锁

默认情况为使用锁，即 `user.c` 中有如下定义：

```c
#define USE_LOCK
```

### 如果不使用锁：

1. 打开 `user.c`
2. 注释掉 `#define USE_LOCK`
3. 重新编译：

```bash
make clean
make
```

4. 重运行：

```bash
make run
```

即可观察任务执行被打断的输出效果

## 五、输出示例

### 使用锁

```
Task 0: Begin ...
Task 0: Running...
...
Task 0: End ...
```

### 不使用锁

```
Task 0: Begin ...
Task 0: Running...
Task 1: Running...    ※ 任务切换打断关键区
```

## 六、常见问题

### QEMU 无法启动，报出 libncursesw\.so 相关错误：

解决办法：

```bash
sudo apt install libncurses5 libncursesw5 libnettle6
```

或更新系统中的 QEMU 版本：

```bash
sudo apt install qemu-system-riscv
```
