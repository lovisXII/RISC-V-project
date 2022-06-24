CC=g++
INCLUDE_PATH=/usr/local/systemc-2.3.3/include -I../../ELFIO
LINK_PATH=/usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_ARGS=-I$(INCLUDE_PATH) -L$(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g


all: dcache_tb 

dcache_tb: dcache.o dcache_tb.o
	$(CC) $^ -o $@ $(C_ARGS)
	
%.o: %.cpp
	$(CC) -c $^ $(C_ARGS) 

clean: 
	rm -f *.o main dcache_tb *.vcd