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
