#include "ifetch.h"

void ifetch::fetch_method() {
    sc_bv<if2dec_size> if2dec_in_var;
    sc_bv<if2dec_size> instr_ri_var = instr_ri.read();
    if (EXCEPTION_SM.read() == 0) {


        if(PRED_TAKEN_RI.read() && !PRED_FAILED_RD.read())
        {
            ADR_SI.write(PRED_ADR_RI.read());
        }
        else
        {
            ADR_SI.write(PC_RD.read());
        }

        // data sent in if2dec
        if2dec_in_var[96] = PRED_ADR_TAKEN_SI.read();
        if2dec_in_var.range(95, 64) = PRED_NEXT_ADR_SI.read();
        if2dec_in_var.range(63, 32) = (sc_bv_base)IC_INST_SI.read();
        if2dec_in_var.range(31, 0)  = (PRED_TAKEN_RI.read() && !PRED_FAILED_RD.read())?PRED_ADR_RI.read():PC_RD.read();
        if2dec_in_si.write(if2dec_in_var);

        // data coming out from if2dec :
        PRED_TAKEN_RI.write((bool)instr_ri_var[96]);
        PRED_ADR_RI.write((sc_bv_base)instr_ri_var.range(95, 64));
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
        if2dec_in_var[96] = PRED_ADR_TAKEN_SI.read();
        if2dec_in_var.range(95, 64) = PRED_NEXT_ADR_SI.read();
        if2dec_in_var.range(63, 32) = nop_encoding;
        if2dec_in_var.range(31, 0)  = (PRED_TAKEN_RI.read() && !PRED_FAILED_RD.read())?PRED_ADR_RI.read():PC_RD.read();
        if(PRED_TAKEN_RI.read() && !PRED_FAILED_RD.read())
        {
            ADR_SI.write(PRED_ADR_RI.read());
        }
        else
        {
            ADR_SI.write(PC_RD.read());
        }


        if2dec_in_si.write(if2dec_in_var);  
        
        // data coming out from if2dec :
        PRED_TAKEN_RI.write((bool)instr_ri_var[96]);
        PRED_ADR_RI.write((sc_bv_base)instr_ri_var.range(95, 64));
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
void ifetch::write_pred_reg()
{
    int index = 0;
    if(BRANCH_INST_RD.read() && !IF2DEC_EMPTY_SI.read())
    {
        bool found = false;
        for(int i = 0; i < predictor_register_size; ++i)
        {
            if(BRANCH_ADR_RI[i] == BRANCH_INST_ADR_RD.read())
            {
                found = true;
                index = i;
                break;
            }
        }
        if(!found)
        {
            BRANCH_ADR_RI[pred_write_pointer_si.read()] = BRANCH_INST_ADR_RD.read();
            PREDICTED_ADR_RI[pred_write_pointer_si.read()] = ADR_TO_BRANCH_RD.read();
            PRED_STATE_RI[pred_write_pointer_si.read()] = weakly_taken;
            pred_write_pointer_si = pred_write_pointer_si.read() + 1;
        }
        else
        {
            PRED_STATE_RI[index] = next_state_pred_si;
        }
    }
}
void ifetch::read_pred_reg()
{
    bool found = false;
    for(int i = 0; i < predictor_register_size; ++i)
    {
        if(BRANCH_ADR_RI[i].read() == PC_RD.read())
        {
            #ifdef BRANCH_PREDICTION
            found = true;
            #endif
            pred_branch_next_adr_si = PREDICTED_ADR_RI[i];
            pred_branch_taken_si = (bool)(PRED_STATE_RI[i].read() == strongly_taken) || (PRED_STATE_RI[i].read() == weakly_taken);
            break;
        }
    }
    if(!found)
    {
        pred_branch_taken_si = false;
    }
}
void ifetch::calc_prob_pred()
{
    for(int i = 0; i < predictor_register_size; ++i)
    {
        if(BRANCH_ADR_RI[i].read() & BRANCH_INST_ADR_RD.read())
        {
            switch(PRED_STATE_RI[i].read())
            {
                case strongly_taken:
                    next_state_pred_si = PRED_SUCCESS_RD.read()?strongly_taken:weakly_taken;
                break;
                case weakly_taken:
                    next_state_pred_si = PRED_SUCCESS_RD.read()?strongly_taken:weakly_not_taken;
                break;
                case weakly_not_taken:
                    next_state_pred_si = PRED_SUCCESS_RD.read()?weakly_taken:strongly_not_taken;
                break;
                case strongly_not_taken:
                    next_state_pred_si = PRED_SUCCESS_RD.read()?weakly_not_taken:strongly_not_taken;
                break;
            }
            break;
        }
    }
}
void ifetch::write_pred_ret_reg()
{
    int index = 0;
    if(RET_INST_RD.read() && !IF2DEC_EMPTY_SI.read())
    {
        bool found = false;
        for(int i = 0; i < ret_predictor_register_size; ++i)
        {
            if(RET_ADR_RI[i] == RET_INST_ADR_RD.read())
            {
                found = true;
                index = i;
                break;
            }
        }
        if(!found)
        {
            RET_ADR_RI[ret_write_pointer_si.read()] = RET_INST_ADR_RD.read();

            ret_write_pointer_si = ret_write_pointer_si.read() + 1;
        }
    }
}
void ifetch::write_ret_stack()
{
    int index = 0;
    if(VALID_ADR_TO_RET_RD.read() && !IF2DEC_EMPTY_SI.read())
    {
        RET_STACK_RI[ret_stack_pointer_si.read()] = ADR_TO_RET_RD.read();

        ret_stack_pointer_si = ret_stack_pointer_si.read() + 1;
    }
}
void ifetch::read_pred_ret_reg()
{
    bool found = false;
    for(int i = 0; i < predictor_register_size; ++i)
    {
        if(RET_ADR_RI[i].read() == PC_RD.read())
        {
            #ifdef BRANCH_PREDICTION
            found = true;
            #endif
            pred_ret_next_adr_si = PREDICTED_ADR_RI[i];
            break;
        }
    }
    pred_ret_taken_si = found;
}
void ifetch::next_pred_adr()
{
    if(pred_branch_taken_si)
        PRED_NEXT_ADR_SI.write(pred_branch_next_adr_si);
    else if(pred_ret_taken_si)
        PRED_NEXT_ADR_SI.write(pred_ret_next_adr_si);

    PRED_ADR_TAKEN_SI = pred_branch_taken_si | pred_ret_taken_si;
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
    sc_trace(tf, BRANCH_INST_RD, GET_NAME(BRANCH_INST_RD));
    sc_trace(tf, BRANCH_INST_ADR_RD, GET_NAME(BRANCH_INST_ADR_RD));
    sc_trace(tf, ADR_TO_BRANCH_RD, GET_NAME(ADR_TO_BRANCH_RD));
    sc_trace(tf, PRED_ADR_RI, GET_NAME(PRED_ADR_RI));
    sc_trace(tf, PRED_TAKEN_RI, GET_NAME(PRED_TAKEN_RI));
    sc_trace(tf, PRED_NEXT_ADR_SI, GET_NAME(PRED_NEXT_ADR_SI));
    sc_trace(tf, PRED_ADR_TAKEN_SI, GET_NAME(PRED_ADR_TAKEN_SI));
    sc_trace(tf, pred_write_pointer_si, GET_NAME(pred_write_pointer_si));
    sc_trace(tf, PRED_SUCCESS_RD, GET_NAME(PRED_SUCCESS_RD));
    sc_trace(tf, next_state_pred_si, GET_NAME(next_state_pred_si));
    sc_trace(tf, PRED_FAILED_RD, GET_NAME(PRED_FAILED_RD));

    for(int i = 0; i < predictor_register_size; i++)
    {
        std::string regname = "REG_ADR_";
        regname += std::to_string(i);
        sc_trace(tf, BRANCH_ADR_RI[i], signal_get_name(BRANCH_ADR_RI[i].name(), regname.c_str()));

        regname = "REG_PRED_";
        regname += std::to_string(i);
        sc_trace(tf, PREDICTED_ADR_RI[i], signal_get_name(PREDICTED_ADR_RI[i].name(), regname.c_str()));

        regname = "REG_STATE_";
        regname += std::to_string(i);
        sc_trace(tf, PRED_STATE_RI[i], signal_get_name(PRED_STATE_RI[i].name(), regname.c_str()));
    }
    
    fifo_inst.trace(tf);
}