#pragma once

#include <systemc.h>
#include <iostream>
#include <string>
#include "../UTIL/debug_util.h"

SC_MODULE(shifter) {
    sc_in<sc_uint<32>> DIN_SE;        // input
    sc_in<sc_uint<5>>  SHIFT_VAL_SE;  // shift value
    sc_in<sc_uint<2>>  CMD_SE;        // command

    sc_out<sc_uint<32>> DOUT_SE;  // output

    // Internal signals :

    sc_signal<bool>        sll_se, srl_se, sra_se;  // command, only one must be true
    sc_signal<sc_uint<32>> sll_dout_se,
        sr_dout_se;  // temporary output for each component

    void decode_cmd();
    void shifter_sll();       // component to shift left the input
    void shifter_sr();        // component to shift right the input, arithmetic or logic
    void shifter_agregate();  // component to choose which shift to use
    void trace(sc_trace_file * tf);

    SC_CTOR(shifter) {
        SC_METHOD(decode_cmd);
        sensitive << CMD_SE;
        SC_METHOD(shifter_sll);
        sensitive << DIN_SE << SHIFT_VAL_SE;
        SC_METHOD(shifter_sr);
        sensitive << DIN_SE << SHIFT_VAL_SE << sra_se;
        SC_METHOD(shifter_agregate);
        sensitive << sll_dout_se << sr_dout_se << sll_se << srl_se << sra_se;
    }
};
