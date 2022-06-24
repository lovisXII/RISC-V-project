#! /bin/bash
# This script allow you to setup the riscof test frimework
# You will need it if you really want to test our design, otherwise it will be
# useless for you to run this script

export TEMPORARY_PATH=$PWD/../riscof
cd $TEMPORARY_PATH
################### PYTHON SETUP ################### 

echo "Please run in sudo"
echo "Installing python"

# sudo apt-get install python3.6
# pip3 install --upgrade pip
# sudo add-apt-repository ppa:deadsnakes/ppa
# sudo apt-get update
# sudo apt-get install python3.6 -y
# pip3 install --upgrade pip


################### RISCOF SETUP ################### 

if [ ! [ riscof > /dev/null 2>&1 ] ]
then
    echo "Installing riscof"
    pip3 install git+https://github.com/riscv/riscof.git
fi


################### GNU-TOOLCHAIN SETUP ################### 

echo "Installing GNU Toolchain"

# sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
if [ ! [ riscv32-unknown-elf-gcc -v > /dev/null 2>&1 ] ] 
then 
    echo "#######################################" 
    echo "#######################################"
    echo "#######################################"
    echo "#######################################"
    echo " PLEASE BE PATIENT THIS OPERATION CAN BE QUITE LONG"
    echo "#######################################" 
    echo "#######################################"
    echo "#######################################"
    echo "#######################################"
    echo "Launching install_riscv to install riscv compiler" 
    sudo ./install_riscv.sh
else
    echo "gnu toolchain is already installed and setup in the bashrc"
fi

################### SPIKE SETUP ################### 

if [ ! [ spike -v > /dev/null 2>&1 ] ] 
then
        echo "Unlucky you, spike isn't installed : Installing spike"
        echo "#######################################" 
        echo "#######################################"
        echo "#######################################"
        echo "#######################################"
        echo " PLEASE BE PATIENT THIS OPERATION IS QUITE LONG"
        echo "#######################################" 
        echo "#######################################"
        echo "#######################################"
        echo "#######################################"
        sudo apt-get install device-tree-compiler
        cd /tmp/ && git clone https://github.com/riscv-software-src/riscv-isa-sim.git
        cd riscv-isa-sim
        mkdir build
        cd build
        ../configure --prefix=$TEMPORARY_PATH
        make -j4
        sudo make install #sudo is required depending on the path chosen in the previous setup
        echo "export $PATH=/opt/spike/bin:$PATH" >> ~/.bashrc
        source ~/.bashrc
else 
    echo "#######################################" 
    echo "#######################################"
    echo "#######################################"
    echo "#######################################"
    echo "LUCKY YOU ! SPIKE IS ALREADY INSTALLED"
    echo "#######################################" 
    echo "#######################################"
    echo "#######################################"
    echo "#######################################"
fi


################### CONFIG SETUP ################### 

cd $TEMPORARY_PATH 
if ! [ -f "config.ini" ] 
then
    echo "
    [RISCOF]
    ReferencePlugin=spike
    ReferencePluginPath=$PWD/RISC-V-project/riscof/spike
    DUTPlugin=projet
    DUTPluginPath=$PWD/RISC-V-project/riscof/projet

    [spike]
    pluginpath=$PWD/RISC-V-project/riscof/spike
    ispec=$PWD/RISC-V-project/riscof/spike/spike_isa.yaml
    pspec=$PWD/RISC-V-project/riscof/spike/spike_platform.yaml
    target_run=1

    [sail_cSim]
    pluginpath=$PWD/RISC-V-project/riscof/sail_cSim

    [projet]
    pluginpath=$PWD/RISC-V-project/riscof/projet
    ispec=$PWD/RISC-V-project/riscof/projet/projet_isa.yaml
    pspec=$PWD/RISC-V-project/riscof/projet/projet_platform.yaml
    PATH=$PWD/RISC-V-project/CORE/core_tb">>config.ini
fi
riscof --verbose info arch-test --clone