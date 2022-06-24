#!/bin/bash

echo "This script will installed precompiled binary file for riscv cross-compiler"
echo "Please run in sudo"

if ( cat /etc/*-release | grep Ubuntu > /dev/null 2>&1) then
  wget https://github.com/stnolting/riscv-gcc-prebuilt/releases/download/rv32i-2.0.0/riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
  mkdir -p /opt/riscv
  tar -xzf riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz -C /opt/riscv/
  OS="`uname`"
  if ( grep -q microsoft /proc/version )
  then
    echo "Ubuntu on Windows"
    echo "export PATH=$PATH:/opt/riscv/bin" >> ~/.bashrc
    source ~/.bashrc
  else
    echo "Native Linux"
    echo "export PATH=$PATH:/opt/riscv/bin" >> ~/.bashrc
    source ~/.bashrc
  fi
  rm -f riscv32-unknown-elf.gcc-10.2.0.rv32i.ilp32.newlib.tar.gz
fi

