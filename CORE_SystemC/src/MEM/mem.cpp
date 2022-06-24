#include "mem.h"

/*
3 memory access are possible :
*  access in byte
*  access in half word
*  access in word
In every situation the adress must be aligned with the size of the access type. Meaning :
* byte      : adress must be aligned on a byte (01)
* half-word : adress must be aligned on a half-word (10)
* word      : adress must be aligned on a word (00)

If a access is made in a specific type and the adress doesnt respect the alignement it must raise an exception.

For load access :
In all the case we send a word-align adress to the ram and we do the masking
operation in mem

For store access :
* byte      : we store the least significant byte of the register in the corresponding part of the adress
* half-word : same thing but with a half word
* word      : the word is directly store inside the memory case
*/

/*
The memory receive a 32 bits adress and ignore the least 2 significant bits.
Meaning it deals with a 30 bits adress.
The Memory must be split in 4 distinct parts, each of these parts
dealing with a byte of a word-align adress.

For example :
li x1,0x1000F101
li x2,12
sb x2,0(x1)

The ram receive the adress 0x1000F101 and ignore the least 2 bits,
so it receive 0x1000F100.
The ram accesses this adress and is informed that a byte access is made
with the least 2 significant bits equal to 01
So it will store the data in the corresponding case adress with is the 2nd one.

*/
void mem::mem2wbk_concat() {
    sc_bv<mem2wbk_size> ff_din;

    ff_din.range(31, 0)   = data_sm.read();
    ff_din.range(36, 32)  = DEST_RE.read();
    ff_din[37]            = wb_sm.read();
    ff_din.range(73, 38)  = PC_EXE2MEM_RE.read();
    ff_din[74]            = CSR_WENABLE_RE.read();
    ff_din.range(106, 75) = CSR_RDATA_RE.read();
    ff_din[107]           = MULT_INST_RE.read();
    mem2wbk_din_sm.write(ff_din);
}
void mem::mem2wbk_unconcat() {
    sc_bv<mem2wbk_size> ff_dout = mem2wbk_dout_sm.read();

    MEM_RES_RM.write((sc_bv_base)ff_dout.range(31, 0));
    DEST_RM.write((sc_bv_base)ff_dout.range(36, 32));
    WB_RM.write((bool)ff_dout[37]);
    PC_MEM2WBK_RM.write((sc_bv_base)ff_dout.range(73, 38));
    CSR_WENABLE_RM.write((bool)ff_dout[74]);
    CSR_RDATA_RM.write((sc_bv_base)ff_dout.range(106, 75));
    MULT_INST_RM.write((bool)ff_dout[107]);
}

void mem::fifo_gestion() {
    bool stall = MCACHE_STALL_SM.read() || mem2wbk_full_sm.read() || EXE2MEM_EMPTY_SE.read();
    mem2wbk_push_sm.write(!stall);
    EXE2MEM_POP_SM.write(!stall);
}
void mem::mem_preprocess() {
    int         size        = MEM_SIZE_RE.read();
    bool        sign_extend = SIGN_EXTEND_RE.read();
    sc_uint<32> din         = MEM_DATA_RE.read();
    int         adr;
    sc_uint<32> dout;
    int         range_size;
    int         range_end;
    int         range_start;  // The beginning of the range of din that should actually
                              // be written to the register
    MEM_SIZE_SM = MEM_SIZE_RE;
    // Memory slot selection
    switch (EXE_RES_RE.read().range(1, 0)) {
        case 0: range_start = 0; break;
        case 1: range_start = 8; break;
        case 2: range_start = 16; break;
        case 3: range_start = 24; break;
        default: range_start = 0; break;
    }

    // Size of the data manipulate
    switch (size) {
        case 2: range_size = 7; break;   // byte
        case 1: range_size = 15; break;  // half word
        case 0: range_size = 31; break;  // word
        default: range_size = 31; break;
    }
    range_end = std::min(31, range_start + range_size);

    if (sign_extend && din(range_end, range_end)) {
        dout = 0xFFFFFFFF;
    } else {
        dout = 0;
    }

    // size = 2, range_size = 7
    // adresse : 3, range_start = 24
    // range_end = 7+24 = 31
    // din.range(7,0)
    // dout.range(31,24)
    // issue with the store value
    dout.range(range_end, range_start) = din.range(range_size, 0);

    // Whether the register bank shoudl actually write the data
    wb_sm.write(WB_RE.read() || LOAD_RE.read());

    // The data sent to the actual memory/Icache
    MCACHE_DATA_SM.write(dout);
    if (STORE_RE)
        adr = EXE_RES_RE.read();
    else
        adr = EXE_RES_RE.read() & 0xFFFFFFFC;
    MCACHE_ADR_SM.write(adr);
    MCACHE_LOAD_SM.write(LOAD_RE.read());
    MCACHE_STORE_SM.write(STORE_RE.read());
    MCACHE_ADR_VALID_SM.write(!EXE2MEM_EMPTY_SE.read());
    // MCACHE_MEM_SIZE_SM.write(MEM_SIZE_RE.read());
}

void mem::sign_extend() {
    // sign extend
    int         size        = MEM_SIZE_RE.read();
    bool        sign_extend = SIGN_EXTEND_RE.read();
    sc_uint<32> din         = MCACHE_RESULT_SM.read();
    sc_uint<32> dout;
    int         range_size;
    int         range_start;  // The beginning of the range of din that should actually
                              // be written to the register
    // MEM_SIZE = 0 -> Word
    // MEM_SIZE = 1 -> Half word
    // MEM_SIZE = 2 -> byte
    switch (EXE_RES_RE.read().range(2, 0)) {
        case 0: range_start = 0; break;
        case 1: range_start = 8; break;
        case 2: range_start = 16; break;
        case 3: range_start = 24; break;
        default: range_start = 0; break;
    }

    switch (size) {
        case 2: range_size = 7; break;
        case 1: range_size = 15; break;
        case 0: range_size = 31; break;
        default: range_size = 31; break;
    }
    int range_end = std::min(31, range_start + range_size);

    if (sign_extend && din(range_end, range_end)) {
        dout = 0xFFFFFFFF;
    } else {
        dout = 0;
    }
    dout.range(range_size, 0) = din.range(range_end, range_start);

    // Choose which data should be written to the register
    if (LOAD_RE.read()) {
        data_sm.write(dout);
    } else {
        data_sm.write(EXE_RES_RE.read());
    }
}

void mem::csr_exception() {
    EXCEPTION_SM            = EXCEPTION_RE.read() || BUS_ERROR_SX.read();
    sc_uint<32> mstatus_new = MSTATUS_RC.read();

    if (!RESET) CURRENT_MODE_SM = 3;

    if (!EXCEPTION_SM) {
        int mip            = MIP_VALUE_RC.read();
        int mie            = MIE_VALUE_RC.read();
        int trap_to_handle = mip & mie;
        if (trap_to_handle != 0) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM  = 0;
            CURRENT_MODE_SM = 3;
            EXCEPTION_SM    = true;
            if (trap_to_handle & (1 << 11)) {
                // mei
                MCAUSE_WDATA_SM.write(11 & (1 << 31));
            } else if (trap_to_handle & (1 << 3)) {
                // msi
                MCAUSE_WDATA_SM.write(3 & (1 << 31));
            } else if (trap_to_handle & (1 << 7)) {
                // mti
                MCAUSE_WDATA_SM.write(7 & (1 << 31));
            } else if (trap_to_handle & (1 << 9)) {
                // sei
                MCAUSE_WDATA_SM.write(9 & (1 << 31));
            } else if (trap_to_handle & (1 << 1)) {
                // ssi
                MCAUSE_WDATA_SM.write(1 & (1 << 31));
            } else if (trap_to_handle & (1 << 5)) {
                // sti
                MCAUSE_WDATA_SM.write(5 & (1 << 31));
            }
        } else if (CSR_WENABLE_RE.read()) {
            CSR_WADR_SM.write(CSR_WADR_SE.read());
            CSR_WDATA_SM.write(EXE_RES_RE.read());
            CSR_ENABLE_BEFORE_FIFO_SM.write(true);
        } else {
            CSR_WADR_SM.write(0);
            CSR_WDATA_SM.write(0);
            CSR_ENABLE_BEFORE_FIFO_SM.write(0);
        }
        MRET_SM = 0;
    } else {
        // Affectation of the cause
        // PLEASE DO NOT MOVE THE IF ORDER
        // THEY ARE IN A SPECIFIC ORDER
        // TO RESPECT PRIORITY IN CASE OF SEVERAL
        // EXCEPTION OCCURING AT THE SAME CYCLE
        bool machine_interrupt_enable = mstatus_new[3];
        if (BUS_ERROR_SX) {       // load access fault
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            // MCAUSE_WDATA_SM.write(5);
        } else if (ENV_CALL_WRONG_MODE_RE) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = 0;
            MCAUSE_WDATA_SM.write(24);
            CURRENT_MODE_SM = 3;
        } else if (MRET_RE) {
            save_restore_sm = 0;
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 1;

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            CURRENT_MODE_SM = 0;  // Retrun in user mode

            // loading return value (main) from EPC to PC :
            // The adress will be send to ifetch

            RETURN_ADRESS_SM = MEPC_SC;

            // Informing IFETCH that a return instruction have been received

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = 0;
            MRET_SM        = MRET_RE;
        } else if (STORE_ACCESS_FAULT_RE) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = EXE_RES_RE;
            MCAUSE_WDATA_SM.write(7);
            CURRENT_MODE_SM = 3;
        } else if (LOAD_ACCESS_FAULT_RE) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = EXE_RES_RE;
            MCAUSE_WDATA_SM.write(5);
            CURRENT_MODE_SM = 3;
        } else if (STORE_ADRESS_MISSALIGNED_RE) {
            save_restore_sm = 0;
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = EXE_RES_RE;
            MCAUSE_WDATA_SM.write(6);
            CURRENT_MODE_SM = 3;
        } else if (LOAD_ADRESS_MISSALIGNED_RE) {
            save_restore_sm = 0;
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = EXE_RES_RE;
            MCAUSE_WDATA_SM.write(4);
            CURRENT_MODE_SM = 3;
        } else if (ENV_CALL_M_MODE_RE) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = 0;
            MCAUSE_WDATA_SM.write(11);
            CURRENT_MODE_SM = 3;
        } else if (ENV_CALL_S_MODE_RE) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = 0;
            MCAUSE_WDATA_SM.write(9);
            CURRENT_MODE_SM = 3;
        } else if (ENV_CALL_U_MODE_RE) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = 0;
            MCAUSE_WDATA_SM.write(8);
            CURRENT_MODE_SM = 3;
        } else if (EBREAK_RE) {
            save_restore_sm = 1;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = PC_EXE2MEM_RE;
            MCAUSE_WDATA_SM.write(3);
            CURRENT_MODE_SM = 3;
        } else if (INSTRUCTION_ADRESS_MISSALIGNED_RE) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = PC_BRANCH_VALUE_RE;
            MCAUSE_WDATA_SM.write(0);
            CURRENT_MODE_SM = 3;
        } else if (ILLEGAL_INSTRUCTION_RE) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = 0;
            MCAUSE_WDATA_SM.write(2);
            CURRENT_MODE_SM = 3;
        } else if (INSTRUCTION_ACCESS_FAULT_RE) {
            save_restore_sm = 0;  // Need to save context
            mpp_sm          = CURRENT_MODE_SM;
            mpie_sm         = mstatus_new[3];  // reading precedent value of MIE
            mie_sm          = 0;               // No interruption during exception gestion

            mstatus_new[31]           = save_restore_sm;
            mstatus_new.range(12, 11) = mpp_sm;
            mstatus_new[7]            = mpie_sm;
            mstatus_new[3]            = mie_sm;
            MSTATUS_WDATA_RM          = mstatus_new;

            MEPC_WDATA_RM.write(PC_EXE2MEM_RE.read());
            MTVAL_WDATA_SM = 0;
            MCAUSE_WDATA_SM.write(1);
            CURRENT_MODE_SM = 3;
        }
        if (!MRET_RE.read()) MRET_SM = 0;
    }
}

void mem::trace(sc_trace_file* tf) {
    sc_trace(tf, MCACHE_ADR_SM, GET_NAME(MCACHE_ADR_SM));  // adress in memory
    sc_trace(tf, MCACHE_DATA_SM, GET_NAME(MCACHE_DATA_SM));
    sc_trace(tf, MCACHE_ADR_VALID_SM, GET_NAME(MCACHE_ADR_VALID_SM));
    sc_trace(tf, MCACHE_STORE_SM, GET_NAME(MCACHE_STORE_SM));
    sc_trace(tf, MCACHE_LOAD_SM, GET_NAME(MCACHE_LOAD_SM));
    sc_trace(tf, MCACHE_RESULT_SM, GET_NAME(MCACHE_RESULT_SM));
    sc_trace(tf, MCACHE_STALL_SM, GET_NAME(MCACHE_STALL_SM));
    sc_trace(tf, EXE_RES_RE, GET_NAME(EXE_RES_RE));
    sc_trace(tf, MEM_DATA_RE, GET_NAME(MEM_DATA_RE));
    sc_trace(tf, DEST_RE, GET_NAME(DEST_RE));
    sc_trace(tf, MEM_SIZE_RE, GET_NAME(MEM_SIZE_RE));
    sc_trace(tf, WB_RE, GET_NAME(WB_RE));
    sc_trace(tf, SIGN_EXTEND_RE, GET_NAME(SIGN_EXTEND_RE));
    sc_trace(tf, LOAD_RE, GET_NAME(LOAD_RE));
    sc_trace(tf, STORE_RE, GET_NAME(STORE_RE));
    sc_trace(tf, EXE2MEM_EMPTY_SE, GET_NAME(EXE2MEM_EMPTY_SE));
    sc_trace(tf, EXE2MEM_POP_SM, GET_NAME(EXE2MEM_POP_SM));
    sc_trace(tf, MEM2WBK_POP_SW, GET_NAME(MEM2WBK_POP_SW));
    sc_trace(tf, mem2wbk_push_sm, GET_NAME(mem2wbk_push_sm));
    sc_trace(tf, mem2wbk_full_sm, GET_NAME(mem2wbk_full_sm));
    sc_trace(tf, MEM2WBK_EMPTY_SM, GET_NAME(MEM2WBK_EMPTY_SM));
    sc_trace(tf, MEM_RES_RM, GET_NAME(MEM_RES_RM));
    sc_trace(tf, DEST_RM, GET_NAME(DEST_RM));
    sc_trace(tf, WB_RM, GET_NAME(WB_RM));
    sc_trace(tf, mem2wbk_din_sm, GET_NAME(mem2wbk_din_sm));
    sc_trace(tf, mem2wbk_dout_sm, GET_NAME(mem2wbk_dout_sm));
    sc_trace(tf, data_sm, GET_NAME(data_sm));
    sc_trace(tf, wb_sm, GET_NAME(wb_sm));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, CSR_WENABLE_RE, GET_NAME(CSR_WENABLE_RE));
    sc_trace(tf, CSR_WADR_SE, GET_NAME(CSR_WADR_SE));
    sc_trace(tf, CSR_RDATA_RE, GET_NAME(CSR_RDATA_RE));
    sc_trace(tf, CSR_WENABLE_RM, GET_NAME(CSR_WENABLE_RM));
    sc_trace(tf, CSR_RDATA_RM, GET_NAME(CSR_RDATA_RM));
    sc_trace(tf, INTERRUPTION_SE, GET_NAME(INTERRUPTION_SE));
    sc_trace(tf, CSR_WADR_SM, GET_NAME(CSR_WADR_SM));
    sc_trace(tf, CSR_WDATA_SM, GET_NAME(CSR_WDATA_SM));
    sc_trace(tf, EXCEPTION_RE, GET_NAME(EXCEPTION_RE));
    sc_trace(
        tf, LOAD_ADRESS_MISSALIGNED_RE, GET_NAME(LOAD_ADRESS_MISSALIGNED_RE));  // adress from store/load isn't aligned
    sc_trace(tf, LOAD_ACCESS_FAULT_RE,
             GET_NAME(LOAD_ACCESS_FAULT_RE));  // trying to access memory in wrong mode
    sc_trace(tf, ENV_CALL_U_MODE_RE, GET_NAME(ENV_CALL_U_MODE_RE));
    sc_trace(tf, ENV_CALL_WRONG_MODE_RE, GET_NAME(ENV_CALL_WRONG_MODE_RE));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RE, GET_NAME(ILLEGAL_INSTRUCTION_RE));  // accessing stuff in wrong mode
    sc_trace(tf,
             INSTRUCTION_ADRESS_MISSALIGNED_RE,
             GET_NAME(INSTRUCTION_ADRESS_MISSALIGNED_RE));  // branch offset is misaligned
    sc_trace(tf, ENV_CALL_S_MODE_RE, GET_NAME(ENV_CALL_S_MODE_RE));
    sc_trace(tf, ENV_CALL_M_MODE_RE, GET_NAME(ENV_CALL_M_MODE_RE));
    sc_trace(tf, BUS_ERROR_SX, GET_NAME(BUS_ERROR_SX));
    sc_trace(tf, EXCEPTION_SM, GET_NAME(EXCEPTION_SM));
    sc_trace(tf, MSTATUS_WDATA_RM, GET_NAME(MSTATUS_WDATA_RM));
    sc_trace(tf, MIP_WDATA_RM, GET_NAME(MIP_WDATA_RM));
    sc_trace(tf, MEPC_WDATA_RM, GET_NAME(MEPC_WDATA_RM));
    sc_trace(tf, MCAUSE_WDATA_SM, GET_NAME(MCAUSE_WDATA_SM));
    sc_trace(tf, MIP_VALUE_RC, GET_NAME(MIP_VALUE_RC));
    sc_trace(tf, CSR_ENABLE_BEFORE_FIFO_SM, GET_NAME(CSR_ENABLE_BEFORE_FIFO_SM));
    sc_trace(tf, exception_sm, GET_NAME(exception_sm));
    sc_trace(tf, MULT_INST_RM, GET_NAME(MULT_INST_RM));
    // sc_trace(tf, MCACHE_MEM_SIZE_SM, GET_NAME(MCACHE_MEM_SIZE_SM));
    sc_trace(tf, CURRENT_MODE_SM, GET_NAME(CURRENT_MODE_SM));
    sc_trace(tf, MRET_RE, GET_NAME(MRET_RE));
    sc_trace(tf, MRET_SM, GET_NAME(MRET_SM));
    sc_trace(tf, RETURN_ADRESS_SM, GET_NAME(RETURN_ADRESS_SM));
    sc_trace(tf, mret_sm, GET_NAME(mret_sm));
    sc_trace(tf, MEM_SIZE_SM, GET_NAME(MEM_SIZE_SM));
    sc_trace(tf, PC_BRANCH_VALUE_RE, GET_NAME(PC_BRANCH_VALUE_RE));
    sc_trace(tf, STORE_ACCESS_FAULT_RE, GET_NAME(STORE_ACCESS_FAULT_RE));
    sc_trace(tf, STORE_ADRESS_MISSALIGNED_RE, GET_NAME(STORE_ADRESS_MISSALIGNED_RE));
    sc_trace(tf, INSTRUCTION_ACCESS_FAULT_RE, GET_NAME(INSTRUCTION_ACCESS_FAULT_RE));
    fifo_inst.trace(tf);
}