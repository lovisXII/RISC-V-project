#ifndef BUFFER_CACHE
#define BUFFER_CACHE

#include <systemc.h>
#include "../UTIL/debug_util.h"

SC_MODULE(buffercache)
{
    sc_in<bool> RESET_N;
    sc_in_clk CLK;

    //INPUT from DCACHE
    sc_in<bool> WRITE_OBUFF;
    sc_in<bool> READ_OBUFF;

    sc_in<sc_uint<32>> DATA_C;
    sc_in<sc_uint<32>> ADR_C;
    sc_in<bool> STORE_C;
    sc_in<bool> LOAD_C;

    //OUTPUT
    //TO DCACHE
    sc_out<bool> FULL;
    sc_out<bool> EMPTY;

    //TO RAM
    sc_out<sc_uint<32>> DATA_MP;   // MP mem primaire
    sc_out<sc_uint<32>> ADR_MP;
    sc_out<bool> STORE_MP;
    sc_out<bool> LOAD_MP;

    //signals
    // buffers
    sc_signal<sc_uint<2>> buffer_LRU;
    //buff0
    sc_signal<sc_uint<32>> buff0_DATA;
    sc_signal<sc_uint<32>> buff0_DATA_ADR;
    sc_signal<bool> buff0_STORE;
    sc_signal<bool> buff0_LOAD;
    sc_signal<bool> buff0_VALIDATE;  // data valid on buffer

    //buff1
    sc_signal<sc_uint<32>> buff1_DATA;
    sc_signal<sc_uint<32>> buff1_DATA_ADR;
    sc_signal<bool> buff1_STORE;
    sc_signal<bool> buff1_LOAD;
    sc_signal<bool> buff1_VALIDATE;  // data valid on buffer

    //debug
    sc_signal<sc_uint<5>> buff_readed;
    void fifo();

    void bufferfull(); 

    void trace(sc_trace_file*);

    SC_CTOR(buffercache)
    {
        SC_METHOD(fifo);
        sensitive << CLK.neg() << WRITE_OBUFF.pos();

        SC_METHOD(bufferfull);
        sensitive << buff0_VALIDATE << buff1_VALIDATE;

        reset_signal_is(RESET_N, false);
    }
};

#endif