#include "shifter.h"

void shifter::decode_cmd() {
    switch (CMD_SE.read()) {
        case 0:
            sll_se.write(1);
            srl_se.write(0);
            sra_se.write(0);
            break;
        case 1:
            sll_se.write(0);
            srl_se.write(1);
            sra_se.write(0);
            break;
        default:
            sll_se.write(0);
            srl_se.write(0);
            sra_se.write(1);
            break;
    }
}

void shifter::shifter_sll() {
    sc_uint<32> din       = DIN_SE.read();
    sc_uint<5>  shift_val = SHIFT_VAL_SE.read();
    sc_uint<32> sll_dout;
    switch (shift_val) {
        // from the mips code.
        case 0: sll_dout.range(31, 0) = din.range(31, 0); break;
        case 1:
            sll_dout.range(31, 1) = din.range(30, 0);
            sll_dout.range(0, 0)  = 0;
            break;
        case 2:
            sll_dout.range(31, 2) = din.range(29, 0);
            sll_dout.range(1, 0)  = 0;
            break;
        case 3:
            sll_dout.range(31, 3) = din.range(28, 0);
            sll_dout.range(2, 0)  = 0;
            break;
        case 4:
            sll_dout.range(31, 4) = din.range(27, 0);
            sll_dout.range(3, 0)  = 0;
            break;
        case 5:
            sll_dout.range(31, 5) = din.range(26, 0);
            sll_dout.range(4, 0)  = 0;
            break;
        case 6:
            sll_dout.range(31, 6) = din.range(25, 0);
            sll_dout.range(5, 0)  = 0;
            break;
        case 7:
            sll_dout.range(31, 7) = din.range(24, 0);
            sll_dout.range(6, 0)  = 0;
            break;
        case 8:
            sll_dout.range(31, 8) = din.range(23, 0);
            sll_dout.range(7, 0)  = 0;
            break;
        case 9:
            sll_dout.range(31, 9) = din.range(22, 0);
            sll_dout.range(8, 0)  = 0;
            break;
        case 10:
            sll_dout.range(31, 10) = din.range(21, 0);
            sll_dout.range(9, 0)   = 0;
            break;
        case 11:
            sll_dout.range(31, 11) = din.range(20, 0);
            sll_dout.range(10, 0)  = 0;
            break;
        case 12:
            sll_dout.range(31, 12) = din.range(19, 0);
            sll_dout.range(11, 0)  = 0;
            break;
        case 13:
            sll_dout.range(31, 13) = din.range(18, 0);
            sll_dout.range(12, 0)  = 0;
            break;
        case 14:
            sll_dout.range(31, 14) = din.range(17, 0);
            sll_dout.range(13, 0)  = 0;
            break;
        case 15:
            sll_dout.range(31, 15) = din.range(16, 0);
            sll_dout.range(14, 0)  = 0;
            break;
        case 16:
            sll_dout.range(31, 16) = din.range(15, 0);
            sll_dout.range(15, 0)  = 0;
            break;
        case 17:
            sll_dout.range(31, 17) = din.range(14, 0);
            sll_dout.range(16, 0)  = 0;
            break;
        case 18:
            sll_dout.range(31, 18) = din.range(13, 0);
            sll_dout.range(17, 0)  = 0;
            break;
        case 19:
            sll_dout.range(31, 19) = din.range(12, 0);
            sll_dout.range(18, 0)  = 0;
            break;
        case 20:
            sll_dout.range(31, 20) = din.range(11, 0);
            sll_dout.range(19, 0)  = 0;
            break;
        case 21:
            sll_dout.range(31, 21) = din.range(10, 0);
            sll_dout.range(20, 0)  = 0;
            break;
        case 22:
            sll_dout.range(31, 22) = din.range(9, 0);
            sll_dout.range(21, 0)  = 0;
            break;
        case 23:
            sll_dout.range(31, 23) = din.range(8, 0);
            sll_dout.range(22, 0)  = 0;
            break;
        case 24:
            sll_dout.range(31, 24) = din.range(7, 0);
            sll_dout.range(23, 0)  = 0;
            break;
        case 25:
            sll_dout.range(31, 25) = din.range(6, 0);
            sll_dout.range(24, 0)  = 0;
            break;
        case 26:
            sll_dout.range(31, 26) = din.range(5, 0);
            sll_dout.range(25, 0)  = 0;
            break;
        case 27:
            sll_dout.range(31, 27) = din.range(4, 0);
            sll_dout.range(26, 0)  = 0;
            break;
        case 28:
            sll_dout.range(31, 28) = din.range(3, 0);
            sll_dout.range(27, 0)  = 0;
            break;
        case 29:
            sll_dout.range(31, 29) = din.range(2, 0);
            sll_dout.range(28, 0)  = 0;
            break;
        case 30:
            sll_dout.range(31, 30) = din.range(1, 0);
            sll_dout.range(29, 0)  = 0;
            break;
        case 31:
            sll_dout.range(31, 31) = din.range(0, 0);
            sll_dout.range(30, 0)  = 0;
            break;
    }
    sll_dout_se.write(sll_dout);
}

void shifter::shifter_sr() {
    sc_uint<32> din       = DIN_SE.read();
    sc_uint<5>  shift_val = SHIFT_VAL_SE.read();
    sc_uint<32> sr_dout;
    sc_uint<32> shiftin;
    // compute the sign extension for arithmetic shift
    if (sra_se.read() & din[31]) {
        shiftin = 0xFFFFFFFF;
    } else {
        shiftin = 0x0;
    }
    switch (shift_val) {
        case 0: sr_dout.range(31, 0) = din.range(31, 0); break;
        case 1:
            sr_dout.range(31, 31) = shiftin.range(31, 31);
            sr_dout.range(30, 0)  = din.range(31, 1);
            break;
        case 2:
            sr_dout.range(31, 30) = shiftin.range(31, 30);
            sr_dout.range(29, 0)  = din.range(31, 2);
            break;
        case 3:
            sr_dout.range(31, 29) = shiftin.range(31, 29);
            sr_dout.range(28, 0)  = din.range(31, 3);
            break;
        case 4:
            sr_dout.range(31, 28) = shiftin.range(31, 28);
            sr_dout.range(27, 0)  = din.range(31, 4);
            break;
        case 5:
            sr_dout.range(31, 27) = shiftin.range(31, 27);
            sr_dout.range(26, 0)  = din.range(31, 5);
            break;
        case 6:
            sr_dout.range(31, 26) = shiftin.range(31, 26);
            sr_dout.range(25, 0)  = din.range(31, 6);
            break;
        case 7:
            sr_dout.range(31, 25) = shiftin.range(31, 25);
            sr_dout.range(24, 0)  = din.range(31, 7);
            break;
        case 8:
            sr_dout.range(31, 24) = shiftin.range(31, 24);
            sr_dout.range(23, 0)  = din.range(31, 8);
            break;
        case 9:
            sr_dout.range(31, 23) = shiftin.range(31, 23);
            sr_dout.range(22, 0)  = din.range(31, 9);
            break;
        case 10:
            sr_dout.range(31, 22) = shiftin.range(31, 22);
            sr_dout.range(21, 0)  = din.range(31, 10);
            break;
        case 11:
            sr_dout.range(31, 21) = shiftin.range(31, 21);
            sr_dout.range(20, 0)  = din.range(31, 11);
            break;
        case 12:
            sr_dout.range(31, 20) = shiftin.range(31, 20);
            sr_dout.range(19, 0)  = din.range(31, 12);
            break;
        case 13:
            sr_dout.range(31, 19) = shiftin.range(31, 19);
            sr_dout.range(18, 0)  = din.range(31, 13);
            break;
        case 14:
            sr_dout.range(31, 18) = shiftin.range(31, 18);
            sr_dout.range(17, 0)  = din.range(31, 14);
            break;
        case 15:
            sr_dout.range(31, 17) = shiftin.range(31, 17);
            sr_dout.range(16, 0)  = din.range(31, 15);
            break;
        case 16:
            sr_dout.range(31, 16) = shiftin.range(31, 16);
            sr_dout.range(15, 0)  = din.range(31, 16);
            break;
        case 17:
            sr_dout.range(31, 15) = shiftin.range(31, 15);
            sr_dout.range(14, 0)  = din.range(31, 17);
            break;
        case 18:
            sr_dout.range(31, 14) = shiftin.range(31, 14);
            sr_dout.range(13, 0)  = din.range(31, 18);
            break;
        case 19:
            sr_dout.range(31, 13) = shiftin.range(31, 13);
            sr_dout.range(12, 0)  = din.range(31, 19);
            break;
        case 20:
            sr_dout.range(31, 12) = shiftin.range(31, 12);
            sr_dout.range(11, 0)  = din.range(31, 20);
            break;
        case 21:
            sr_dout.range(31, 11) = shiftin.range(31, 11);
            sr_dout.range(10, 0)  = din.range(31, 21);
            break;
        case 22:
            sr_dout.range(31, 10) = shiftin.range(31, 10);
            sr_dout.range(9, 0)   = din.range(31, 22);
            break;
        case 23:
            sr_dout.range(31, 9) = shiftin.range(31, 9);
            sr_dout.range(8, 0)  = din.range(31, 23);
            break;
        case 24:
            sr_dout.range(31, 8) = shiftin.range(31, 8);
            sr_dout.range(7, 0)  = din.range(31, 24);
            break;
        case 25:
            sr_dout.range(31, 7) = shiftin.range(31, 7);
            sr_dout.range(6, 0)  = din.range(31, 25);
            break;
        case 26:
            sr_dout.range(31, 6) = shiftin.range(31, 6);
            sr_dout.range(5, 0)  = din.range(31, 26);
            break;
        case 27:
            sr_dout.range(31, 5) = shiftin.range(31, 5);
            sr_dout.range(4, 0)  = din.range(31, 27);
            break;
        case 28:
            sr_dout.range(31, 4) = shiftin.range(31, 4);
            sr_dout.range(3, 0)  = din.range(31, 28);
            break;
        case 29:
            sr_dout.range(31, 3) = shiftin.range(31, 3);
            sr_dout.range(2, 0)  = din.range(31, 29);
            break;
        case 30:
            sr_dout.range(31, 2) = shiftin.range(31, 2);
            sr_dout.range(1, 0)  = din.range(31, 30);
            break;
        case 31:
            sr_dout.range(31, 1) = shiftin.range(31, 1);
            sr_dout.range(0, 0)  = din.range(31, 31);
            break;
    }
    sr_dout_se.write(sr_dout);
}

void shifter::shifter_agregate() {
    if (sll_se.read()) {
        DOUT_SE.write(sll_dout_se.read());
    } else {
        DOUT_SE.write(sr_dout_se.read());
    }
}

void shifter::trace(sc_trace_file* tf) {
    sc_trace(tf, DIN_SE, GET_NAME(DIN_SE));
    sc_trace(tf, SHIFT_VAL_SE, GET_NAME(SHIFT_VAL_SE));
    sc_trace(tf, CMD_SE, GET_NAME(CMD_SE));
    sc_trace(tf, DOUT_SE, GET_NAME(DOUT_SE));
    sc_trace(tf, sra_se, GET_NAME(sra_se));
    sc_trace(tf, sll_se, GET_NAME(sll_se));
    sc_trace(tf, srl_se, GET_NAME(srl_se));
    sc_trace(tf, sll_dout_se, GET_NAME(sll_dout_se));
    sc_trace(tf, sr_dout_se, GET_NAME(sr_dout_se));
}