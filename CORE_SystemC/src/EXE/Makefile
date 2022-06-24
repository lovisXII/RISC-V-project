CC=g++
INCLUDE_PATH=/usr/local/systemc-2.3.3/include
LINK_PATH=/usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_ARGS=-I$(INCLUDE_PATH) -L$(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g


all: alu_tb shifter_tb exec_tb

exec_tb: alu.o shifter.o exec.o exec_tb.o ../UTIL/fifo_78b/fifo_78b.o
	$(CC) $^ -o $@ $(C_ARGS)

alu_tb: alu.o alu_tb.o
	$(CC) $^ -o $@ $(C_ARGS)

shifter_tb: shifter.o shifter_tb.o
	$(CC) $^ -o $@ $(C_ARGS)
	
%.o: %.cpp
	$(CC) -c $^ $(C_ARGS) 
	 
../UTIL/fifo_78b/fifo_78b.o: ../UTIL/fifo_78b/fifo_78b.cpp
	make -C ../UTIL/fifo_78b/

clean: 
	rm -f *.o main alu_tb shifter_tb exec_tb *.vcd
	cd ../UTIL/fifo_78b/ && make clean