#include "core.h"

void core::trace(sc_trace_file* tf) {
    sc_trace(tf, PC_INIT, GET_NAME(PC_INIT));
    sc_trace(tf, MEM_SIZE_SM, GET_NAME(MEM_SIZE_SM));
    ifetch_inst.trace(tf);
    dec_inst.trace(tf);
    exec_inst.trace(tf);
    mem_inst.trace(tf);
    wbk_inst.trace(tf);
    reg_inst.trace(tf);
    csr_inst.trace(tf);
    timer_inst.trace(tf);
    x0_multiplier_inst.trace(tf);
    x1_multiplier_inst.trace(tf);
    x2_multiplier_inst.trace(tf);
}

void core::core_method() {
    DEBUG_PC_READ.write(READ_PC_SR.read());
}