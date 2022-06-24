#pragma once
#include <systemc.h>
#include <iostream>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"
#define mem2wbk_size 108
/*
Assuming the following stuff :

MEM_SIZE = 00 -> access in word
MEM_SIZE = 01 -> access in half word
MEM_SIZE = 10  -> acces in byte

*/
SC_MODULE(mem) {
    // Mcache Interface :

    sc_out<sc_uint<32>> MCACHE_ADR_SM;  // adress in memory
    sc_out<sc_uint<32>> MCACHE_DATA_SM;
    sc_out<bool>        MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM;

    sc_in<sc_uint<32>> MCACHE_RESULT_SM;
    sc_in<bool>        MCACHE_STALL_SM;
    sc_out<sc_uint<2>> MEM_SIZE_SM;
    // Exe Interface :

    sc_in<sc_uint<32>> EXE_RES_RE;
    sc_in<sc_uint<32>> MEM_DATA_RE;
    sc_in<sc_uint<6>>  DEST_RE;
    sc_in<sc_uint<2>>  MEM_SIZE_RE;

    sc_in<sc_uint<32>> PC_EXE2MEM_RE;
    sc_in<bool>        WB_RE;
    sc_in<bool>        SIGN_EXTEND_RE;     // taille fifo entrée : 74
    sc_in<bool>        LOAD_RE, STORE_RE;  // 15
    sc_in<bool>        MULT_INST_RE;

    sc_in<bool>        CSR_WENABLE_RE;
    sc_in<sc_uint<12>> CSR_WADR_SE;
    sc_in<sc_uint<32>> CSR_RDATA_RE;

    sc_in<bool> EXCEPTION_RE;
    sc_in<bool> LOAD_ADRESS_MISSALIGNED_RE;         // adress from store/load isn't aligned
    sc_in<bool> LOAD_ACCESS_FAULT_RE;               // trying to access memory in wrong mode
    sc_in<bool> ILLEGAL_INSTRUCTION_RE;             // accessing stuff in wrong mode
    sc_in<bool> INSTRUCTION_ADRESS_MISSALIGNED_RE;  // branch offset is misaligned
    sc_in<bool> STORE_ADRESS_MISSALIGNED_RE;
    sc_in<bool> STORE_ACCESS_FAULT_RE;
    sc_in<bool> ENV_CALL_S_MODE_RE;
    sc_in<bool> ENV_CALL_M_MODE_RE;
    sc_in<bool> ENV_CALL_U_MODE_RE;
    sc_in<bool> ENV_CALL_WRONG_MODE_RE;
    sc_in<bool> INSTRUCTION_ACCESS_FAULT_RE;
    sc_in<bool> MRET_RE;  // 31

    sc_in<sc_uint<32>> PC_BRANCH_VALUE_RE;

    // Bus Interface : // No bus in our implemation but can be use for further use

    sc_in<bool> BUS_ERROR_SX;

    // exe2mem interface :

    sc_in<bool>  EXE2MEM_EMPTY_SE;
    sc_out<bool> EXE2MEM_POP_SM;

    // mem2wbk interface

    sc_in<bool>         MEM2WBK_POP_SW;  // 35
    sc_signal<bool>     mem2wbk_push_sm;
    sc_signal<bool>     mem2wbk_full_sm;
    sc_out<bool>        MEM2WBK_EMPTY_SM;
    sc_out<sc_uint<32>> PC_MEM2WBK_RM;
    sc_out<bool>        CSR_WENABLE_RM;  // 38

    // WBK interface
    sc_out<sc_uint<32>> MEM_RES_RM;
    sc_out<sc_uint<6>>  DEST_RM;
    sc_out<bool>        WB_RM;
    sc_out<sc_uint<32>> CSR_RDATA_RM;
    sc_out<bool>        MULT_INST_RM;
    // Global Interface :

    sc_out<bool>        EXCEPTION_SM;
    sc_out<sc_uint<2>>  CURRENT_MODE_SM;
    sc_out<sc_uint<32>> RETURN_ADRESS_SM;
    sc_out<bool>        MRET_SM;  // 46

    sc_in_clk   CLK;
    sc_in<bool> RESET;

    // Interruption :

    sc_in<bool> INTERRUPTION_SE;
    sc_in<bool> MACHINE_SOFTWARE_INTERRUPT_SE;
    sc_in<bool> MACHINE_TIMER_INTERRUPT_SE;
    sc_in<bool> MACHINE_EXTERNAL_INTERRUPT_SE;
    sc_in<bool> EBREAK_RE;
    // CSR Interface :

    sc_out<sc_uint<12>> CSR_WADR_SM;
    sc_out<sc_uint<32>> CSR_WDATA_SM;
    sc_out<bool>        CSR_ENABLE_BEFORE_FIFO_SM;

    sc_out<sc_uint<32>> MSTATUS_WDATA_RM;
    sc_out<sc_uint<32>> MIP_WDATA_RM;
    sc_out<sc_uint<32>> MEPC_WDATA_RM;
    sc_out<sc_uint<32>> MCAUSE_WDATA_SM;
    sc_out<sc_uint<32>> MTVAL_WDATA_SM;
    sc_in<sc_uint<32>>  MEPC_SC;
    sc_in<sc_uint<32>>  MSTATUS_RC;  // 57
    sc_in<sc_uint<32>>  MTVEC_VALUE_RC;
    sc_in<sc_uint<32>>  MIP_VALUE_RC;
    sc_in<sc_uint<32>>  MIE_VALUE_RC;

    // Internal signals

    sc_signal<sc_bv<mem2wbk_size>> mem2wbk_din_sm;
    sc_signal<sc_bv<mem2wbk_size>> mem2wbk_dout_sm;
    sc_signal<bool>                exception_sm;
    sc_signal<sc_uint<32>>         data_sm;
    sc_signal<bool>                wb_sm;

    // Exception signals

    sc_signal<bool>       save_restore_sm;  // sd bit from mstatus
    sc_signal<sc_uint<2>> mpp_sm;           // MPP bit from mstatus
    sc_signal<bool>       mpie_sm;          // MPIE bit from mstatus
    sc_signal<bool>       mie_sm;           // MIE bit from mstatus

    sc_signal<sc_uint<32>> return_adress_sm;
    sc_signal<bool>        mret_sm;

    // FIFO
    fifo<mem2wbk_size> fifo_inst;

    void mem2wbk_concat();
    void mem2wbk_unconcat();
    void fifo_gestion();
    void mem_preprocess();
    void sign_extend();
    void interruption();
    void csr_exception();
    void trace(sc_trace_file * tf);

    SC_CTOR(mem) : fifo_inst("mem2wbk") {
        fifo_inst.DIN_S(mem2wbk_din_sm);
        fifo_inst.DOUT_R(mem2wbk_dout_sm);
        fifo_inst.EMPTY_S(MEM2WBK_EMPTY_SM);
        fifo_inst.FULL_S(mem2wbk_full_sm);
        fifo_inst.PUSH_S(mem2wbk_push_sm);
        fifo_inst.POP_S(MEM2WBK_POP_SW);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(mem2wbk_concat);
        sensitive << data_sm << DEST_RE << wb_sm << CSR_WENABLE_RE << CSR_RDATA_RE << exception_sm << mret_sm
                  << return_adress_sm;
        SC_METHOD(mem2wbk_unconcat);
        sensitive << mem2wbk_dout_sm;
        SC_METHOD(fifo_gestion);
        sensitive << MCACHE_STALL_SM << mem2wbk_full_sm << EXE2MEM_EMPTY_SE << wb_sm;
        SC_METHOD(mem_preprocess);
        sensitive << WB_RE << LOAD_RE << MEM_SIZE_RE << MCACHE_RESULT_SM << EXE_RES_RE << MEM_DATA_RE << STORE_RE
                  << SIGN_EXTEND_RE << EXE2MEM_EMPTY_SE;
        SC_METHOD(sign_extend);
        sensitive << MEM_SIZE_RE << SIGN_EXTEND_RE << MCACHE_RESULT_SM << EXE_RES_RE << LOAD_RE;
        SC_METHOD(csr_exception);
        sensitive << EXCEPTION_RE << CSR_WENABLE_RE << MIP_VALUE_RC << PC_EXE2MEM_RE << CSR_WADR_SE << EXE_RES_RE
                  << INSTRUCTION_ACCESS_FAULT_RE << ILLEGAL_INSTRUCTION_RE << INSTRUCTION_ADRESS_MISSALIGNED_RE
                  << ENV_CALL_U_MODE_RE << ENV_CALL_S_MODE_RE << ENV_CALL_M_MODE_RE << LOAD_ADRESS_MISSALIGNED_RE
                  << STORE_ADRESS_MISSALIGNED_RE << LOAD_ACCESS_FAULT_RE << STORE_ACCESS_FAULT_RE << MRET_RE
                  << EXCEPTION_SM << ENV_CALL_WRONG_MODE_RE << BUS_ERROR_SX << EXCEPTION_SM << RESET << MSTATUS_RC
                  << EXE_RES_RE << MEPC_SC;
    }
};
