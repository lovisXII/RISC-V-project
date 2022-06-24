CC=g++
INCLUDE_PATH=/usr/local/systemc-2.3.3/include
LINK_PATH=/usr/local/systemc-2.3.3/lib-linux64
LINKER_ARGS=-rpath=/usr/local/systemc-2.3.3/lib-linux64
LIBS=-lsystemc -lm
C_ARGS=-I$(INCLUDE_PATH) -L$(LINK_PATH) -Wl,$(LINKER_ARGS) $(LIBS) -g
FIFO_PATH=../UTIL/fifo_42b/

all: mem_tb

mem_tb: mem_tb.o $(FIFO_PATH)fifo_42b.o 
	$(CC) $^ -o $@ $(C_ARGS)

%.o: %.cpp
	$(CC) -c $^ $(C_ARGS)

$(FIFO_PATH)fifo_42b.o: $(FIFO_PATH)fifo_42b.cpp
	make -C $(FIFO_PATH)

clean:
	cd $(FIFO_PATH) && make clean
	rm *.o mem_tb