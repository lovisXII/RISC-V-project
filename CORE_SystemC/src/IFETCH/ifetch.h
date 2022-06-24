#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"

#define nop_encoding 0x0000013
#define if2dec_size  64

SC_MODULE(ifetch) {
    // Icache Interface :

    sc_in<sc_bv<32>> IC_INST_SI;
    sc_in<bool>      IC_STALL_SI;

    sc_out<sc_uint<32>> ADR_SI;  // @ which we search a data block from memory
    sc_out<bool>        ADR_VALID_SI;

    // dec2if interface :

    sc_in<bool>  DEC2IF_EMPTY_SI;
    sc_out<bool> DEC2IF_POP_SI;

    // if2dec interface

    sc_in<bool>      IF2DEC_FLUSH_SD;  // allow to flush if2dec in case of a branch
    sc_in<bool>      IF2DEC_POP_SD;
    sc_in<sc_bv<32>> PC_RD;  // PC coming to fetch an instruction

    sc_out<bool>        IF2DEC_EMPTY_SI;
    sc_out<sc_bv<32>>   INSTR_RI;      // instruction sent to if2dec
    sc_out<sc_uint<32>> PC_IF2DEC_RI;  // pc sent to if2dec

    sc_out<bool> EXCEPTION_RI;  // tells if an instruction have been made in IFETCH

    // Interruption :

    sc_in<bool> INTERRUPTION_SE;
    sc_in<sc_uint<2>> CURRENT_MODE_SM ;
    sc_in<bool> MRET_SM ;
    sc_in<sc_uint<32>> RETURN_ADRESS_SM ;

    // Global Interface :

    sc_in<bool> EXCEPTION_SM;
    sc_in_clk   CLK;
    sc_in<bool>   RESET;

    // FIFO
    fifo<if2dec_size> fifo_inst;

    // Internals signals :

    sc_signal<bool>                 IF2DEC_PUSH_SI;
    sc_signal<bool>                 IF2DEC_FULL_SI;
    sc_signal<sc_bv<if2dec_size>>   if2dec_in_si;
    sc_signal<sc_bv<if2dec_size>>   instr_ri;  // instruction sent to if2dec

    
    void fetch_method();
    void trace(sc_trace_file * tf);
    void exception();
    
    SC_CTOR(ifetch) : fifo_inst("if2dec") {
        fifo_inst.DIN_S(if2dec_in_si);
        fifo_inst.DOUT_R(instr_ri);
        fifo_inst.EMPTY_S(IF2DEC_EMPTY_SI);
        fifo_inst.FULL_S(IF2DEC_FULL_SI);
        fifo_inst.PUSH_S(IF2DEC_PUSH_SI);
        fifo_inst.POP_S(IF2DEC_POP_SD);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(fetch_method);
        sensitive << IC_INST_SI << DEC2IF_EMPTY_SI 
        << IF2DEC_FULL_SI << PC_RD 
        << IF2DEC_FLUSH_SD << IC_STALL_SI 
        << RESET
        << EXCEPTION_SM 
        << MRET_SM 
        << RETURN_ADRESS_SM;
        SC_METHOD(exception)
        sensitive << RESET << EXCEPTION_SM ;
    }
};
