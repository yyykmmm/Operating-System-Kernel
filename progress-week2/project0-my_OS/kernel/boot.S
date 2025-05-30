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
