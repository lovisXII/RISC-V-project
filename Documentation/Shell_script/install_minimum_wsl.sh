echo "Please be in sudo"
echo "This Script setup a ws/Ubuntu envirronment"
apt update
apt install ghdl gtkwave
apt install gedit
apt-get install python3.6
apt install gcc
apt install clang
apt install ssh
apt install git
echo "Installing pi 4 compiler :"
sudo apt-get install gcc-aarch64-linux-gnu
echo "Installing riscv compiler"
sh install_riscv.sh
sh install_systemc.sh