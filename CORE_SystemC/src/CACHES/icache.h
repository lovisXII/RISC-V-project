#ifndef I_CACHE
#define I_CACHE

#include <systemc.h>
#include "../UTIL/debug_util.h"

SC_MODULE(icache)
{
    sc_in_clk CLK;
    sc_in<bool> RESET_N;

    // interface processeur
    sc_in<sc_uint<32> > ADR_SI ; 
    sc_in<bool> ADR_VALID_SI ; 

    sc_out<sc_bv<32> > IC_INST_SI ;
    sc_out<bool> IC_STALL_SI ;

    //interface MP
    sc_in<sc_uint<32>> DT;
    sc_out<sc_uint<32>> A;
    sc_out<bool> DTA_VALID;

    sc_in<bool> SLAVE_ACK_SP;

    //signals
    sc_signal<sc_bv<32>> data[256][4];
    sc_signal<sc_uint<20>> tag[256];
    sc_signal<bool> data_validate[256];

    sc_signal<sc_uint<20>> address_tag;
    sc_signal<sc_uint<8>> address_index;
    sc_signal<sc_uint<4>> address_offset;

    sc_signal<bool> hit;

    sc_signal<sc_uint<3>> fsm_state;

    void trace(sc_trace_file*);

    void parse_adr();
    void miss_detection();
    void transition();

    SC_CTOR(icache)
    {
        SC_METHOD(parse_adr);
        sensitive << ADR_SI; 

        SC_METHOD(miss_detection);
        sensitive   << address_tag 
                    << address_index 
                    << address_offset 
                    << IC_STALL_SI;
        for(int i=0; i < 256 ;i++){
            sensitive << data[i][0]
                        << data[i][1]
                        << data[i][2]
                        << data[i][3]
                        << data_validate[i];
        }

        SC_THREAD(transition);
        sensitive << CLK.neg() << DT << SLAVE_ACK_SP;

        reset_signal_is(RESET_N, false);
    }
};

#endif