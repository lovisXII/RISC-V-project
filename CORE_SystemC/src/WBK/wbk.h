#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"
/*
REG_DATA : data at destination of REG, will be WBK in REG
MEM_DATA : data get from a load access, if no load it's just the result from
exe_res


*/

SC_MODULE(wbk) {
    // Reg Interface :

    sc_out<sc_uint<6>>  WADR_SW;
    sc_out<sc_uint<32>> WDATA_SW;
    sc_out<bool>        WENABLE_SW;

    // Mem Interface :

    sc_in<sc_uint<32>> MEM_RES_RM;
    sc_in<sc_uint<6>>  DEST_RM;
    sc_in<bool>        WB_RM;
    sc_in<sc_uint<32>> PC_MEM2WBK_RM;
    sc_in<sc_uint<32>> CSR_RDATA_RM;
    sc_in<bool>        CSR_WENABLE_RM;
    sc_in<sc_uint<2>>  CURRENT_MODE_SM ;

    sc_in<bool>        MULT_INST_RM;
    sc_in<sc_uint<32>> X2_RES_RX2; 
    
    // Mem2wbk interface

    sc_in<bool>  MEM2WBK_EMPTY_SM;
    sc_out<bool> MEM2WBK_POP_SW;

    // Global Interface :

    sc_in_clk CLK;
    sc_in_clk RESET;

    // Interruption :

    sc_in<bool> INTERRUPTION_SE;

    // FIFO

    void wbk_method();
    void trace(sc_trace_file * tf);
    SC_CTOR(wbk) {
        SC_METHOD(wbk_method);
        sensitive << MEM_RES_RM << DEST_RM << WB_RM << MEM2WBK_EMPTY_SM << X2_RES_RX2 << MULT_INST_RM;
    }
};