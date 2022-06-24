#include "ifetch.h"

void ifetch::fetch_method() {
    sc_bv<if2dec_size> if2dec_in_var;
    sc_bv<if2dec_size> instr_ri_var = instr_ri.read();
    if (EXCEPTION_SM.read() == 0) {
        ADR_SI.write(PC_RD.read());

        // data sent in if2dec


        if2dec_in_var.range(63, 32) = (sc_bv_base)IC_INST_SI.read();
        if2dec_in_var.range(31, 0)  = (sc_bv_base)PC_RD.read();
        if2dec_in_si.write(if2dec_in_var);

        // data coming out from if2dec :

        INSTR_RI.write((sc_bv_base)instr_ri_var.range(63, 32));
        PC_IF2DEC_RI.write((sc_bv_base)instr_ri_var.range(31, 0));
        if (IF2DEC_FLUSH_SD.read()) {
            IF2DEC_PUSH_SI.write(false);
            DEC2IF_POP_SI.write(true);
            ADR_VALID_SI.write(false);
        } else {
            // stall if the memory stalls, if we can't push to dec, or have no value
            // of pc to pop from dec
            bool stall = IC_STALL_SI.read() || IF2DEC_FULL_SI.read() || DEC2IF_EMPTY_SI.read() ;
            IF2DEC_PUSH_SI.write(!stall);
            DEC2IF_POP_SI.write(!stall);
            ADR_VALID_SI.write(!DEC2IF_EMPTY_SI.read());
        }

    } else {
        // data sent in if2dec
        // If an exception is detected
        // Pipeline pass in M-mode
        // Fifo send nop instruction 

        if2dec_in_var.range(63, 32) = nop_encoding;

        if2dec_in_var.range(31, 0)  = (sc_bv_base)PC_RD.read();
        ADR_SI.write(PC_RD.read());


        if2dec_in_si.write(if2dec_in_var);  
        
        // data coming out from if2dec :

        INSTR_RI.write((sc_bv_base)instr_ri_var.range(63, 32));
        PC_IF2DEC_RI.write((sc_bv_base)instr_ri_var.range(31, 0));
        IF2DEC_PUSH_SI.write(true);
        DEC2IF_POP_SI.write(true);
        ADR_VALID_SI.write(false);
    }

}


void ifetch::exception()  
// No exception in IFECTH
// Gestion of current mode, start in Machine mode
// Then keep the same mode as the pipeline 
{
    EXCEPTION_RI.write(0);
}


void ifetch::trace(sc_trace_file* tf) {
    sc_trace(tf, ADR_SI, GET_NAME(ADR_SI));
    sc_trace(tf, ADR_VALID_SI, GET_NAME(ADR_VALID_SI));
    sc_trace(tf, IC_INST_SI, GET_NAME(IC_INST_SI));
    sc_trace(tf, IC_STALL_SI, GET_NAME(IC_STALL_SI));
    sc_trace(tf, DEC2IF_EMPTY_SI, GET_NAME(DEC2IF_EMPTY_SI));
    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));
    sc_trace(tf, IF2DEC_POP_SD, GET_NAME(IF2DEC_POP_SD));
    sc_trace(tf, IF2DEC_PUSH_SI, GET_NAME(IF2DEC_PUSH_SI));
    sc_trace(tf, IF2DEC_FULL_SI, GET_NAME(IF2DEC_FULL_SI));
    sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
    sc_trace(tf, PC_RD, GET_NAME(PC_RD));
    sc_trace(tf, INSTR_RI, GET_NAME(INSTR_RI));
    sc_trace(tf, PC_IF2DEC_RI, GET_NAME(PC_IF2DEC_RI));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, if2dec_in_si, GET_NAME(if2dec_in_si));
    sc_trace(tf, instr_ri, GET_NAME(instr_ri));
    sc_trace(tf, EXCEPTION_RI, GET_NAME(EXCEPTION_RI));
    sc_trace(tf, EXCEPTION_SM, GET_NAME(EXCEPTION_SM));
    sc_trace(tf, INTERRUPTION_SE, GET_NAME(INTERRUPTION_SE));
    sc_trace(tf, MRET_SM, GET_NAME(MRET_SM));
    fifo_inst.trace(tf);
}