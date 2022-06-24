sudo apt-get install device-tree-compiler
PWD=$(pwd)
cd ~
git clone https://github.com/riscv-software-src/riscv-isa-sim.git
cd riscv-isa-sim
mkdir build
cd build
mkdir $HOME/spike/
../configure --prefix=$HOME/spike/
make
make install 