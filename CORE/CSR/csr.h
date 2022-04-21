#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"

#define N_CSR 12
#define adress_mtvec 0x80000000

SC_MODULE(csr) {
    // Input :

    sc_in<sc_uint<12>> CSR_WADR_SM;
    sc_in<sc_uint<32>> CSR_WDATA_SM;
    sc_in<bool>        CSR_ENABLE_BEFORE_FIFO_SM;

    sc_in<bool>         EXCEPTION_RM;
    sc_in<sc_uint<32>>  MSTATUS_WDATA_RM;
    sc_in<sc_uint<32>>  MIP_WDATA_RM;
    sc_in<sc_uint<32>>  MEPC_WDATA_RM;
    sc_in<sc_uint<32>>  MCAUSE_WDATA_RM;
    sc_out<sc_uint<32>> MTVEC_VALUE_RC;
    sc_out<sc_uint<32>> MIP_VALUE_RC;

    // Output :

    sc_in<sc_uint<12>>  CSR_RADR_SD;
    sc_out<sc_uint<32>> CSR_RDATA_SC;

    // General Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET_N;

    // Internal signals :
    /*** Register Map ***
     * 0: mvendorid 0xF11
     * 1: marchid 0xF12
     * 2: mimpid 0xF13
     * 3: mstatus 0x300
     * 4: misa 0x301
     * 5: mie 0x304
     * 6: mtvec 0x305
     * 7: mstatush 0x310
     * 8: mepc 0x341
     * 9: mcause 0x342
     * 10: mtval 0x343
     * 11: mip 0x344
     ***/
    sc_signal<sc_uint<32>> csr_rc[N_CSR];
    sc_signal<sc_uint<32>> test_sc ;
    void writing_csr();
    void reading_csr();
    void trace(sc_trace_file * tf);
    SC_CTOR(csr) {
        SC_CTHREAD(writing_csr, CLK.pos());
        SC_METHOD(reading_csr);
        sensitive << CSR_WADR_SM << CSR_RADR_SD << CSR_ENABLE_BEFORE_FIFO_SM << EXCEPTION_RM
                  << MSTATUS_WDATA_RM << MIP_WDATA_RM << MEPC_WDATA_RM << MCAUSE_WDATA_RM ;
        for (int i = 0; i < N_CSR; i++)
            sensitive << csr_rc[i];
    }
};