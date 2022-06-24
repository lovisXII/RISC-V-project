if (($1 == "--help")) 
then 
echo "Usage : run_test [file] [compiler options] [simulator options]"
else
riscv32-unknown-elf-gcc -nostdlib -march=rv32im -T app.ld $2 $1
./../install/bin/core_tb a.out $3
fi
