#include "icache.h"


void icache::parse_adr()
{
    sc_uint<32> DT_A_SI = ADR_SI.read();
    address_tag.write(DT_A_SI.range(31,12));
    address_index.write(DT_A_SI.range(11,4));
    address_offset.write(DT_A_SI.range(3,0));
}

void icache::miss_detection()
{

    // COMPARE HIT WAY0
    if(address_tag == tag[address_index.read()] && data_validate[address_index.read()])
    {   
        hit = true;       
        IC_STALL_SI.write(false); 
        IC_INST_SI.write(data[address_index.read()][(address_offset.read()/4)]);
    }
    else{
        hit = false;
        IC_STALL_SI.write(true);
    
    }

}

void icache::transition()
{
    typedef enum // MAE STATES
    {
        IDLE,
        WAIT_MEM,
        UPDT,
        ERROR
    } states_fsm;

    states_fsm fsm_current_state = IDLE;

    int cpt = 0;
    bool dta_valid;
    sc_uint<8> current_address_index;
    sc_uint<20>  current_address_tag;
    bool adr_valid;

    bool debug = true;
    while(1)
    {
        dta_valid = false;

        switch (fsm_current_state)
        {
            case IDLE:
                if(ADR_VALID_SI.read() & RESET_N.read())
                {
                    if(!hit)
                    {
                        fsm_current_state = WAIT_MEM;
                        A.write(ADR_SI.read() & 0xFFFFFFF0);
                        current_address_index = address_index;
                        current_address_tag = address_tag;
                        dta_valid = true;
                        cpt = 0;
                        
                    }
                }
                break;
            case WAIT_MEM:
                if(SLAVE_ACK_SP.read())
                {
                    fsm_current_state = UPDT;

                    data[current_address_index][cpt++] = DT.read();
                    tag[current_address_index] = current_address_tag;
                    data_validate[current_address_index] = false;
                    

                }
            break;
            case UPDT:
                if(!SLAVE_ACK_SP.read())
                {
                    fsm_current_state = IDLE;
                    data_validate[current_address_index] = true;
                }
                else
                {
                    data[current_address_index][cpt++] = DT.read();
                }
            break;
            default:
            break;
        }

        switch(fsm_current_state)
        {
          case IDLE:
            fsm_state.write(0);
          break;
          case WAIT_MEM:
            fsm_state.write(1);
          break;
          case UPDT:
            fsm_state.write(2);
          break;
          case ERROR:
            fsm_state.write(3);
          break;
        }

        DTA_VALID.write(dta_valid);
        wait();
    }
}

void icache::trace(sc_trace_file* tf)
{
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));

    sc_trace(tf, ADR_SI, GET_NAME(ADR_SI)); 
    sc_trace(tf, ADR_VALID_SI, GET_NAME(ADR_VALID_SI)); 

    sc_trace(tf, IC_INST_SI, GET_NAME(IC_INST_SI));
    sc_trace(tf, IC_STALL_SI, GET_NAME(IC_STALL_SI));

    sc_trace(tf, DT, GET_NAME(DT));
    sc_trace(tf, A, GET_NAME(A));
    sc_trace(tf, DTA_VALID, GET_NAME(DTA_VALID));

    sc_trace(tf, SLAVE_ACK_SP, GET_NAME(SLAVE_ACK_SP));

    sc_trace(tf, hit, GET_NAME(hit));

    sc_trace(tf, address_tag, GET_NAME(address_tag));
    sc_trace(tf, address_index, GET_NAME(address_index));
    sc_trace(tf, address_offset, GET_NAME(address_offset));

    sc_trace(tf, fsm_state, GET_NAME(fsm_state));

    for (int i = 0; i < 256; i++) {
        std::string icname = "ICACHE_";
        for(int j = 0; j < 4; j++)
        {
            icname = "D_I_" + std::to_string(i) + "_O_" + std::to_string(j);
            sc_trace(tf, data[i][j], signal_get_name(data[i][j].name(), icname.c_str()));
        }
        icname = "ICACHE_TAG_" + std::to_string(i);
        sc_trace(tf, tag[i], signal_get_name(tag[i].name(), icname.c_str()));
        icname = "ICACHE_VALID_" + std::to_string(i);
        sc_trace(tf, data_validate[i], signal_get_name(data_validate[i].name(), icname.c_str()));
    }
}