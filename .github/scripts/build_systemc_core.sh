#!/bin/bash
make -C CORE clean
PATH=$PATH:$HOME/riscv/bin SYSTEMC=~/systemc-2.3.3 make -C CORE