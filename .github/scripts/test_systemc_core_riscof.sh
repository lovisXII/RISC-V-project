#! /bin/bash
export PATH=$PATH:$HOME/riscv/bin
export PATH=$PATH:$HOME/spike/bin
source ~/venv/bin/activate 
cd riscof
riscof validateyaml --config=config.ini
riscof testlist --config=config.ini --suite=riscv-arch-test/riscv-test-suite/ --env=riscv-arch-test/riscv-test-suite/env
riscof run --config=config.ini --suite=riscv-arch-test/riscv-test-suite/ --env=riscv-arch-test/riscv-test-suite/env
