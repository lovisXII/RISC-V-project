#ifndef D_CACHE
#define D_CACHE

#include <systemc.h>
#include "buffercache.h"
#include "../UTIL/debug_util.h"

//cache N-way associatif, write through et buffet
// taille du cache 1024
// buffer de taille 2

// 2-way => 1024/2 => 512 / 4mots => 128 lignes
// index => 7 bits
// offset => 4 mots => 2 bits
// tag => 23 bits

//communication direct avec la MP (pas de bus ni BCU) 

// C: cache   M: memory  P: MP
//acronym_X

#define WAY_SIZE 128



SC_MODULE(dcache)
{
  sc_in_clk CLK;
  sc_in<bool> RESET_N;

// interface processeur
  sc_in<sc_uint<32>> DATA_ADR_SM;
  sc_in<sc_uint<32>> DATA_SM;
  sc_in<bool> LOAD_SM;
  sc_in<bool> STORE_SM;
  sc_in<bool> VALID_ADR_SM;
  sc_in<sc_uint<2>> MEM_SIZE_SM;

  sc_out<sc_uint<32>> DATA_SC;
  sc_out<bool> STALL_SC;               // if stall donc miss else hit
// interface MP
  sc_out<sc_uint<2>> MEM_SIZE_SC;
  sc_out<bool> DTA_VALID_SC;         // data or/and adresse valid
  sc_out<bool> READ_SC, WRITE_SC;

  // DT & A n'ont pas de reference d'ou il vient car ils peuvent venir de 
  // la MP ou du CACHE
  sc_out<sc_uint<32>> DT_SC;
  sc_out<sc_uint<32>> A_SC;

  sc_in<sc_uint<32>> DT_SP;
  sc_in<sc_uint<32>> A_SP;
  sc_in<bool> SLAVE_ACK_SP;          // slave answer (slave dt valid)

//signals
  //parse address from CPU
  sc_signal<sc_uint<21>> address_tag;
  sc_signal<sc_uint<7>> address_index;
  sc_signal<sc_uint<4>> address_offset;
  //parse address from MP
  sc_signal<sc_uint<21>> mp_address_tag;
  sc_signal<sc_uint<7>> mp_address_index;
  sc_signal<sc_uint<4>> mp_address_offset;

  sc_signal<sc_uint<4>> mp_last_addr_offset;

  sc_signal<bool> way0_hit;
  sc_signal<bool> way1_hit;
  
  sc_signal<sc_uint<32>> selected_data;

  sc_signal<bool> current_LRU; // false: 0, true: 1

// WAYS 128 lines
  sc_signal<bool> LRU_bit_check[128];     // bit to compare least recently used 

// WAY 0
  sc_signal<sc_uint<32>> w0_word[128][4];
  sc_signal<sc_uint<21>> w0_TAG[128];
  sc_signal<bool> w0_LINE_VALIDATE[128];

//WAY 1
  sc_signal<sc_uint<32>> w1_word[128][4];
  sc_signal<sc_uint<21>> w1_TAG[128];
  sc_signal<bool> w1_LINE_VALIDATE[128];

//buffer
  sc_signal<bool> write_buff, read_buff;
  sc_signal<bool> full, empty;

//FMS signal debug
  sc_signal<sc_uint<3>> fsm_state;

  void adresse_parcer();
  void miss_detection();
  void transition();

  void buffer_manager();

  void trace(sc_trace_file*);

  buffercache buffcache_inst;

  SC_CTOR(dcache) :
  buffcache_inst("buffercache")
  {     
    SC_METHOD(adresse_parcer);
    sensitive << DATA_ADR_SM;

    SC_METHOD(miss_detection);
    sensitive << address_tag 
              << MEM_SIZE_SM
              << address_index << address_offset << STALL_SC << CLK;
              
      
    SC_THREAD(transition);
    sensitive << CLK.neg() << SLAVE_ACK_SP << A_SP;

    reset_signal_is(RESET_N, false);


    buffcache_inst.RESET_N(RESET_N);
    buffcache_inst.CLK(CLK);
    buffcache_inst.WRITE_OBUFF(write_buff);
    buffcache_inst.READ_OBUFF(read_buff);
    buffcache_inst.DATA_C(DATA_SM);
    buffcache_inst.ADR_C(DATA_ADR_SM);
    buffcache_inst.STORE_C(STORE_SM);
    buffcache_inst.LOAD_C(LOAD_SM);
    buffcache_inst.FULL(full);
    buffcache_inst.EMPTY(empty);
    buffcache_inst.DATA_MP(DT_SC);
    buffcache_inst.ADR_MP(A_SC);
    buffcache_inst.STORE_MP(WRITE_SC);
    buffcache_inst.LOAD_MP(READ_SC);
    
  }
};

#endif