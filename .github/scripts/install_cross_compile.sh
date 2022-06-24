#!/bin/bash

wget https://github.com/stnolting/riscv-gcc-prebuilt/releases/download/rv32i-2.0.0/riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz

mkdir -p ~/riscv

tar -xzf riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz -C ~/riscv

