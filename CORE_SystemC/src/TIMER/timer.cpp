#include "timer.h"

void timer::thread() {
    config_rt  = 0;
    divider_rt = 1;
    TIME_RT    = 0;
    counter_rt = 0;

    while (1) {
        if (TIMER_CONFIG_WB_SC.read()) config_rt = DATA_SC.read().range(1, 0);
        if (TIMER_DIVIDER_WB_SC.read()) divider_rt = DATA_SC;
        TIME_RT      = TIME_RT.read() + 1LL;
        TIMER_INT_ST = 0;
        if (config_rt.read()[0]) {
            if (counter_rt == divider_rt) {
                if (ACK_SP.read()) {
                    counter_rt = 0;
                    if (!config_rt.read()[1]) config_rt.write(0);
                } else {
                    TIMER_INT_ST = 1;
                }
            } else {
                counter_rt = counter_rt.read() + 1;
            }
        }
        wait(1);
    }
}

void timer::trace(sc_trace_file* tf) {
    sc_trace(tf, TIMER_CONFIG_WB_SC, GET_NAME(TIMER_CONFIG_WB_SC));
    sc_trace(tf, TIMER_DIVIDER_WB_SC, GET_NAME(TIMER_DIVIDER_WB_SC));
    sc_trace(tf, DATA_SC, GET_NAME(DATA_SC));
    sc_trace(tf, TIME_RT, GET_NAME(TIME_RT));
    sc_trace(tf, TIMER_INT_ST, GET_NAME(TIMER_INT_ST));
    sc_trace(tf, ACK_SP, GET_NAME(ACK_SP));
    sc_trace(tf, counter_rt, GET_NAME(counter_rt));
    sc_trace(tf, divider_rt, GET_NAME(divider_rt));
    sc_trace(tf, config_rt, GET_NAME(config_rt));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
}