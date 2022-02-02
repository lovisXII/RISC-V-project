#include <systemc.h>

SC_MODULE(fifo_78b)
{
    sc_in< sc_bv<78> > DIN ;
    sc_in_clk CLK ;
    sc_in<bool> PUSH, POP ;
    sc_in<bool> RESET_N ;
    
    sc_out<bool> FULL, EMPTY ;
    sc_out< sc_bv<78> > DOUT ;

    sc_signal<bool> fifo_v ;
    sc_signal< sc_bv<78> > data_inside ;
    
    void function() ;

    SC_CTOR(fifo_78b)
    {
        SC_CTHREAD(function,fifo_78b::CLK.pos()) ;
        reset_signal_is(RESET_N, false) ;
    }
};