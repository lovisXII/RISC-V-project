#include "dcache.h"

void dcache::adresse_parcer()
{
  sc_uint<32> DT_A_SM = DATA_ADR_SM.read();
  address_tag.write(DT_A_SM.range(31,11));
  address_index.write(DT_A_SM.range(10,4));
  address_offset.write(DT_A_SM.range(3,0));
}

void dcache::miss_detection()
{

    // Assignation of MEM_SIZE, just to do an interface 
    //with the core

    MEM_SIZE_SC = MEM_SIZE_SM ;
  /*std::cout << "tag : " << address_tag 
            << "    index : " << address_index.read() 
            << "    tag0 : " << w0_TAG[address_index.read()]
            << "    tag1 : " << w1_TAG[address_index.read()] << std::endl;*/
  // COMPARE HIT WAY0
  if(address_tag == w0_TAG[address_index.read()])
  {
    way0_hit = w0_LINE_VALIDATE[address_index.read()];
    if(LOAD_SM.read() && way0_hit)
    {
      DATA_SC.write(w0_word[address_index.read()][address_offset.read()/4]);
    }
    
  }
  else
    way0_hit = false;

  // COMPARE HIT WAY1
  if(address_tag == w1_TAG[address_index.read()])
  {
    way1_hit = w1_LINE_VALIDATE[address_index.read()];

    //std::cout << "    -> " << way1_hit << std::endl;
    if(LOAD_SM.read() && way1_hit)
    {
      DATA_SC.write(w1_word[address_index.read()][address_offset.read()/4]);
    }
  }
  else
    way1_hit = false;
}

void dcache::transition()
{
  typedef enum // MAE STATES
  {
    IDLE,
    WAIT_BUFF_READ,
    WAIT_MEM,
    UPDT,
    WAIT_BUFF_WRITE,
    ERROR
  } states_fsm;

  states_fsm fsm_current_state = IDLE;
    
  bool dta_v;
  bool write_b;
  bool read_b;
  bool miss;
  int burst_cpt;
  while(1)
  {
    dta_v = false;
    write_b = false;
    read_b = true;
    switch(fsm_current_state)
    {
      case IDLE:
        miss = !way0_hit && !way1_hit;
        write_b = (LOAD_SM.read() || STORE_SM.read()) && VALID_ADR_SM.read() && !full && miss;
        dta_v = (LOAD_SM.read() || STORE_SM.read()) && !full && VALID_ADR_SM.read();
      
        if(LOAD_SM.read() && VALID_ADR_SM.read())
        {
          if(miss && full)
          {
            fsm_current_state = WAIT_BUFF_READ;
            STALL_SC.write(true);
          }
          else if(miss && !full)
          {
            fsm_current_state = WAIT_MEM;
            STALL_SC.write(true);
          }
          else if(!miss)
          {
            STALL_SC.write(false);
          }
        }
        else if(STORE_SM.read() && VALID_ADR_SM.read())
        {
          if(full)
          {
            fsm_current_state = WAIT_BUFF_WRITE;
            STALL_SC.write(true);
          }
          else
          {
            STALL_SC.write(false);            
          }

          if(miss)
          {
            if(LRU_bit_check[address_index.read()])
            {
              w0_word[address_index.read()][address_offset.read()/4] = DATA_SM.read();
              w0_TAG[address_index.read()] = address_tag.read();
              w0_LINE_VALIDATE[address_index.read()] = true;
            }
            else
            {
              w1_word[address_index.read()][address_offset.read()/4] = DATA_SM.read();
              w1_TAG[address_index.read()] = address_tag.read();
              w1_LINE_VALIDATE[address_index.read()] = true;
            }
          }
          else if(way0_hit)
          {
            w0_word[address_index.read()][address_offset.read()/4] = DATA_SM.read();
          }
          else if(way1_hit)
          {
            w1_word[address_index.read()][address_offset.read()/4] = DATA_SM.read();
          }
        } 
        break;
      case WAIT_BUFF_READ:
        if(!full)
        {
          dta_v = true;

          fsm_current_state = WAIT_MEM;
        }
        break;
        case WAIT_MEM:
        //std::cout << "dcache : WAIT MEME" << std::endl;

        if(SLAVE_ACK_SP.read())
        {
          dta_v = false;
          read_b = false;

          //parse adresse from  RAM
          sc_uint<32> DT_A_MP = A_SP.read();

          if(LRU_bit_check[DT_A_MP.range(10,4)])
          {
            w0_word[DT_A_MP.range(10,4)][burst_cpt++] = DT_SP.read();
            w0_TAG[DT_A_MP.range(10,4)] = DT_A_MP.range(31,11);
            w0_LINE_VALIDATE[DT_A_MP.range(10,4)] = true;
          }
          else
          {
            w1_word[DT_A_MP.range(10,4)][burst_cpt++] = DT_SP.read();
            w1_TAG[DT_A_MP.range(10,4)] = DT_A_MP.range(31,11);
            w1_LINE_VALIDATE[DT_A_MP.range(10,4)] = true;
          }
          
          mp_last_addr_offset = mp_address_offset;

          fsm_current_state = UPDT;
        }
        else
        {
          read_b = true;
          dta_v = true;

          burst_cpt = 0;
        }
        write_b = false;
        break;
        case UPDT:
          if(!SLAVE_ACK_SP.read())
          {
            STALL_SC.write(false);

            LRU_bit_check[mp_address_index.read()] = !LRU_bit_check[mp_address_index.read()];

            fsm_current_state = IDLE;
          }
          else
          {
            //parse adresse from  RAM
            sc_uint<32> DT_A_MP = A_SP.read();

            if(DT_A_MP.range(3,0) == (mp_last_addr_offset.read() + 4))
            {
              mp_last_addr_offset = DT_A_MP.range(3,0);
              if(LRU_bit_check[DT_A_MP.range(10,4)])
              {
                w0_word[DT_A_MP.range(10,4)][burst_cpt++] = DT_SP.read();
              }
              else
              {
                w1_word[DT_A_MP.range(10,4)][burst_cpt++] = DT_SP.read();
              }
            }
          }
        break;
        case WAIT_BUFF_WRITE:
        if(!full)
        {
          dta_v = true;
          write_b = true;

          fsm_current_state = IDLE;
        }
        break;
        case ERROR:
          std::cout << "WTF" << std::endl;
        break;
    }
    DTA_VALID_SC.write(dta_v);
    write_buff.write(write_b);
    read_buff.write(read_b);
    //debug
    switch(fsm_current_state)
    {
      case IDLE:
        fsm_state.write(0);
      break;
      case WAIT_BUFF_READ:
        fsm_state.write(1);
      break;
      case WAIT_MEM:
        fsm_state.write(2);
      break;
      case UPDT:
        fsm_state.write(3);
      break;
      case WAIT_BUFF_WRITE:
        fsm_state.write(4);
      break;
      case ERROR:
        fsm_state.write(5);
      break;
    }
    wait();
  }
}

void dcache::trace(sc_trace_file* tf)
{
  sc_trace(tf, CLK, GET_NAME(CLK));
  sc_trace(tf, RESET_N, GET_NAME(RESET_N));

  // interfaz processeur
  sc_trace(tf, DATA_ADR_SM, GET_NAME(DATA_ADR_SM));
  sc_trace(tf, DATA_SM, GET_NAME(DATA_SM));
  sc_trace(tf, LOAD_SM, GET_NAME(LOAD_SM));
  sc_trace(tf, STORE_SM, GET_NAME(STORE_SM));
  sc_trace(tf, VALID_ADR_SM, GET_NAME(VALID_ADR_SM));

  sc_trace(tf, DATA_SC, GET_NAME(DATA_SC));
  sc_trace(tf, STALL_SC, GET_NAME(STALL_SC));

  // interfaz bus
  sc_trace(tf, DTA_VALID_SC, GET_NAME(DTA_VALID_SC));
  sc_trace(tf, READ_SC, GET_NAME(READ_SC)); 
  sc_trace(tf, WRITE_SC, GET_NAME(WRITE_SC));
  sc_trace(tf, DT_SC, GET_NAME(DT_SC));
  sc_trace(tf, A_SC, GET_NAME(A_SC));
  sc_trace(tf, DT_SP, GET_NAME(DT_SP));
  sc_trace(tf, A_SP, GET_NAME(A_SP));
  sc_trace(tf, SLAVE_ACK_SP, GET_NAME(SLAVE_ACK_SP));

  // signals
  sc_trace(tf, way0_hit, GET_NAME(way0_hit));
  sc_trace(tf, way1_hit, GET_NAME(way1_hit));

  sc_trace(tf, full, GET_NAME(full));
  sc_trace(tf, empty, GET_NAME(empty));

  sc_trace(tf, fsm_state, GET_NAME(fsm_state));

  sc_trace(tf, write_buff, GET_NAME(write_buff));
  sc_trace(tf, read_buff, GET_NAME(read_buff));

  sc_trace(tf, address_tag, GET_NAME(address_tag));
  sc_trace(tf, address_index, GET_NAME(address_index));
  sc_trace(tf, address_offset, GET_NAME(address_offset));

  sc_trace(tf, mp_address_tag, GET_NAME(mp_address_tag));
  sc_trace(tf, mp_address_index, GET_NAME(mp_address_index));
  sc_trace(tf, MEM_SIZE_SM, GET_NAME(MEM_SIZE_SM));
  sc_trace(tf, MEM_SIZE_SC, GET_NAME(MEM_SIZE_SC));

  for (int i = 0; i < 128; i++) {
    std::string icname = "DCACHE_";
    for(int j = 0; j < 4; j++)
    {
        icname = "D_W0_I_" + std::to_string(i) + "_O_" + std::to_string(j);
        sc_trace(tf, w0_word[i][j], signal_get_name(w0_word[i][j].name(), icname.c_str()));
        icname = "D_W1_I_" + std::to_string(i) + "_O_" + std::to_string(j);
        sc_trace(tf, w1_word[i][j], signal_get_name(w1_word[i][j].name(), icname.c_str()));
    }
    icname = "W0_TAG_" + std::to_string(i);
    sc_trace(tf, w0_TAG[i], signal_get_name(w0_TAG[i].name(), icname.c_str()));
    icname = "W1_TAG_" + std::to_string(i);
    sc_trace(tf, w1_TAG[i], signal_get_name(w1_TAG[i].name(), icname.c_str()));

    icname = "W0_VALID_" + std::to_string(i);
    sc_trace(tf, w0_LINE_VALIDATE[i], signal_get_name(w0_LINE_VALIDATE[i].name(), icname.c_str()));
    icname = "W1_VALID_" + std::to_string(i);
    sc_trace(tf, w1_LINE_VALIDATE[i], signal_get_name(w1_LINE_VALIDATE[i].name(), icname.c_str()));

    icname = "WAY_LRU_" + std::to_string(i);
    sc_trace(tf, LRU_bit_check[i], signal_get_name(LRU_bit_check[i].name(), icname.c_str()));
  }

  buffcache_inst.trace(tf); 
}