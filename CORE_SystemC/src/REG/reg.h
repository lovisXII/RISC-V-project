#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"
SC_MODULE(reg) {
    // Reading Port :

    sc_in<sc_uint<6>> RADR1_SD;
    sc_in<sc_uint<6>> RADR2_SD;

    sc_out<sc_uint<32>> RDATA1_SR;  // data output read from register
    sc_out<sc_uint<32>> RDATA2_SR;

    // Writing Port :

    sc_in<sc_uint<6>>  WADR_SW;
    sc_in<bool>        WENABLE_SW;
    sc_in<sc_uint<32>> WDATA_SW;

    sc_in<sc_uint<32>> WRITE_PC_SD;
    sc_in<bool>        WRITE_PC_ENABLE_SD;

    // PC Gestion :

    sc_out<sc_uint<32>> READ_PC_SR;

    // Global Interface :

    sc_in_clk          CLK;
    sc_in<bool>        RESET_N;
    sc_in<sc_uint<32>> PC_INIT;

    // Registres :
    sc_signal<sc_uint<32>> REG_RR[33];

    void reading_adresses();
    void writing_adresse();
    void trace(sc_trace_file * tf);

    SC_CTOR(reg) {
        SC_METHOD(reading_adresses);
        sensitive << RADR1_SD << RADR2_SD << RESET_N;
        for (int i = 0; i < 33; i++)
            sensitive << REG_RR[i];
        SC_CTHREAD(writing_adresse, reg::CLK.pos());
        reset_signal_is(RESET_N, false);
    }
};