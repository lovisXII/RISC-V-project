#include "buffercache.h"
#include <systemc.h>


void buffercache::fifo()
{
    sc_uint<2> b_LRU = buffer_LRU.read();

    //WRITE_OBUFF ON BUFFER
    if(WRITE_OBUFF.read())
    {
        if(!(buff0_VALIDATE && ADR_C.read() == buff0_DATA_ADR.read() ||
            buff1_VALIDATE && ADR_C.read() == buff1_DATA_ADR.read()) 
            && STORE_C.read() || LOAD_C.read())
        {
            if(!buff0_VALIDATE && ADR_C.read() != buff0_DATA_ADR.read())
            {
                buff0_DATA.write(DATA_C.read());
                buff0_DATA_ADR.write(ADR_C.read());
                buff0_STORE.write(STORE_C.read());
                buff0_LOAD.write(LOAD_C.read());
                buff0_VALIDATE.write(STORE_C.read() || LOAD_C.read());

                buffer_LRU.write(0b01);
            }
            else if(!buff1_VALIDATE && ADR_C.read() != buff1_DATA_ADR.read())
            {
                buff1_DATA.write(DATA_C.read());
                buff1_DATA_ADR.write(ADR_C.read());
                buff1_STORE.write(STORE_C.read());
                buff1_LOAD.write(LOAD_C.read());
                buff1_VALIDATE.write(STORE_C.read() || LOAD_C.read());

                buffer_LRU.write(0b10);
            }
        }
    }

    //READ ON BUFFER
    /*std::cout << " buffercache :" << buff0_VALIDATE 
            << "      " << buff1_VALIDATE << std::endl; 
    */
    if(READ_OBUFF.read())
    {
        if(buff0_VALIDATE & buff1_VALIDATE)
        {
            if(b_LRU.range(1,0) == 0b01)
            {
                DATA_MP.write(buff1_DATA.read());
                ADR_MP.write(buff1_DATA_ADR.read());
                STORE_MP.write(buff1_STORE.read());
                LOAD_MP.write(buff1_LOAD.read());

                buff1_VALIDATE.write(false);
                buff_readed.write(0b0001);
            }
            else if(b_LRU.range(1,0) == 0b10)
            {
                DATA_MP.write(buff0_DATA.read());
                ADR_MP.write(buff0_DATA_ADR.read());
                STORE_MP.write(buff0_STORE.read());
                LOAD_MP.write(buff0_LOAD.read());

                buff0_VALIDATE.write(false);
                buff_readed.write(0b0010);
            }
        }
        else if(buff0_VALIDATE & !buff1_VALIDATE)
        {
            DATA_MP.write(buff0_DATA.read());
            ADR_MP.write(buff0_DATA_ADR.read());
            STORE_MP.write(buff0_STORE.read());
            LOAD_MP.write(buff0_LOAD.read());

            buff0_VALIDATE.write(false);
            buff_readed.write(0b0100);
        }
        else if(!buff0_VALIDATE & buff1_VALIDATE)
        {
            DATA_MP.write(buff1_DATA.read());
            ADR_MP.write(buff1_DATA_ADR.read());
            STORE_MP.write(buff1_STORE.read());
            LOAD_MP.write(buff1_LOAD.read());

            buff1_VALIDATE.write(false);
            buff_readed.write(0b1000);
        }
    }

    if(!READ_OBUFF.read() && !WRITE_OBUFF.read())
    {
        STORE_MP.write(false);
        LOAD_MP.write(false);
    }
}

void buffercache::bufferfull()
{
    FULL.write(buff1_VALIDATE & buff0_VALIDATE);
    EMPTY.write(!buff1_VALIDATE & !buff0_VALIDATE);
}

void buffercache::trace(sc_trace_file* tf)
{
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    //INPUT
    sc_trace(tf, WRITE_OBUFF, GET_NAME(WRITE_OBUFF));
    sc_trace(tf, READ_OBUFF, GET_NAME(READ_OBUFF));
    sc_trace(tf, DATA_C, GET_NAME(DATA_C));
    sc_trace(tf, ADR_C, GET_NAME(ADR_C));
    sc_trace(tf, STORE_C, GET_NAME(STORE_C));
    sc_trace(tf, LOAD_C, GET_NAME(STORE_C));

    //OUTPUT
    sc_trace(tf, FULL, GET_NAME(FULL));
    sc_trace(tf, EMPTY, GET_NAME(EMPTY));
    sc_trace(tf, DATA_MP, GET_NAME(DATA_MP));
    sc_trace(tf, ADR_MP, GET_NAME(ADR_MP));
    sc_trace(tf, STORE_MP, GET_NAME(STORE_MP));
    sc_trace(tf, LOAD_MP, GET_NAME(LOAD_MP));

    //signals
    sc_trace(tf, buffer_LRU, GET_NAME(buffer_LRU));

    sc_trace(tf, buff0_DATA, GET_NAME(buff0_DATA));
    sc_trace(tf, buff0_DATA_ADR, GET_NAME(buff0_DATA_ADR));
    sc_trace(tf, buff0_STORE, GET_NAME(buff0_STORE));
    sc_trace(tf, buff0_LOAD, GET_NAME(buff0_LOAD));
    sc_trace(tf, buff0_VALIDATE, GET_NAME(buff0_VALIDATE));

    sc_trace(tf, buff1_DATA, GET_NAME(buff1_DATA));
    sc_trace(tf, buff1_DATA_ADR, GET_NAME(buff1_DATA_ADR));
    sc_trace(tf, buff1_STORE, GET_NAME(buff1_STORE));
    sc_trace(tf, buff1_LOAD, GET_NAME(buff1_LOAD));
    sc_trace(tf, buff1_VALIDATE, GET_NAME(buff1_VALIDATE));

    //debug
    sc_trace(tf, buff_readed, GET_NAME(buff_readed));
}