#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"

SC_MODULE(timer) {
    // Csr interface
    sc_in<bool>         TIMER_CONFIG_WB_SC;
    sc_in<bool>         TIMER_DIVIDER_WB_SC;
    sc_in<sc_uint<32>>  DATA_SC;
    sc_out<sc_uint<64>> TIME_RT;
    sc_out<bool>        TIMER_INT_ST;
    sc_in<bool>         ACK_SP;

    // Global Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // Internal signals
    sc_signal<sc_uint<32>> counter_rt;
    sc_signal<sc_uint<32>> divider_rt;
    sc_signal<sc_uint<2>>  config_rt;

    void trace(sc_trace_file * tf);
    void thread();
    SC_CTOR(timer) { SC_CTHREAD(thread, CLK.pos()); }
};