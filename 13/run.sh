#!/bin/bash

set -xum

# QEMUへのファイルパス
QEMU=qemu-system-riscv32

# clangのパス
CC=clang

OBJCOPY=/usr/bin/llvm-objcopy

CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib"

# シェルをビルド
${CC} ${CFLAGS} -Wl,-Tuser.ld -Wl,-Map=shell.map -o shell.elf shell.c user.c common.c
${OBJCOPY} --set-section-flags .bss=alloc,contents -O binary shell.elf shell.bin
${OBJCOPY} -Ibinary -Oelf32-littleriscv shell.bin shell.bin.o

# カーネルビルド
${CC} ${CFLAGS} -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
	kernel.c common.c shell.bin.o

# QEMUを起動
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
	-kernel kernel.elf
