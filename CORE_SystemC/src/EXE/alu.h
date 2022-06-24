#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"

SC_MODULE(alu) {
    sc_in<sc_uint<32>>  OP1_SE, OP2_SE;
    sc_in<bool>         CIN_SE;
    sc_in<sc_uint<2>>   CMD_SE;
    sc_out<sc_uint<32>> RES_SE;

    void operation();
    void trace(sc_trace_file * tf);

    SC_CTOR(alu) {
        SC_METHOD(operation);
        sensitive << OP1_SE << OP2_SE << CIN_SE << CMD_SE;
    }
};
