#pragma once
#include <systemc.h>
#include <iostream>
#include <string>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"
#include "alu.h"
#include "shifter.h"
SC_MODULE(exec) {
    // Input/Output of EXE :

    sc_in<sc_uint<32>> OP1_RD;  // can contains CSR if CSR_type_operation_RD == 1
    sc_in<sc_uint<32>> OP2_RD;
    sc_in<bool>        OP1_VALID_RD;
    sc_in<bool>        OP2_VALID_RD;
    sc_in<sc_uint<6>>  RADR1_RD;
    sc_in<sc_uint<6>>  RADR2_RD;

    sc_in<sc_uint<32>> PC_DEC2EXE_RD;
    sc_in<sc_uint<32>> MEM_DATA_RD;
    sc_in<sc_uint<6>>  DEST_RD;
    sc_in<sc_uint<2>>  CMD_RD;
    sc_in<sc_uint<2>>  MEM_SIZE_RD;
    sc_in<bool>        NEG_OP2_RD, WB_RD;
    sc_in<bool>        MEM_SIGN_EXTEND_RD;
    sc_in<bool>        SELECT_SHIFT_RD;  // taille fifo entrée : 110
    sc_in<bool>        MEM_LOAD_RD, MEM_STORE_RD;
    sc_in<bool>        EXE2MEM_POP_SM;
    sc_in<bool>        DEC2EXE_EMPTY_SD;
    sc_in<bool>        SLT_RD, SLTU_RD;

    sc_in<bool>        CSR_WENABLE_RD;
    sc_in<sc_uint<12>> CSR_WADR_RD;
    sc_in<sc_uint<32>> CSR_RDATA_RD;

    // Exception coming from Decod :

    sc_in<bool> ECALL_I_SD;
    sc_in<bool> EBREAK_I_SD;
    sc_in<bool> ILLEGAL_INSTRUCTION_RD;  // accessing stuff in wrong mode
    sc_in<bool> ADRESS_MISSALIGNED;      // branch offset is misaligned
    sc_in<bool> SYSCALL_U_MODE_SD;
    sc_in<bool> SYSCALL_S_MODE_SD;

    sc_in<bool> EXCEPTION_RD;  // tells if an instruction have been made in DEC

    // Interruption :

    sc_out<bool> INTERRUPTION_SE;
    sc_in<bool>  INTERRUPTION_SX;  // asynchrone Interruption from outside

    // Genral Interface :

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // Fifo exe2mem interface :

    sc_out<sc_uint<32>> EXE_RES_RE;
    sc_out<sc_uint<32>> MEM_DATA_RE;
    sc_out<sc_uint<6>>  DEST_RE;
    sc_out<sc_uint<2>>  MEM_SIZE_RE;
    sc_out<sc_uint<32>> PC_EXE2MEM_RE;

    sc_out<bool> WB_RE, MEM_SIGN_EXTEND_RE;  // taille fifo sortie : 7
    sc_out<bool> MEM_LOAD_RE, MEM_STORE_RE;
    sc_out<bool> EXE2MEM_EMPTY_SE, DEC2EXE_POP_SE;

    sc_out<bool>        CSR_WENABLE_RE;
    sc_out<sc_uint<12>> CSR_WADR_RE;
    sc_out<sc_uint<32>> CSR_RDATA_RE;

    // Internals signals :

    sc_signal<sc_uint<32>> exe_res_se;

    sc_signal<sc_bv<153>> exe2mem_din_se;  // concatenation of exe_res, mem_data...etc
    sc_signal<sc_bv<153>> exe2mem_dout_se;

    sc_signal<sc_uint<32>> op1_se;
    sc_signal<sc_uint<32>> op2_se;
    sc_signal<sc_uint<32>> alu_in_op2_se;
    sc_signal<sc_uint<32>> alu_out_se;
    sc_signal<sc_uint<32>> shifter_out_se;
    sc_signal<sc_uint<32>> bp_mem_data_sd;
    sc_signal<sc_uint<5>>  shift_val_se;

    sc_signal<bool> exe2mem_push_se, exe2mem_full_se;
    sc_signal<bool> blocked;

    sc_signal<bool> wb_re;
    sc_signal<bool> mem_load_re;
    sc_signal<bool> mem_store_re;

    // bypasses

    sc_in<sc_uint<6>>  MEM_DEST_RM;
    sc_in<sc_uint<32>> MEM_RES_RM;
    sc_in<bool>        CSR_WENABLE_RM;
    sc_in<sc_uint<32>> CSR_RDATA_RM;

    // Instance used :

    alu       alu_inst;
    shifter   shifter_inst;
    fifo<153> fifo_inst;

    void preprocess_op();    // send op2 or ~op2 in ALU_IN_OP2
    void select_exec_res();  // setup FFIN_EXE_RES as ALU_OUT or SHIFTER_OUT
    void fifo_concat();      // setup result  in fifo exe2mem
    void fifo_unconcat();    // unconcatenet result from the fifo
    void manage_fifo();      // allow the push/pop of fifo exe2mem

    void bypasses();  // allow the push/pop of fifo exe2mem
    void exception();

    void trace(sc_trace_file * tf);
    SC_CTOR(exec) : alu_inst("alu"), shifter_inst("shifter"), fifo_inst("exe2mem") {
        // ALU port map :

        alu_inst.OP1_SE(op1_se);
        alu_inst.OP2_SE(alu_in_op2_se);
        alu_inst.CMD_SE(CMD_RD);
        alu_inst.CIN_SE(NEG_OP2_RD);
        alu_inst.RES_SE(alu_out_se);

        // Shifter port map :

        shifter_inst.DIN_SE(op1_se);
        shifter_inst.SHIFT_VAL_SE(shift_val_se);
        shifter_inst.CMD_SE(CMD_RD);
        shifter_inst.DOUT_SE(shifter_out_se);

        // fifo port map

        fifo_inst.DIN_S(exe2mem_din_se);
        fifo_inst.DOUT_R(exe2mem_dout_se);
        fifo_inst.EMPTY_S(EXE2MEM_EMPTY_SE);
        fifo_inst.FULL_S(exe2mem_full_se);
        fifo_inst.PUSH_S(exe2mem_push_se);
        fifo_inst.POP_S(EXE2MEM_POP_SM);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(preprocess_op);
        sensitive << op1_se << NEG_OP2_RD << op2_se;
        SC_METHOD(select_exec_res);
        sensitive << alu_out_se << shifter_out_se << SELECT_SHIFT_RD;
        SC_METHOD(fifo_concat);
        sensitive << bp_mem_data_sd << DEST_RD << MEM_SIZE_RD << MEM_LOAD_RD << MEM_SIGN_EXTEND_RD << MEM_STORE_RD
                  << WB_RD << exe_res_se << mem_load_re << mem_store_re << wb_re;
        SC_METHOD(fifo_unconcat);
        sensitive << exe2mem_dout_se;
        SC_METHOD(manage_fifo);
        sensitive << exe2mem_full_se << DEC2EXE_EMPTY_SD << OP1_VALID_RD << OP2_VALID_RD << blocked;
        SC_METHOD(bypasses);
        sensitive << OP1_VALID_RD << OP2_VALID_RD << MEM_DEST_RM << MEM_RES_RM << DEST_RE << EXE_RES_RE << RADR1_RD
                  << RADR2_RD << OP1_RD << OP2_RD

                  << MEM_DATA_RD;
        SC_METHOD(exception);
        sensitive << INTERRUPTION_SX << WB_RD << MEM_LOAD_RD << MEM_STORE_RD << WB_RD << EXCEPTION_RD;
    }
};
