#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NOC='\033[0m'
mkdir ../build
cd ../build 
cmake ../CORE_SystemC/src/
make -j install
cd ../tests/
riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T app.ld exception.s reset.s -o kernel
for file in $(ls tests); do 
    printf "Test ${file} non opt..." 
    timeout 20s ./run_test.sh tests/$file >/dev/null 2>&1
    if (($? == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    else
        printf "${RED} failed\n${NOC}"
    fi
    printf "Test ${file} opt..." 
    timeout 20s ./run_test.sh tests/$file -O2 >/dev/null 2>&1
    if (($? == 0)) 
    then
        printf "${GREEN} passed\n${NOC}"
    else
        printf "${RED} failed\n${NOC}"
    fi

done
