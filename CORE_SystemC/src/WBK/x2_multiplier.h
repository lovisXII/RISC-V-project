#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"

SC_MODULE(x2_multiplier)
{
    // Input :
    sc_in<sc_bv<128>> IN_RX1;
    sc_in<bool> SIGNED_OP_RX1;
    sc_in<bool> CARRY_RX1;

    sc_in<bool> X12X2_EMPTY_SX1;

    // Output :
    sc_out<sc_uint<32>> RES_RX2;
    sc_out<bool>        X12X2_POP_SX2;

    // General interace : 
    sc_in_clk   CLK;
    sc_in<bool> RESET;

    sc_signal<sc_bv<64>> a;
    sc_signal<sc_bv<64>> b;
    sc_signal<bool> S[64];
    sc_signal<bool> G[64];
    sc_signal<bool> P[64];

    sc_signal<bool> c[64];

    void pre_process();
    //modified full adder
    void MFA_0();
    void C_gen_1();

    void RES();

    void manage_fifo();
    void trace(sc_trace_file* tf);

    SC_CTOR(x2_multiplier)
    {
        SC_METHOD(pre_process);
        sensitive << IN_RX1 << CARRY_RX1 << SIGNED_OP_RX1 << CLK;

        SC_METHOD(MFA_0);
        sensitive << a << b << c[0];
        SC_METHOD(C_gen_1);
        sensitive << G[0] << P[0] << c[0];

        SC_METHOD(RES);
        for(int i = 0; i < 64; i++)
          sensitive << S[i];
        sensitive << SIGNED_OP_RX1 << CLK;

        SC_METHOD(manage_fifo);
        sensitive << X12X2_EMPTY_SX1;
  }
};