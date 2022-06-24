#include "x2_multiplier.h"

void x2_multiplier::pre_process()
{
    sc_bv<128> in = IN_RX1.read();
    a = in.range(63, 0);
    b = in.range(127, 64);

    c[0] = CARRY_RX1.read();

    //test
    sc_biguint<64> a_ = (sc_bv<64>)a;
    sc_biguint<64> b_ = (sc_bv<64>)b;
    sc_biguint<64> test = a_ + b_;

    for(int i = 63; i >= 0; i--)
    {
        S[i] = (bool)test[i];
    }
}

void x2_multiplier::MFA_0()
{
    /*sc_bv<64> a_ = (sc_bv<64>)a;
    sc_bv<64> b_ = (sc_bv<64>)b;
    
    P[0] = (bool)a_[0] ^ (bool)b_[0];
    G[0] = (bool)a_[0] & (bool)b_[0];
    S[0] = ((bool)a_[0] ^ (bool)b_[0]) ^ c[0];*/
}
void x2_multiplier::C_gen_1()
{
    c[1] = G[0] + (P[0] & c[0]);
}

void x2_multiplier::RES()
{
    sc_bv<64> res;
    for(int i = 0; i < 64; i++)
        res[i] = S[i];

    if(SIGNED_OP_RX1)
        RES_RX2.write((sc_bv_base)res.range(63, 32));
    else
        RES_RX2.write((sc_bv_base)res.range(31, 0));
}
void x2_multiplier::manage_fifo() 
{
    X12X2_POP_SX2.write(!X12X2_EMPTY_SX1);
}
void x2_multiplier::trace(sc_trace_file* tf)
{
    sc_trace(tf, IN_RX1, GET_NAME(IN_RX1));
    sc_trace(tf, a, GET_NAME(a));
    sc_trace(tf, b, GET_NAME(b));
    sc_trace(tf, RES_RX2, GET_NAME(RES_RX2));
    sc_trace(tf, SIGNED_OP_RX1, GET_NAME(SIGNED_OP_RX1));
}