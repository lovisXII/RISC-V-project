#pragma once
#include <systemc.h>
#include <iostream>
#include <string>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"
#include "alu.h"
#include "shifter.h"

#define exe2mem_size        200
#define start_kernel_adress 0xF0000000

SC_MODULE(exec) {
    // Input/Output of EXE :

    sc_in<sc_uint<32>> OP1_RD;  // can contains CSR if CSR_type_operation_RD == 1
    sc_in<sc_uint<32>> OP2_RD;
    sc_in<bool>        OP1_VALID_RD;
    sc_in<bool>        OP2_VALID_RD;
    sc_in<sc_uint<6>>  RADR1_RD;
    sc_in<sc_uint<6>>  RADR2_RD;
    sc_in<bool>        BLOCK_BP_RD;

    sc_in<sc_uint<32>> PC_DEC2EXE_RD;
    sc_in<sc_uint<32>> MEM_DATA_RD;
    sc_in<sc_uint<6>>  DEST_RD;
    sc_in<sc_uint<2>>  CMD_RD;
    sc_in<sc_uint<2>>  MEM_SIZE_RD;
    sc_in<bool>        NEG_OP2_RD, WB_RD;
    sc_in<bool>        MEM_SIGN_EXTEND_RD;
    sc_in<sc_uint<4>>  SELECT_TYPE_OPERATIONS_RD;  // taille fifo entrée : 110
    sc_in<bool>        MEM_LOAD_RD, MEM_STORE_RD;

    sc_in<bool> EXE2MEM_POP_SM;
    sc_in<bool> MULT_INST_RD;
    sc_in<bool> DEC2EXE_EMPTY_SD;
    sc_in<bool> SLT_RD, SLTU_RD;

    sc_in<bool>        CSR_WENABLE_RD;
    sc_in<sc_uint<12>> CSR_WADR_RD;
    sc_in<sc_uint<32>> CSR_RDATA_RD;

    sc_in<bool> MULT_INST_RM;
    sc_in<bool> BP_MEM2WBK_EMPTY_SM;
    // Exception coming from Decod :

    sc_in<bool>         EXCEPTION_RD;  // tells if an instruction have been made in DEC
    sc_in<sc_uint<2>>   CURRENT_MODE_SM;
    sc_in<sc_uint<32>>  PC_BRANCH_VALUE_RD;
    sc_out<sc_uint<32>> PC_BRANCH_VALUE_RE;

    sc_in<bool> ILLEGAL_INSTRUCTION_RD;  // accessing stuff in wrong mode
    sc_in<bool> ADRESS_MISSALIGNED_RD;   // branch offset is misaligned
    sc_in<bool> ENV_CALL_U_MODE_RD;
    sc_in<bool> ENV_CALL_M_MODE_RD;
    sc_in<bool> ENV_CALL_S_MODE_RD;
    sc_in<bool> ENV_CALL_WRONG_MODE_RD;
    sc_in<bool> INSTRUCTION_ACCESS_FAULT_RD;
    sc_in<bool> MRET_RD;
    sc_in<bool> EBREAK_RD;

    sc_out<bool> EXCEPTION_RE;
    sc_out<bool> LOAD_ADRESS_MISSALIGNED_RE;  // adress from store/load isn't aligned
    sc_out<bool> LOAD_ACCESS_FAULT_RE;        // trying to access memory in wrong mode
    sc_out<bool> STORE_ADRESS_MISSALIGNED_RE;
    sc_out<bool> STORE_ACCESS_FAULT_RE;
    sc_out<bool> ILLEGAL_INSTRUCTION_RE;             // accessing stuff in wrong mode
    sc_out<bool> INSTRUCTION_ADRESS_MISSALIGNED_RE;  // branch offset is misaligned
    sc_out<bool> ENV_CALL_S_MODE_RE;
    sc_out<bool> ENV_CALL_M_MODE_RE;
    sc_out<bool> ENV_CALL_U_MODE_RE;
    sc_out<bool> ENV_CALL_WRONG_MODE_RE;
    sc_out<bool> INSTRUCTION_ACCESS_FAULT_RE;
    sc_out<bool> MRET_RE;
    sc_out<bool> EBREAK_RE;

    sc_out<sc_uint<32>> OP1_SE;
    sc_out<sc_uint<32>> OP2_SE;
    // Interruption :

    sc_out<bool> MACHINE_SOFTWARE_INTERRUPT_SE;
    sc_out<bool> MACHINE_TIMER_INTERRUPT_SE;
    sc_out<bool> MACHINE_EXTERNAL_INTERRUPT_SE;
    sc_out<bool> INTERRUPTION_SE;
    // bypasses

    sc_in<sc_uint<6>>  MEM_DEST_RM;
    sc_in<sc_uint<32>> MEM_RES_RM;
    sc_in<bool>        CSR_WENABLE_RM;
    sc_in<sc_uint<32>> CSR_RDATA_RM;

    // General Interface :

    sc_in<bool> EXCEPTION_SM;
    sc_in_clk   CLK;
    sc_in<bool> RESET;
    sc_in<bool> MACHINE_SOFTWARE_INTERRUPT_SX;
    sc_in<bool> MACHINE_TIMER_INTERRUPT_SX;
    sc_in<bool> MACHINE_EXTERNAL_INTERRUPT_SX;

    // Fifo exe2mem interface :

    sc_out<sc_uint<32>> EXE_RES_RE;
    sc_out<sc_uint<32>> MEM_DATA_RE;
    sc_out<sc_uint<6>>  DEST_RE;
    sc_out<sc_uint<2>>  MEM_SIZE_RE;
    sc_out<sc_uint<32>> PC_EXE2MEM_RE;

    sc_out<bool> WB_RE, MEM_SIGN_EXTEND_RE;  // taille fifo sortie : 7
    sc_out<bool> MEM_LOAD_RE, MEM_STORE_RE;
    sc_out<bool> MULT_INST_RE;      // multiplication instruction
    sc_out<bool> MULT_SEL_HIGH_RE;  // select higher bits of multiplication

    sc_out<bool> EXE2MEM_EMPTY_SE, DEC2EXE_POP_SE;

    sc_out<bool>        CSR_WENABLE_RE;
    sc_out<sc_uint<12>> CSR_WADR_RE;
    sc_out<sc_uint<32>> CSR_RDATA_RE;

    // Internals signals :

    sc_signal<sc_uint<32>> exe_res_se;

    sc_signal<sc_bv<exe2mem_size>> exe2mem_din_se;  // concatenation of exe_res, mem_data...etc
    sc_signal<sc_bv<exe2mem_size>> exe2mem_dout_se;

    sc_signal<sc_uint<32>> op1_se;
    sc_signal<sc_uint<32>> op2_se;
    sc_signal<sc_uint<32>> alu_in_op2_se;
    sc_signal<sc_uint<32>> alu_out_se;
    sc_signal<sc_uint<32>> shifter_out_se;
    sc_signal<sc_uint<32>> divider_out_se;
    sc_signal<sc_uint<32>> bp_mem_data_sd;
    sc_signal<sc_uint<5>>  shift_val_se;

    sc_signal<bool> exe2mem_push_se, exe2mem_full_se;

    sc_signal<bool> blocked;

    sc_signal<bool> wb_re;
    sc_signal<bool> mem_load_re;
    sc_signal<bool> mem_store_re;

    sc_signal<bool> stall_se;
    sc_signal<bool> r1_valid_se;
    sc_signal<bool> r2_valid_se;
    // Exception :

    sc_signal<bool> exception_se;
    sc_signal<bool> load_adress_missaligned_se;  // adress from store/load isn't aligned
    sc_signal<bool> load_access_fault_se;        // trying to access memory in wrong mode
    sc_signal<bool> store_access_fault_se;
    sc_signal<bool> store_adress_missaligned_se;
    // Instance used :

    alu     alu_inst;
    shifter shifter_inst;
    // divider          divider_inst;
    fifo<exe2mem_size> fifo_inst;

    void preprocess_op();    // send op2 or ~op2 in ALU_IN_OP2
    void select_exec_res();  // setup FFIN_EXE_RES as ALU_OUT or SHIFTER_OUT
    void fifo_concat();      // setup result  in fifo exe2mem
    void fifo_unconcat();    // unconcatenet result from the fifo
    void manage_fifo();      // allow the push/pop of fifo exe2mem

    void bypasses();  // allow the push/pop of fifo exe2mem
    void exception();

    void trace(sc_trace_file * tf);
    SC_CTOR(exec)
        : alu_inst("alu"),
          shifter_inst("shifter"),
          // divider_inst("divider"),
          fifo_inst("exe2mem") {
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

        // DIVIDER port map :

        /*divider_inst.OP1_SE(op1_se);
        divider_inst.OP2_SE(alu_in_op2_se);
        divider_inst.CMD_SE(CMD_RD);
        divider_inst.CIN_SE(NEG_OP2_RD);
        divider_inst.RES_SE(divider_out_se);*/

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
        sensitive << alu_out_se << divider_out_se << shifter_out_se << SLT_RD << SLTU_RD << SELECT_TYPE_OPERATIONS_RD
                  << CURRENT_MODE_SM << MEM_LOAD_RD << MEM_STORE_RD << exception_se << RESET;
        SC_METHOD(fifo_concat);
        sensitive << bp_mem_data_sd << DEST_RD << MEM_SIZE_RD << MEM_LOAD_RD << MEM_SIGN_EXTEND_RD << MEM_STORE_RD
                  << WB_RD << exe_res_se << mem_load_re << mem_store_re << wb_re << CSR_WENABLE_RD << CSR_WADR_RD
                  << CSR_RDATA_RD << ILLEGAL_INSTRUCTION_RD << ADRESS_MISSALIGNED_RD << ENV_CALL_U_MODE_RD
                  << ENV_CALL_M_MODE_RD << exception_se << load_adress_missaligned_se << load_access_fault_se
                  << store_access_fault_se << store_adress_missaligned_se << EXCEPTION_SM << MRET_RD
                  << INSTRUCTION_ACCESS_FAULT_RD << MULT_INST_RD;
        SC_METHOD(fifo_unconcat);
        sensitive << exe2mem_dout_se;
        SC_METHOD(manage_fifo);
        sensitive << exe2mem_full_se << DEC2EXE_EMPTY_SD << OP1_VALID_RD << OP2_VALID_RD << exception_se << blocked
                  << r1_valid_se << r2_valid_se;
        SC_METHOD(bypasses);
        sensitive << OP1_VALID_RD << OP2_VALID_RD << MEM_DEST_RM << MEM_RES_RM << DEST_RE << EXE_RES_RE << RADR1_RD
                  << CSR_WENABLE_RE << BLOCK_BP_RD << DEST_RE << MEM_LOAD_RE << CSR_WENABLE_RM << CSR_RDATA_RM
                  << RADR2_RD << OP1_RD << OP2_RD << exception_se << MEM_DATA_RD << MEM_STORE_RD << MULT_INST_RE
                  << MULT_INST_RM << BP_MEM2WBK_EMPTY_SM << EXE2MEM_EMPTY_SE;
        SC_METHOD(exception);
        sensitive << WB_RD << MEM_LOAD_RD << MEM_STORE_RD << WB_RD << EXCEPTION_RD << load_adress_missaligned_se
                  << exception_se << load_access_fault_se << store_access_fault_se << store_adress_missaligned_se;
    }
};
