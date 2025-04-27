#include "os.h"

/*
 * Following functions SHOULD be called ONLY ONE time here,
 * so just declared here ONCE and NOT included in file os.h.
 */
extern void uart_init(void);
extern void page_init(void);

extern void *page_alloc(int npages);
extern void page_free(void *p);
extern void uart_hex(unsigned long d); // 补充 uart_hex 声明

void start_kernel(void)
{
    uart_init();
    uart_puts("Hello, RVOS!\n");

    page_init();
    uart_puts("Page management initialized.\n");

    // 测试 1：分配 1 页，释放
    uart_puts("\n[Test 1] Alloc 1 page:\n");
    void *p1 = page_alloc(1);
    uart_puts("Allocated address: ");
    uart_hex((unsigned long)p1);
    uart_puts("\n");
    page_free(p1);
    uart_puts("Freed 1 page.\n");

    // 测试 2：连续分配 3 页，释放
    uart_puts("\n[Test 2] Alloc 3 pages:\n");
    void *p2 = page_alloc(3);
    uart_puts("Allocated address: ");
    uart_hex((unsigned long)p2);
    uart_puts("\n");
    page_free(p2);
    uart_puts("Freed 3 pages.\n");

    // 测试 3：连续分配超过空闲页数，返回 NULL
    uart_puts("\n[Test 3] Alloc too many pages:\n");
    void *p3 = page_alloc(100000); // 超量申请
    if (p3 == NULL) {
        uart_puts("Allocation failed as expected (NULL).\n");
    } else {
        uart_puts("Unexpected allocation! Address: ");
        uart_hex((unsigned long)p3);
        uart_puts("\n");
    }

    // 测试 4：连续分配+释放若干次
    uart_puts("\n[Test 4] Alloc and free multiple times:\n");

    void *ptrs[5];
    for (int i = 0; i < 5; i++) {
        ptrs[i] = page_alloc(2); // 每次分配2页
        uart_puts("Allocated 2 pages at: ");
        uart_hex((unsigned long)ptrs[i]);
        uart_puts("\n");
    }

    for (int i = 0; i < 5; i++) {
        page_free(ptrs[i]);
        uart_puts("Freed 2 pages at: ");
        uart_hex((unsigned long)ptrs[i]);
        uart_puts("\n");
    }

    uart_puts("\nAll tests completed.\n");

    while (1) {}; // stop here!
}


