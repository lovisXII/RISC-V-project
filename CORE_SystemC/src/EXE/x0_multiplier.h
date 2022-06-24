#pragma once
#include <systemc.h>
#include "../UTIL/debug_util.h"
#include "../UTIL/fifo.h"

#define x02x1_size          386

SC_MODULE(x0_multiplier)
{
    // input :
    sc_in<sc_uint<32>> OP1_SE, OP2_SE;
    sc_in<sc_uint<2>>  EXE_CMD_RD;
    sc_in<bool>        X02X1_POP_SX1;
    sc_in<bool>        DEC2X0_EMPTY_SD;
    
    // output :
    sc_out<sc_bv<384>> RES_RX0;
    sc_out<bool>       SIGNED_OP_RX0;
    sc_out<bool>       CARRY_RX0;
    sc_out<bool>       X02X1_EMPTY_SX0;

    // General interace : 
    sc_in_clk   CLK;
    sc_in<bool> RESET;


    sc_signal<sc_bv<64>> product[34];
    sc_signal<sc_bv<64>> product_s1[22]; // product of stage 1
    sc_signal<sc_bv<64>> product_s2[14]; // product of stage 2
    sc_signal<sc_bv<64>> product_s3[10]; // product of stage 3
    sc_signal<sc_bv<64>> product_s4[6];  // product of stage 4
    sc_signal<sc_bv<64>> product_s5[4];  // product of stage 5

    sc_signal<bool>        signed_op;
    sc_signal<bool>        select_higher_bits_sx0;
    sc_signal<bool>        carry_sx0;
    
    // fifo x02x1
    sc_signal<sc_bv<x02x1_size>> x02x1_din_sx0;  
    sc_signal<sc_bv<x02x1_size>> x02x1_dout_sx0;
    sc_signal<bool> x02x1_push_sx0, x02x1_full_sx0;

    fifo<x02x1_size> fifo_inst;

    void operation();

    // stage 1 (11 CSA remind product 33)
    void CSA_1();
    void CSA_2();
    void CSA_3();
    void CSA_4();
    void CSA_5();
    void CSA_6();
    void CSA_7();
    void CSA_8();
    void CSA_9();
    void CSA_10();
    void CSA_11();

    // stage 2 (7 CSA remind product 33 and product_s1 21)

    void CSA_12();
    void CSA_13();
    void CSA_14();
    void CSA_15();
    void CSA_16();
    void CSA_17();
    void CSA_18();

    //stage 3 (5 CSA remind product 33)

    void CSA_19();
    void CSA_20();
    void CSA_21();
    void CSA_22();
    void CSA_23();

    //stage 4 (3 CSA remind product_s3 9 product 33)

    void CSA_24();
    void CSA_25();
    void CSA_26();

    //stage 5 (2 CSA remind product_s3 9)

    void CSA_27();
    void CSA_28();

    void bypasses();  // allow the push/pop of fifo exe2mem

    //res => 320bits => 5x64 => M4 M3 M2 M1 M0
    void manage_fifo();
    void fifo_concat();
    void fifo_unconcat();
    void trace(sc_trace_file* tf);

    SC_CTOR(x0_multiplier) :
    fifo_inst("x02x1")
    {
        fifo_inst.DIN_S(x02x1_din_sx0);
        fifo_inst.DOUT_R(x02x1_dout_sx0);
        fifo_inst.EMPTY_S(X02X1_EMPTY_SX0);
        fifo_inst.FULL_S(x02x1_full_sx0);
        fifo_inst.PUSH_S(x02x1_push_sx0);
        fifo_inst.POP_S(X02X1_POP_SX1);
        fifo_inst.CLK(CLK);
        fifo_inst.RESET_N(RESET);

        SC_METHOD(operation);
        sensitive << OP1_SE << OP2_SE << EXE_CMD_RD;

        //stage 1
        SC_METHOD(CSA_1);
        sensitive << product[0] << product[1] << product[2];

        SC_METHOD(CSA_2);
        sensitive << product[3] << product[4] << product[5];
        
        SC_METHOD(CSA_3);
        sensitive << product[6] << product[7] << product[8];

        SC_METHOD(CSA_4);
        sensitive << product[9] << product[10] << product[11];

        SC_METHOD(CSA_5);
        sensitive << product[12] << product[13] << product[14];

        SC_METHOD(CSA_6);
        sensitive << product[15] << product[16] << product[17];

        SC_METHOD(CSA_7);
        sensitive << product[18] << product[19] << product[20];

        SC_METHOD(CSA_8);
        sensitive << product[21] << product[22] << product[23];

        SC_METHOD(CSA_9);
        sensitive << product[24] << product[25] << product[26];

        SC_METHOD(CSA_10);
        sensitive << product[27] << product[28] << product[29];

        SC_METHOD(CSA_11);
        sensitive << product[30] << product[31] << product[32];

        // stage 2
        SC_METHOD(CSA_12);
        sensitive << product_s1[0] << product_s1[1] << product_s1[2];

        SC_METHOD(CSA_13);
        sensitive << product_s1[3] << product_s1[4] << product_s1[5];
        
        SC_METHOD(CSA_14);
        sensitive << product_s1[6] << product_s1[7] << product_s1[8];

        SC_METHOD(CSA_15);
        sensitive << product_s1[9] << product_s1[10] << product_s1[11];

        SC_METHOD(CSA_16);
        sensitive << product_s1[12] << product_s1[13] << product_s1[14];

        SC_METHOD(CSA_17);
        sensitive << product_s1[15] << product_s1[16] << product_s1[17];

        SC_METHOD(CSA_18);
        sensitive << product_s1[18] << product_s1[19] << product_s1[20];

        // stage 3
        SC_METHOD(CSA_19);
        sensitive << product_s2[0] << product_s2[1] << product_s2[2];

        SC_METHOD(CSA_20);
        sensitive << product_s2[3] << product_s2[4] << product_s2[5];
        
        SC_METHOD(CSA_21);
        sensitive << product_s2[6] << product_s2[7] << product_s2[8];

        SC_METHOD(CSA_22);
        sensitive << product_s2[9] << product_s2[10] << product_s2[11];

        SC_METHOD(CSA_23);
        sensitive << product_s2[12] << product_s2[13] << product_s1[21];

        //stage 4
        SC_METHOD(CSA_24);
        sensitive << product_s3[0] << product_s3[1] << product_s3[2];

        SC_METHOD(CSA_25);
        sensitive << product_s3[3] << product_s3[4] << product_s3[5];
        
        SC_METHOD(CSA_26);
        sensitive << product_s3[6] << product_s3[7] << product_s3[8];

        //stage 5
        SC_METHOD(CSA_27);
        sensitive << product_s4[0] << product_s4[1] << product_s4[2];

        SC_METHOD(CSA_28);
        sensitive << product_s4[3] << product_s4[4] << product_s4[5];

        //fifo
        SC_METHOD(fifo_concat);
        sensitive << product_s5[0] << product_s5[1] << product_s5[2] << product_s5[3];

        SC_METHOD(fifo_unconcat);
        sensitive << x02x1_dout_sx0;

        SC_METHOD(manage_fifo);
        sensitive << x02x1_full_sx0 << DEC2X0_EMPTY_SD;
    }
};