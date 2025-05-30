// 声明外部函数
extern void uart_puts(const char *str);

// 内核入口
void main() {
    uart_puts("Hello, RISC-V World!\n");
    while (1);  // 无限循环
}
