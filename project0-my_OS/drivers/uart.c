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
