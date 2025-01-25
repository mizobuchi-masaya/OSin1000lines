#!/bin/bash

set -xum

# QEMUへのファイルパス
QEMU=qemu-system-riscv32

# clangのパス
CC=clang
##CC=clang-19

OBJCOPY=/usr/bin/llvm-objcopy
##OBJCOPY=/usr/bin/llvm-objcopy-19

CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"
FLOATSRC="float/floatsidf.c float/muldf3.c float/truncdfsf2.c float/extendsfdf2.c float/mulsf3.c float/subdf3.c float/subsf3.c float/addsf3.c float/adddf3.c float/comparesf2.c float/fp_mode.c"
#FLOATSRC="float/floatsidf.c float/muldf3.c float/truncdfsf2.c float/extendsfdf2.c float/mulsf3.c float/subdf3.c float/subsf3.c float/addsf3.c float/adddf3.c"
#FLOATSRC=

# シェルをビルド
${CC} ${CFLAGS} -I./float -Wl,-Tuser.ld -Wl,-Map=shell.map -o shell.elf shell.c user.c common.c ${FLOATSRC}
${OBJCOPY} --set-section-flags .bss=alloc,contents -O binary shell.elf shell.bin
${OBJCOPY} -Ibinary -Oelf32-littleriscv shell.bin shell.bin.o

# カーネルビルド
${CC} ${CFLAGS} -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
	kernel.c common.c shell.bin.o

# QEMUを起動
## $QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
##     -d unimp,guest_errors,int,cpu_reset -D qemu.log \
##     -drive id=drive0,file=lorem.txt,format=raw,if=none \
##     -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
## 	-kernel kernel.elf
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -d unimp,guest_errors,int,cpu_reset -D qemu.log \
    -drive id=drive0,file=disk.tar,format=raw,if=none \
    -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
	-kernel kernel.elf
