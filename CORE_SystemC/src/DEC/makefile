CC=g++
INCLUDE_PATH=/usr/local/systemc-2.3.3/include
LINK_PATH=/usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_ARGS=-I$(INCLUDE_PATH) -L$(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
fifo_110b=../UTIL/fifo_110b/fifo_110b

all: dec_tb 

dec_tb: dec.o dec_tb.o ../UTIL/fifo_32b/fifo_32b.o $(fifo_110b).o
	$(CC) $^ -o $@ $(C_ARGS)

%.o: %.cpp	
	$(CC) -c $^ $(C_ARGS)

../UTIL/fifo_32b/fifo_32b.o: ../UTIL/fifo_32b/fifo_32b.cpp
	make -C ../UTIL/fifo_32b/

$(fifo_110b).o: $(fifo_110b).cpp
	make -C ../UTIL/fifo_110b/
	
clean:
	rm -f *.o dec_tb *.vcd 