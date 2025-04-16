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
