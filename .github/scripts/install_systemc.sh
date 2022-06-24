cd ~
apt install build-essential make wget git gcc g++ automake
wget http://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.gz
tar -xzf systemc-2.3.3.gz
cd systemc-2.3.3
./configure --prefix=$HOME/systemc-2.3.3/
make
make install