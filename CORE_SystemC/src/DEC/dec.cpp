#include "dec.h"


// ---------------------------------------------DECODING INSTRUCTION
// :---------------------------------------------

void decod::concat_dec2exe() {
    sc_bv<dec2exe_size> dec2exe_in_var;
    if (EXCEPTION_SM.read() == 0) {
        dec2exe_in_var.range(251,220) = pc_branch_value_sd;  
        dec2exe_in_var[219] = mul_i_sd || mulh_i_sd || mulhsu_i_sd || mulhu_i_sd;  
        dec2exe_in_var[218] = ebreak_i_sd;
        dec2exe_in_var[217] = instruction_access_fault_sd;  
        dec2exe_in_var[216] = mret_i_sd;            
        dec2exe_in_var[215] = block_bp_sd;
        dec2exe_in_var[214] = illegal_instruction_sd || instruction_adress_missaligned_sd || env_call_u_mode_sd ||
                              env_call_m_mode_sd || env_call_s_mode_sd || env_call_wrong_mode || mret_i_sd ||
                              instruction_access_fault_sd || ebreak_i_sd;  // tells if there is an exception
        dec2exe_in_var[213]            = env_call_wrong_mode.read();
        dec2exe_in_var[212]            = env_call_u_mode_sd.read();
        dec2exe_in_var[211]            = illegal_instruction_sd.read();
        dec2exe_in_var[210]            = instruction_adress_missaligned_sd.read();
        dec2exe_in_var[209]            = env_call_m_mode_sd.read();
        dec2exe_in_var[208]            = env_call_s_mode_sd.read();
        dec2exe_in_var.range(207, 176) = CSR_RDATA_SC.read();
        dec2exe_in_var[175]            = csr_wenable_sd.read();
        dec2exe_in_var.range(174, 163) = csr_radr_sd.read();
        dec2exe_in_var.range(162, 131) = PC_IF2DEC_RI.read();
        dec2exe_in_var[130]            = r1_valid_sd.read();
        dec2exe_in_var[129]            = r2_valid_sd.read();
        dec2exe_in_var.range(128, 123) = RADR1_SD.read();
        dec2exe_in_var.range(122, 117) = RADR2_SD.read();
        dec2exe_in_var.range(116, 115) = exe_cmd_sd.read();
        dec2exe_in_var.range(114, 83)  = exe_op1_sd.read();
        dec2exe_in_var.range(82, 51)   = exe_op2_sd.read();
        dec2exe_in_var[50]             = exe_neg_op2_sd.read();
        dec2exe_in_var[49]             = exe_wb_sd.read();

        dec2exe_in_var.range(48, 17) = mem_data_sd.read();

        dec2exe_in_var[16] = mem_load_sd.read();
        dec2exe_in_var[15] = mem_store_sd.read();

        dec2exe_in_var[14]           = mem_sign_extend_sd.read();
        dec2exe_in_var.range(13, 12) = mem_size_sd.read();
        dec2exe_in_var.range(11, 8)  = select_type_operations_sd.read();
        dec2exe_in_var.range(7, 2)   = adr_dest_sd.read();
        dec2exe_in_var[1]            = slt_i_sd.read() | slti_i_sd.read();
        dec2exe_in_var[0]            = sltu_i_sd.read() | sltiu_i_sd.read();
    } else {
        dec2exe_in_var.range(251,220)  = 0;
        dec2exe_in_var[219]            = 0;
        dec2exe_in_var[218]            = 0; 
        dec2exe_in_var[217]            = 0;  
        dec2exe_in_var[216]            = 0;            
        dec2exe_in_var[215]            = 0;
        dec2exe_in_var[214]            = 0;  // tells if there is an exception
        dec2exe_in_var[213]            = 0;
        dec2exe_in_var[212]            = 0;
        dec2exe_in_var[211]            = 0;
        dec2exe_in_var[210]            = 0;
        dec2exe_in_var[209]            = 0;
        dec2exe_in_var[208]            = 0;
        dec2exe_in_var.range(207, 176) = 0;
        dec2exe_in_var[175]            = 0;
        dec2exe_in_var.range(174, 163) = 0;
        dec2exe_in_var.range(162, 131) = PC_IF2DEC_RI.read();
        dec2exe_in_var[130]            = 0;
        dec2exe_in_var[129]            = 0;
        dec2exe_in_var.range(128, 123) = 0;
        dec2exe_in_var.range(122, 117) = 0;
        dec2exe_in_var.range(116, 115) = 0;
        dec2exe_in_var.range(114, 83)  = 0;
        dec2exe_in_var.range(82, 51)   = 0;
        dec2exe_in_var[50]             = 0;
        dec2exe_in_var[49]             = 0;
        dec2exe_in_var.range(48, 17)   = 0;
        dec2exe_in_var[16]             = 0;
        dec2exe_in_var[15]             = 0;
        dec2exe_in_var[14]             = 0;
        dec2exe_in_var.range(13, 12)   = 0;
        dec2exe_in_var.range(11, 8)    = 0;
        dec2exe_in_var.range(7, 2)     = 0;
        dec2exe_in_var[1]              = 0;
        dec2exe_in_var[0]              = 0;
    }

    dec2exe_in_sd.write(dec2exe_in_var);
}

void decod::unconcat_dec2exe() {
    sc_bv<dec2exe_size> dec2exe_out_var = dec2exe_out_sd.read();

    PC_BRANCH_VALUE_RD.write((sc_bv_base)dec2exe_out_var.range(251, 220));
    MULT_INST_RD.write((bool)dec2exe_out_var[219]);
    EBREAK_RD.write((bool)dec2exe_out_var[218]);
    INSTRUCTION_ACCESS_FAULT_RD.write((bool)dec2exe_out_var[217]);
    MRET_RD.write((bool)dec2exe_out_var[216]);
    BLOCK_BP_RD.write((bool)dec2exe_out_var[215]);
    EXCEPTION_RD.write((bool)dec2exe_out_var[214]);
    ENV_CALL_WRONG_MODE_RD.write((bool)dec2exe_out_var[213]);
    ENV_CALL_U_MODE_RD.write((bool)dec2exe_out_var[212]);
    ILLEGAL_INSTRUCTION_RD.write((bool)dec2exe_out_var[211]);
    ADRESS_MISSALIGNED_RD.write((bool)dec2exe_out_var[210]);
    ENV_CALL_M_MODE_RD.write((bool)dec2exe_out_var[209]);
    ENV_CALL_S_MODE_RD.write((bool)dec2exe_out_var[208]);
    CSR_RDATA_RD.write((sc_bv_base)dec2exe_out_var.range(207, 176));
    CSR_WENABLE_RD.write((bool)dec2exe_out_var[175]);
    CSR_WADR_RD.write((sc_bv_base)dec2exe_out_var.range(174, 163));
    PC_DEC2EXE_RD.write((sc_bv_base)dec2exe_out_var.range(162, 131));
    BP_R1_VALID_RD.write((bool)dec2exe_out_var[130]);
    BP_R2_VALID_RD.write((bool)dec2exe_out_var[129]);

    BP_RADR1_RD.write((sc_bv_base)dec2exe_out_var.range(128, 123));
    BP_RADR2_RD.write((sc_bv_base)dec2exe_out_var.range(122, 117));

    EXE_CMD_RD.write((sc_bv_base)dec2exe_out_var.range(116, 115));
    OP1_RD.write((sc_bv_base)dec2exe_out_var.range(114, 83));
    OP2_RD.write((sc_bv_base)dec2exe_out_var.range(82, 51));
    NEG_OP2_RD.write((bool)dec2exe_out_var[50]);
    WB_RD.write((bool)dec2exe_out_var[49]);

    MEM_DATA_RD.write((sc_bv_base)dec2exe_out_var.range(48, 17));

    MEM_LOAD_RD.write((bool)dec2exe_out_var[16]);
    MEM_STORE_RD.write((bool)dec2exe_out_var[15]);

    MEM_SIGN_EXTEND_RD.write((bool)dec2exe_out_var[14]);
    MEM_SIZE_RD.write((sc_bv_base)dec2exe_out_var.range(13, 12));
    SELECT_TYPE_OPERATIONS_RD.write((sc_bv_base)dec2exe_out_var.range(11, 8));
    EXE_DEST_SD.write((sc_bv_base)dec2exe_out_var.range(7, 2));
    SLT_RD.write((bool)dec2exe_out_var[1]);
    SLTU_RD.write((bool)dec2exe_out_var[0]);
}

//---------------------------------------------INSTRUCTION TYPE DETECTION
//:---------------------------------------------

void decod::decoding_instruction_type() {
    sc_uint<32> if_ir   = INSTR_RI.read();
    r_type_inst_sd      = (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) != 0b0000001) ? 1 : 0;
    i_type_inst_sd      = (if_ir.range(6, 0) == 0b0010011 | if_ir.range(6, 0) == 0b0000011) ? 1 : 0;
    s_type_inst_sd      = if_ir.range(6, 0) == 0b0100011 ? 1 : 0;
    b_type_inst_sd      = if_ir.range(6, 0) == 0b1100011 ? 1 : 0;
    u_type_inst_sd      = (if_ir.range(6, 0) == 0b0110111 || if_ir.range(6, 0) == 0b0010111) ? 1 : 0;
    j_type_inst_sd      = if_ir.range(6, 0) == 0b1101111 ? 1 : 0;
    jalr_type_inst_sd   = if_ir.range(6, 0) == 0b1100111 ? 1 : 0;
    system_type_inst_sd = if_ir.range(6, 0) == 0b1110011 ? 1 : 0;
    m_type_inst_sd      = (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000001) ? 1 : 0;
}

//---------------------------------------------INSTRUCTION DETECTION
//:---------------------------------------------

void decod::decoding_instruction() {
    sc_uint<32> if_ir = INSTR_RI.read();

    // R-type Instruction :

    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b000)
        add_i_sd.write(1);
    else
        add_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b010)
        slt_i_sd.write(1);
    else
        slt_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b011)
        sltu_i_sd.write(1);
    else
        sltu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b111)
        and_i_sd.write(1);
    else
        and_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b110)
        or_i_sd.write(1);
    else
        or_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b100)
        xor_i_sd.write(1);
    else
        xor_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b001)
        sll_i_sd.write(1);
    else
        sll_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b101)
        srl_i_sd.write(1);
    else
        srl_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0100000 && if_ir.range(14, 12) == 0b000)
        sub_i_sd.write(1);
    else
        sub_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(31, 25) == 0b0100000 && if_ir.range(14, 12) == 0b101)
        sra_i_sd.write(1);
    else
        sra_i_sd.write(0);

    // I-type Instructions :

    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b000)
        addi_i_sd.write(1);
    else
        addi_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b010)
        slti_i_sd.write(1);
    else
        slti_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b011)
        sltiu_i_sd.write(1);
    else
        sltiu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b111)
        andi_i_sd.write(1);
    else
        andi_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b110)
        ori_i_sd.write(1);
    else
        ori_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(14, 12) == 0b100)
        xori_i_sd.write(1);
    else
        xori_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0001111 && if_ir.range(14, 12) == 0b000)
        fence_i_sd.write(1);
    else
        fence_i_sd.write(0);

    // I-type shift instructions :

    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b001)
        slli_i_sd.write(1);
    else
        slli_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0000000 && if_ir.range(14, 12) == 0b101)
        srli_i_sd.write(1);
    else
        srli_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010011 && if_ir.range(31, 25) == 0b0100000 && if_ir.range(14, 12) == 0b101)
        srai_i_sd.write(1);
    else
        srai_i_sd.write(0);

    // I-type load instructions :

    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b010)
        lw_i_sd.write(1);
    else
        lw_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b001)
        lh_i_sd.write(1);
    else
        lh_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b101)
        lhu_i_sd.write(1);
    else
        lhu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b000)
        lb_i_sd.write(1);
    else
        lb_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0000011 && if_ir.range(14, 12) == 0b100)
        lbu_i_sd.write(1);
    else
        lbu_i_sd.write(0);

    // B-type Instructions :

    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b000)
        beq_i_sd.write(1);
    else
        beq_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b001)
        bne_i_sd.write(1);
    else
        bne_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b100)
        blt_i_sd.write(1);
    else
        blt_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b101)
        bge_i_sd.write(1);
    else
        bge_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b110)
        bltu_i_sd.write(1);
    else
        bltu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100011 && if_ir.range(14, 12) == 0b111)
        bgeu_i_sd.write(1);
    else
        bgeu_i_sd.write(0);

    // U-type Instructions :

    if (if_ir.range(6, 0) == 0b0110111)
        lui_i_sd.write(1);
    else
        lui_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0010111)
        auipc_i_sd.write(1);
    else
        auipc_i_sd.write(0);

    // J-type Instructions :

    if (if_ir.range(6, 0) == 0b1101111)
        jal_i_sd.write(1);
    else
        jal_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1100111)
        jalr_i_sd.write(1);
    else
        jalr_i_sd.write(0);

    // S-type Instructions :

    if (if_ir.range(6, 0) == 0b0100011 && if_ir.range(14, 12) == 0b010)
        sw_i_sd.write(1);
    else
        sw_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0100011 && if_ir.range(14, 12) == 0b001)
        sh_i_sd.write(1);
    else
        sh_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0100011 && if_ir.range(14, 12) == 0b000)
        sb_i_sd.write(1);
    else
        sb_i_sd.write(0);

    // System-type Instructions :

    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b000 && if_ir.range(31, 20) == 0b000000000000)
        ecall_i_sd.write(1);
    else
        ecall_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b000 && if_ir.range(31, 20) == 0b000000000001)
        ebreak_i_sd.write(1);
    else
        ebreak_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b001)
        csrrw_i_sd.write(1);
    else
        csrrw_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b010)
        csrrs_i_sd.write(1);
    else
        csrrs_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b011)
        csrrc_i_sd.write(1);
    else
        csrrc_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b101)
        csrrwi_i_sd.write(1);
    else
        csrrwi_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b110)
        csrrsi_i_sd.write(1);
    else
        csrrsi_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b1110011 && if_ir.range(14, 12) == 0b111)
        csrrci_i_sd.write(1);
    else
        csrrci_i_sd.write(0);
    if (if_ir == 0b00110000001000000000000001110011)
        mret_i_sd.write(1);
    else
        mret_i_sd.write(0);
    if (if_ir == 0b00010000001000000000000001110011)
        sret_i_sd.write(1);
    else
        sret_i_sd.write(0);

    // M-type Instructions :

    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(14, 12) == 0b000 && if_ir.range(31, 25) == 0b0000001)
        mul_i_sd.write(1);
    else
        mul_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(14, 12) == 0b001 && if_ir.range(31, 25) == 0b0000001)
        mulh_i_sd.write(1);
    else
        mulh_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(14, 12) == 0b010 && if_ir.range(31, 25) == 0b0000001)
        mulhsu_i_sd.write(1);
    else
        mulhsu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(14, 12) == 0b011 && if_ir.range(31, 25) == 0b0000001)
        mulhu_i_sd.write(1);
    else
        mulhu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(14, 12) == 0b100 && if_ir.range(31, 25) == 0b0000001)
        div_i_sd.write(1);
    else
        div_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(14, 12) == 0b101 && if_ir.range(31, 25) == 0b0000001)
        divu_i_sd.write(1);
    else
        divu_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(14, 12) == 0b110 && if_ir.range(31, 25) == 0b0000001)
        rem_i_sd.write(1);
    else
        rem_i_sd.write(0);
    if (if_ir.range(6, 0) == 0b0110011 && if_ir.range(14, 12) == 0b111 && if_ir.range(31, 25) == 0b0000001)
        remu_i_sd.write(1);
    else
        remu_i_sd.write(0);
}

//---------------------------------------------REGISTRE & OPERAND DETECTION
//:---------------------------------------------
// this needs to be done in two steps :
void decod::pre_reg_read_decoding() {
    sc_uint<32> if_ir = INSTR_RI.read();
    sc_uint<6>  radr1_var;
    sc_uint<6>  radr2_var;
    sc_uint<6>  adr_dest_var;

    // R-type Instruction :
    if (r_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = if_ir.range(11, 7);
        CSR_RADR_SD.write(0);
    }
    // I-type Instruction :
    else if (i_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = 0;
        adr_dest_var = if_ir.range(11, 7);
        CSR_RADR_SD.write(0);
    }
    // S-type Instruction :
    else if (s_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = 0;
        CSR_RADR_SD.write(0);
    }
    // Branch Instruction :
    else if (b_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = 0;
        CSR_RADR_SD.write(0);
    }
    // U-type Instruction :
    else if (u_type_inst_sd == 1) {
        radr1_var    = 0;
        adr_dest_var = if_ir.range(11, 7);
        radr2_var    = 0;
        CSR_RADR_SD.write(0);
    }
    // J-type Instruction :
    else if (j_type_inst_sd == 1) {
        radr1_var    = 0;
        radr2_var    = 0;
        adr_dest_var = if_ir.range(11, 7);
        CSR_RADR_SD.write(0);
    }
    // JALR-type Instruction :
    else if (jalr_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = 0;
        adr_dest_var = if_ir.range(11, 7);
        CSR_RADR_SD.write(0);
    }
    // M-type Instruction :
    else if (m_type_inst_sd == 1) {
        radr1_var    = if_ir.range(19, 15);
        radr2_var    = if_ir.range(24, 20);
        adr_dest_var = if_ir.range(11, 7);
    }
    // system-type Instruction
    else if (system_type_inst_sd == 1) {
        // in CSR operation we always have :
        // rd = CSR
        // CSR = (rs1 | 0) operation CSR
        // So CSR must be wbk in rd
        if (csrrw_i_sd || csrrs_i_sd || csrrc_i_sd || csrrwi_i_sd || csrrsi_i_sd || csrrci_i_sd) {
            CSR_RADR_SD.write(if_ir.range(31, 20));
            csr_radr_sd  = if_ir.range(31, 20);
            radr1_var    = (csrrw_i_sd | csrrs_i_sd | csrrc_i_sd) ? if_ir.range(19, 15) : 0;
            radr2_var    = 0;
            adr_dest_var = if_ir.range(11, 7);
        } else if (ecall_i_sd || ebreak_i_sd) {
            radr1_var    = 0;
            radr2_var    = 0;
            adr_dest_var = 0;

            // Generate the exception corresponding to the right
            // call

            if (CURRENT_MODE_SM.read() == 0 && ecall_i_sd) {
                env_call_u_mode_sd = 1;
                env_call_s_mode_sd = 0;
                env_call_m_mode_sd = 0;
            } else if (ecall_i_sd && CURRENT_MODE_SM.read() == 3) {
                env_call_u_mode_sd = 0;
                env_call_s_mode_sd = 0;
                env_call_m_mode_sd = 1;
            } else {
                cout << "other " << sc_time_stamp() << endl;
                env_call_u_mode_sd = 0;
                env_call_s_mode_sd = 0;
                env_call_m_mode_sd = 0;
            }

        } else if (sret_i_sd || mret_i_sd) {
            // xRET instruction read the current value of MEPC register
            radr1_var    = 0;
            radr2_var    = 0;
            adr_dest_var = 0;

            // Exception will be generate in case of mret in wrong mode

            if (mret_i_sd && CURRENT_MODE_SM.read() != 3) {
                env_call_wrong_mode = 1;
            } else if (sret_i_sd && CURRENT_MODE_SM.read() != 2)
                env_call_s_mode_sd = 1;
            else {
                env_call_wrong_mode = 0;
            }
        }
    }
    // Default case :
    else {
        radr1_var    = 0;
        radr2_var    = 0;
        adr_dest_var = 0;
    }
    if (!ecall_i_sd) {
        env_call_u_mode_sd = 0;
        env_call_s_mode_sd = 0;
        env_call_m_mode_sd = 0;
    }
    if (!mret_i_sd | !sret_i_sd) { env_call_wrong_mode = 0; }
    RADR1_SD.write(radr1_var);
    RADR2_SD.write(radr2_var);
    adr_dest_sd.write(adr_dest_var);
}

//---------------------------------------------EXE & MEM SIGNAL DETECTION
//:---------------------------------------------

void decod::post_reg_read_decoding() {
    // We are going to setup commands sent to EXE here, so each if will be
    // execute for one type of command :

    // CMD : +
    bool        dec2exe_wb_var;
    sc_uint<32> dec2exe_op1_var;
    sc_uint<32> dec2exe_op2_var;
    sc_uint<32> if_ir = INSTR_RI.read();
    sc_uint<32> mem_data_var;
    sc_uint<32> offset_branch_var;
    bool        not_jump_var;
    bool        illegal_inst = false;

    // R-type Instruction :

    if (r_type_inst_sd || i_type_inst_sd || u_type_inst_sd || m_type_inst_sd) {
        csr_wenable_sd.write(0);
        if (i_type_inst_sd) {
            dec2exe_op1_var = (rdata1_sd.read());
            if (if_ir.range(31, 31) == 1) {
                dec2exe_op2_var.range(31, 12) = 0b11111111111111111111;
            } else {
                dec2exe_op2_var.range(31, 12) = 0b00000000000000000000;
            }
            dec2exe_op2_var.range(11, 0) = if_ir.range(31, 20);
        } else if (u_type_inst_sd) {
            dec2exe_op1_var.range(31, 12) = if_ir.range(31, 12);
            dec2exe_op1_var.range(11, 0)  = 0;
            if (auipc_i_sd)
                dec2exe_op2_var = PC_IF2DEC_RI.read();
            else
                dec2exe_op2_var = rdata2_sd.read();
        } else {
            dec2exe_op1_var = (rdata1_sd.read());
            dec2exe_op2_var = (rdata2_sd.read());
        }

        if (lw_i_sd | lh_i_sd | lhu_i_sd | lb_i_sd | lbu_i_sd) {
            mem_load_sd.write(1);
            if (lw_i_sd) {
                mem_size_sd.write(0);
                mem_sign_extend_sd.write(0);
            } else if (lh_i_sd | lhu_i_sd) {
                mem_size_sd.write(1);
                if (lhu_i_sd)
                    mem_sign_extend_sd.write(0);
                else
                    mem_sign_extend_sd.write(1);
            } else if (lb_i_sd | lbu_i_sd) {
                mem_size_sd.write(2);
                if (lbu_i_sd)
                    mem_sign_extend_sd.write(0);
                else
                    mem_sign_extend_sd.write(1);
            }
        } else {
            mem_load_sd.write(0);
            mem_size_sd.write(0);
            mem_sign_extend_sd.write(0);
        }
        mem_store_sd.write(0);
        dec2exe_wb_var    = 1;
        mem_data_var      = 0;
        offset_branch_var = 0;
        not_jump_var        = 1;

        // sub
        exe_neg_op2_sd.write(sub_i_sd | slt_i_sd | slti_i_sd | sltu_i_sd | sltiu_i_sd);

        // Command for exe
        if (and_i_sd || andi_i_sd || srl_i_sd || srli_i_sd || mul_i_sd || div_i_sd)
            exe_cmd_sd.write(1);
        else if (or_i_sd || ori_i_sd || sra_i_sd || srai_i_sd || mulh_i_sd || mulhsu_i_sd || mulhu_i_sd || divu_i_sd)
            exe_cmd_sd.write(2);
        else if (xor_i_sd || xori_i_sd || rem_i_sd)
            exe_cmd_sd.write(3);
        else
            exe_cmd_sd.write(0);

        if (div_i_sd || divu_i_sd || rem_i_sd || remu_i_sd)
            select_type_operations_sd.write(0b1000);
        else if (mul_i_sd || mulh_i_sd || mulhsu_i_sd || mulhu_i_sd)
            select_type_operations_sd.write(0b0100);
        else if (sll_i_sd || slli_i_sd || srl_i_sd || srli_i_sd || sra_i_sd || srai_i_sd)
            select_type_operations_sd.write(0b0010);
        else
            select_type_operations_sd.write(0b0001);
    } else if (s_type_inst_sd) {
        csr_wenable_sd.write(0);
        exe_cmd_sd.write(0);
        select_type_operations_sd.write(0b0001);
        exe_neg_op2_sd.write(0);
        mem_load_sd.write(0);
        offset_branch_var = 0;
        not_jump_var        = 1;
        dec2exe_op1_var   = rdata1_sd.read();

        // The adress is obtained by adding rs1 to the 12 bit immediat sign
        // extended
        // rs2 is copied to the memory
        if (if_ir.range(31, 31) == 1) {
            dec2exe_op2_var.range(31, 12) = 0b11111111111111111111;
        } else {
            dec2exe_op2_var.range(31, 12) = 0b00000000000000000000;
        }
        dec2exe_op2_var.range(11, 5) = if_ir.range(31, 25);
        dec2exe_op2_var.range(4, 0)  = if_ir.range(11, 7);

        mem_data_var = rdata2_sd.read();

        // STORE :

        if (sw_i_sd | sh_i_sd | sb_i_sd) {
            mem_store_sd.write(1);
            if (sw_i_sd) {
                mem_size_sd.write(0);
            } else if (sh_i_sd) {
                mem_size_sd.write(1);
            } else if (sb_i_sd) {
                mem_size_sd.write(2);
            }
        } else {
            mem_size_sd.write(0);
            mem_store_sd.write(0);
        }
    } else if (b_type_inst_sd == 1) {
        csr_wenable_sd.write(0);
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        dec2exe_wb_var = 1;
        mem_data_var   = 0;
        exe_cmd_sd.write(0);
        select_type_operations_sd.write(0b0001);
        exe_neg_op2_sd.write(0);
        // Offset must be *4, so he's shift by 2 on the left

        if (if_ir.range(31, 31) == 1) {
            offset_branch_var.range(31, 13) = 0b1111111111111111111;
        } else {
            offset_branch_var.range(31, 13) = 0b0000000000000000000;
        }
        offset_branch_var.range(12, 12) = if_ir.range(31, 31);
        offset_branch_var.range(11, 11) = if_ir.range(7, 7);
        offset_branch_var.range(10, 5)  = if_ir.range(30, 25);
        offset_branch_var.range(4, 1)   = if_ir.range(11, 8);
        offset_branch_var.range(0, 0)   = 0;

        /*BRANCH CONDITION GESTION : */

        dec2exe_op1_var = rdata1_sd.read();
        dec2exe_op2_var = rdata2_sd.read();
        sc_uint<33> res_comparaison;
        res_comparaison     = dec2exe_op1_var - dec2exe_op2_var;
        bool different_sign = dec2exe_op1_var[31] ^ dec2exe_op2_var[31];
        bool equal          = res_comparaison == 0;
        bool branch;
        if (bne_i_sd.read()) {
            branch = !equal;
        } else if (beq_i_sd.read()) {
            branch = equal;
        } else if (blt_i_sd.read()) {
            if (different_sign)
                branch = dec2exe_op1_var[31];  // branch if the first number is negative
            else
                branch = res_comparaison[31];  // branch if the result is negative
        } else if (bltu_i_sd.read()) {
            if (different_sign)
                branch = dec2exe_op2_var[31];  // branch if the second number is bigger
            else
                branch = res_comparaison[31];  // branch if the result is negative
        } else if (bge_i_sd.read()) {
            if (different_sign)
                branch = dec2exe_op2_var[31];  // branch if the second number is negative
            else
                branch = !res_comparaison[31];  // branch if the result is positive
        } else if (bgeu_i_sd.read()) {
            if (different_sign)
                branch = dec2exe_op1_var[31];  // branch if the second number is negative
            else
                branch = !res_comparaison[31];  // branch if the result is positive
        }
        not_jump_var = !branch;
    }

    else if (jalr_type_inst_sd.read() || j_type_inst_sd.read()) {
        csr_wenable_sd.write(0);
        exe_cmd_sd.write(0);
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 1;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_type_operations_sd.write(1);
        mem_data_var = 0;
        if (jalr_type_inst_sd) {
            dec2exe_op1_var = READ_PC_SR.read();
            dec2exe_op2_var = 0x0;

            if (if_ir.range(31, 31) == 1) {
                offset_branch_var.range(31, 12) = 0b11111111111111111111;
            } else {
                offset_branch_var.range(31, 12) = 0b00000000000000000000;
            }
            offset_branch_var.range(11, 0) = if_ir.range(31, 20);
            offset_branch_var += rdata1_sd.read() - READ_PC_SR.read() + 4;
            offset_branch_var.range(0, 0) = 0;
            mem_data_var                  = 0;
            not_jump_var                    = 0;
        } else {
            dec2exe_op1_var = READ_PC_SR.read();
            dec2exe_op2_var = 0x0;  // on va envoyer l'adresse de retour

            if (if_ir.range(31, 31) == 1) {
                offset_branch_var.range(31, 21) = 0b11111111111;
            } else {
                offset_branch_var.range(31, 21) = 0b00000000000;
            }
            offset_branch_var.range(20, 20) = if_ir.range(31, 31);
            offset_branch_var.range(19, 12) = if_ir.range(19, 12);
            offset_branch_var.range(11, 11) = if_ir.range(20, 20);
            offset_branch_var.range(10, 1)  = if_ir.range(30, 21);
            offset_branch_var.range(0, 0)   = 0;
            mem_data_var                    = 0;
            not_jump_var                      = 0;
        }
    } else if (system_type_inst_sd == 1) {
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_type_operations_sd.write(1);
        // in CSR operation we always have :
        // rd = CSR
        // CSR = (rs1 | 0) operation CSR
        // So CSR must be wbk in rd
        if (csrrw_i_sd || csrrs_i_sd || csrrc_i_sd || csrrwi_i_sd || csrrsi_i_sd || csrrci_i_sd) {
            csr_wenable_sd.write(1);
            sc_uint<32> rdata1_signal_sd = rdata1_sd;  // loading value of rs1
            if (csrrc_i_sd || csrrci_i_sd) {
                exe_neg_op2_sd.write(0);
                exe_cmd_sd.write(1);
            } else if (csrrw_i_sd || csrrwi_i_sd) {
                exe_neg_op2_sd.write(0);
                exe_cmd_sd.write(0);
            } else {
                exe_neg_op2_sd.write(0);
                exe_cmd_sd.write(2);
            }

            // We put rdata1 or immediat value in rs1

            if (csrrw_i_sd | csrrc_i_sd | csrrs_i_sd) {
                if (!csrrc_i_sd)
                    dec2exe_op1_var = rdata1_sd;
                else
                    dec2exe_op1_var = ~rdata1_sd.read();
            } else {
                if (!csrrci_i_sd)
                    dec2exe_op1_var = if_ir.range(19, 15);
                else
                    dec2exe_op1_var = ~if_ir.range(19, 15);
            }

            // We put CSR value or 0 inside rs2
            // If instruction are write one, the value of rs1 just
            // erase what's inside the csr
            if (csrrs_i_sd || csrrc_i_sd || csrrsi_i_sd || csrrci_i_sd) {
                dec2exe_op2_var = CSR_RDATA_SC;
            } else {
                dec2exe_op2_var = 0;
            }

            dec2exe_wb_var    = 1;
            offset_branch_var = 0;
            mem_data_var      = 0;
            not_jump_var        = 1;

        } else if (ecall_i_sd || ebreak_i_sd) {
            csr_wenable_sd.write(0);
            exe_cmd_sd.write(0);
            dec2exe_op1_var = 0;
            dec2exe_op2_var = 0;
            exe_neg_op2_sd.write(0);
            dec2exe_wb_var = 0;
            mem_load_sd.write(0);
            mem_store_sd.write(0);
            mem_sign_extend_sd.write(0);
            mem_size_sd.write(0);
            offset_branch_var = 0;
            mem_data_var      = 0;
            not_jump_var        = 1;
        } else if (sret_i_sd || mret_i_sd) {
            csr_wenable_sd.write(0);
            exe_cmd_sd.write(0);
            dec2exe_op1_var = 0;
            dec2exe_op2_var = 0;
            exe_neg_op2_sd.write(0);
            dec2exe_wb_var = 0;
            mem_load_sd.write(0);
            mem_store_sd.write(0);
            mem_sign_extend_sd.write(0);
            mem_size_sd.write(0);
            select_type_operations_sd.write(1);
            offset_branch_var = 0;
            mem_data_var      = 0;
            not_jump_var      = 1;
        }
    } else if (fence_i_sd) {
        csr_wenable_sd.write(0);
        exe_cmd_sd.write(0);
        dec2exe_op1_var = 0;
        dec2exe_op2_var = 0;
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 0;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_type_operations_sd.write(0b0001);
        offset_branch_var = 0;
        mem_data_var      = 0;
        not_jump_var        = 1;
    } else {
        csr_wenable_sd.write(0);
        exe_cmd_sd.write(0);
        dec2exe_op1_var = 0;
        dec2exe_op2_var = 0;
        exe_neg_op2_sd.write(0);
        dec2exe_wb_var = 0;
        mem_load_sd.write(0);
        mem_store_sd.write(0);
        mem_sign_extend_sd.write(0);
        mem_size_sd.write(0);
        select_type_operations_sd.write(0b0001);
        illegal_inst = true;
    }

    // Illegal instruction Gestion :

    if (r_type_inst_sd) {
        if (if_ir.range(31, 25) != 0 && if_ir.range(31, 25) != 0b0100000) { illegal_inst = true; }
    } else if (b_type_inst_sd) {
        if (if_ir.range(14, 12) == 2 || if_ir.range(14, 12) == 3) illegal_inst = true;
    }

    else if (s_type_inst_sd) {
        if (if_ir.range(14, 12) > 2) illegal_inst = true;
    } else if (system_type_inst_sd)
        if (if_ir.range(14, 12) == 4) illegal_inst = true;

    illegal_inst = illegal_inst && !IF2DEC_EMPTY_SI.read();

    illegal_instruction_sd.write(illegal_inst);
    block_bp_sd.write(jalr_type_inst_sd);
    exe_wb_sd.write(dec2exe_wb_var);
    offset_branch_sd.write(offset_branch_var);
    exe_op1_sd.write(dec2exe_op1_var);
    exe_op2_sd.write(dec2exe_op2_var);
    mem_data_sd.write(mem_data_var);
    // inc_pc_sd.write(((inc_pc_var || IF2DEC_EMPTY_SI) && dec2if_push_sd.read()) && !EXCEPTION_SM);
    // add_offset_to_pc_sd.write((!stall_sd && !inc_pc_var && (b_type_inst_sd || j_type_inst_sd 
    //                             || jalr_type_inst_sd) &&
    //                            dec2if_push_sd.read() && !illegal_inst && !IF2DEC_EMPTY_SI) &&
    //                           !EXCEPTION_SM);
    jump_sd = !not_jump_var ; 
}

//---------------------------------------------PC GESTION
//:---------------------------------------------

void decod::pc_inc() {
    sc_uint<32> pc                = READ_PC_SR.read();
    sc_uint<32> pc_out            = pc;
    sc_uint<32> offset_branch_var = offset_branch_sd.read();
    bool add_offset_to_pc = jump_sd.read() && !IF2DEC_EMPTY_SI ;
    

    // PC Incrementation

    if (!add_offset_to_pc && !dec2if_full_sd ) {
        pc_out = pc + 4;
        WRITE_PC_ENABLE_SD  = 1;
        dec2if_push_sd      = 1;
    } else if (add_offset_to_pc && !dec2if_full_sd && !stall_sd) {
        pc_out = PC_IF2DEC_RI.read() + offset_branch_var;
        WRITE_PC_ENABLE_SD  = 1;
        dec2if_push_sd      = 1;
    } else {
        WRITE_PC_ENABLE_SD  = 0;
        dec2if_push_sd      = 0;
    }

        DEC2IF_EMPTY_SD     = dec2if_empty_sd ;
    
    if (EXCEPTION_SM.read() == 0 && EXCEPTION_SM.read() != 1) {
        dec2if_in_sd.write(pc_out);
        WRITE_PC_SD.write(pc_out);
        pc_branch_value_sd = pc_out ;
        if (pc_out > start_kernel_adress && CURRENT_MODE_SM.read() != 3) {
            instruction_access_fault_sd = 1;
        } else {
            instruction_access_fault_sd = 0;
        }
    }
    //Instruction adress missaligned exception :
    if ((pc_out & 0b11) != 0 || (((RETURN_ADRESS_SM.read() & 0b11) != 0) && EXCEPTION_SM.read()))
    {
        instruction_adress_missaligned_sd = 1;
    } 
    else{
        instruction_adress_missaligned_sd = 0;
    }

    // Exception & fifo gestion

    if (EXCEPTION_SM.read() == 1) {
        if (!MRET_SM) {
            // Need to check MTVEC value, bits 1,0 indicate :
            // =0 -> direct type, so pc just get value of mtvec.range(31,2)
            // =1 -> vectorise, so pc get value of mtvec.range(31,2) + 4*mcause
            sc_uint<32> dec2if_var;
            sc_uint<32> WRITE_PC_VAR;
            sc_uint<32> MTVEC_VALUE_VAR;

            MTVEC_VALUE_VAR.range(31, 2) = MTVEC_VALUE_RC.read().range(31, 2);
            MTVEC_VALUE_VAR.range(1, 0)  = 0;

            if (MTVEC_VALUE_RC.read().range(1, 0) == 0) {  // direct
                dec2if_in_sd = MTVEC_VALUE_VAR;
                WRITE_PC_SD  = MTVEC_VALUE_VAR;
                WRITE_PC_ENABLE_SD.write(1);
            } else if (MTVEC_VALUE_RC.read().range(1, 0) == 1) {  // vectorise
                sc_uint<32> MCAUSE_VAR;
                // MCAUSE * 4 :
                MCAUSE_VAR.range(31, 2) = MCAUSE_WDATA_SM.read().range(29, 0);
                MCAUSE_VAR.range(1, 0)  = 0;
                dec2if_in_sd.write(MCAUSE_VAR + MTVEC_VALUE_VAR);
                WRITE_PC_SD.write(MCAUSE_VAR + MTVEC_VALUE_VAR);
                WRITE_PC_ENABLE_SD.write(1);
            }

        } else {
            dec2if_in_sd.write(RETURN_ADRESS_SM.read());
            WRITE_PC_SD.write(RETURN_ADRESS_SM.read());
            WRITE_PC_ENABLE_SD.write(1);
        }          
            
        
        // IF2DEC Gestion
        
        IF2DEC_POP_SD.write(1);
        IF2DEC_FLUSH_SD.write(0);

        // DEC2EXE Gestion

        dec2exe_push_sd.write(1);

    }
    else{
        
        // IF2DEC Gestion
        
        if (jump_sd.read() && !stall_sd) {
            IF2DEC_POP_SD.write(1);
            IF2DEC_FLUSH_SD.write(1);
        } else if (!jump_sd && !stall_sd) {
            IF2DEC_POP_SD.write(1);
            IF2DEC_FLUSH_SD.write(0);
        } else {
            IF2DEC_POP_SD.write(0);
            IF2DEC_FLUSH_SD.write(0);
        }

        // DEC2EXE Gestion
        
        if (stall_sd) {
            dec2exe_push_sd.write(0);
        } else {
            dec2exe_push_sd.write(1);
        }
    }
}

void decod::bypasses() {
    if (RADR1_SD.read() == 0) {  // ignore r0
        rdata1_sd.write(RDATA1_SR.read());
        r1_valid_sd.write(true);
    } 
    else if (RADR1_SD.read() == EXE_DEST_SD.read() && !DEC2EXE_EMPTY_SD.read()) 
    {  // dont bypass if instr is currently in exe
        r1_valid_sd.write(false);
    } 
    else if (RADR1_SD.read() == BP_DEST_RE.read() && BP_MEM_LOAD_RE.read() && !BP_EXE2MEM_EMPTY_SE) 
    {  // dont bypass if load instr is currently in mem
        r1_valid_sd.write(false);
    } 
    else if(RADR1_SD.read() == BP_DEST_RE.read() && MULT_INST_RE && !BP_EXE2MEM_EMPTY_SE.read())
    { // dont bypass if mul instruction didnt finish
        r1_valid_sd.write(false);
    } 
    else if(RADR1_SD.read() == BP_DEST_RM.read() && MULT_INST_RM && !BP_MEM2WBK_EMPTY_SM.read())
    { // dont bypass if mul instruction didnt finish
        r1_valid_sd.write(false);
    } 
    else if (RADR1_SD.read() == BP_DEST_RE.read() && !BP_EXE2MEM_EMPTY_SE) 
    {  // bypass E->D
        r1_valid_sd.write(true);
        if (CSR_WENABLE_RE.read())
            rdata1_sd.write(CSR_RDATA_RE.read());
        else
            rdata1_sd.write(BP_EXE_RES_RE.read());
    } 
    else if (RADR1_SD.read() == BP_DEST_RM.read() && !BP_MEM2WBK_EMPTY_SM.read()) 
    {  // bypass M->D
        r1_valid_sd.write(true);
        if (CSR_WENABLE_RM.read())
            rdata1_sd.write(CSR_RDATA_RM.read());
        else
            rdata1_sd.write(BP_MEM_RES_RM.read());
    } 
    else {  // no bypass
        r1_valid_sd.write(true);
        rdata1_sd.write(RDATA1_SR.read());
    }

    if (RADR2_SD.read() == 0) {  // ignore r0
        rdata2_sd.write(RDATA2_SR.read());
        r2_valid_sd.write(true);
    } else if (RADR2_SD.read() == EXE_DEST_SD.read() &&
               !DEC2EXE_EMPTY_SD.read()) {  // dont bypass if instr is currently in exe
        r2_valid_sd.write(false);
    } else if (RADR2_SD.read() == BP_DEST_RE.read() && BP_MEM_LOAD_RE.read() &&
               !BP_EXE2MEM_EMPTY_SE) {  // dont bypass if load instr is
                                        // currently in mem
        r2_valid_sd.write(false);
    } else if(RADR2_SD.read() == BP_DEST_RE.read() && MULT_INST_RE && !BP_EXE2MEM_EMPTY_SE.read()){ // dont bypass if mul instruction didnt finish
        r2_valid_sd.write(false);
    } else if(RADR2_SD.read() == BP_DEST_RM.read() && MULT_INST_RM && !BP_MEM2WBK_EMPTY_SM.read()){ // dont bypass if mul instruction didnt finish
        r2_valid_sd.write(false);
    } else if (RADR2_SD.read() == BP_DEST_RE.read() && !BP_EXE2MEM_EMPTY_SE) {  // bypass E->D
        r2_valid_sd.write(true);
        if (CSR_WENABLE_RE.read())
            rdata2_sd.write(CSR_RDATA_RE.read());
        else
            rdata2_sd.write(BP_EXE_RES_RE.read());
    } else if (RADR2_SD.read() == BP_DEST_RM.read() && !BP_MEM2WBK_EMPTY_SM.read()) {  // bypass M->D
        r2_valid_sd.write(true);
        if (CSR_WENABLE_RM.read())
            rdata2_sd.write(CSR_RDATA_RM.read());
        else
            rdata2_sd.write(BP_MEM_RES_RM.read());
    } else {  // no bypass
        r2_valid_sd.write(true);
        rdata2_sd.write(RDATA2_SR.read());
    }
    // When a load is in exe, we can block the pipeline now
    // Avoid an issue with load - load - add sequence
    block_in_dec.write((RADR1_SD.read() == EXE_DEST_SD.read() && MEM_LOAD_RD && !DEC2EXE_EMPTY_SD.read()) ||
                       (RADR2_SD.read() == EXE_DEST_SD.read() && MEM_LOAD_RD && !DEC2EXE_EMPTY_SD.read()));
}

void decod::stall_method() {
    csr_in_progress = (CSR_WENABLE_RD && !DEC2EXE_EMPTY_SD) || (CSR_WENABLE_RE && !BP_EXE2MEM_EMPTY_SE);
    stall_sd        = (csr_in_progress || ((!r1_valid_sd || !r2_valid_sd) &&
                      (b_type_inst_sd || jalr_type_inst_sd || j_type_inst_sd || block_in_dec))
                      || IF2DEC_EMPTY_SI || dec2exe_full_sd);
}

//---------------------------------------------METHOD TO TRACE SIGNALS
//:---------------------------------------------

void decod::trace(sc_trace_file* tf) {
    dec2if.trace(tf);
    dec2exe.trace(tf);
    sc_trace(tf, RDATA1_SR, GET_NAME(RDATA1_SR));
    sc_trace(tf, RDATA2_SR, GET_NAME(RDATA2_SR));

    sc_trace(tf, READ_PC_SR, GET_NAME(READ_PC_SR));  // value of r32 which is pc coming from REG

    sc_trace(tf, RADR1_SD, GET_NAME(RADR1_SD));  // adress of rs
    sc_trace(tf, RADR2_SD, GET_NAME(RADR2_SD));  // adress of rt

    sc_trace(tf, WRITE_PC_SD, GET_NAME(WRITE_PC_SD));
    sc_trace(tf, WRITE_PC_ENABLE_SD, GET_NAME(WRITE_PC_ENABLE_SD));

    // Interface with EXE :

    sc_trace(tf, OP1_RD, GET_NAME(OP1_RD));            // value of op1
    sc_trace(tf, OP2_RD, GET_NAME(OP2_RD));            // value of op2
    sc_trace(tf, EXE_CMD_RD, GET_NAME(EXE_CMD_RD));    // value of the command sent to exe
    sc_trace(tf, NEG_OP2_RD, GET_NAME(NEG_OP2_RD));    // say if we take the opposite of the op1 to do a
                                                       // substraction for example
    sc_trace(tf, WB_RD, GET_NAME(WB_RD));              // say if we plan to wbk the value of rd or no
    sc_trace(tf, EXE_DEST_SD, GET_NAME(EXE_DEST_SD));  // the destination register
    sc_trace(tf, SELECT_TYPE_OPERATIONS_RD, GET_NAME(SELECT_TYPE_OPERATIONS_RD));  // taille fifo entrée : 110
    sc_trace(tf, SLT_RD, GET_NAME(SLT_RD));
    sc_trace(tf, SLTU_RD, GET_NAME(SLTU_RD));
    sc_trace(tf, PC_DEC2EXE_RD, GET_NAME(PC_DEC2EXE_RD));  // PC link to the current decoded instruction

    sc_trace(tf, MEM_DATA_RD, GET_NAME(MEM_DATA_RD));    // data sent to mem for storage
    sc_trace(tf, MEM_LOAD_RD, GET_NAME(MEM_LOAD_RD));    // say to mem if we do a load
    sc_trace(tf, MEM_STORE_RD, GET_NAME(MEM_STORE_RD));  // say to mem if we do a store
    sc_trace(tf, MEM_SIGN_EXTEND_RD, GET_NAME(MEM_SIGN_EXTEND_RD));
    sc_trace(tf, MEM_SIZE_RD, GET_NAME(MEM_SIZE_RD));  // tells to mem if we do an acces in word, hw or byte

    sc_trace(tf, CSR_WENABLE_RD, GET_NAME(CSR_WENABLE_RD));  // indicate if we do a csr operation,
                                                             // if so need to WBK CSR in rd
    sc_trace(tf, CSR_WADR_RD, GET_NAME(CSR_WADR_RD));        // CSR adress sent to EXE, will allow to wbk csr in MEM
    sc_trace(tf, CSR_RDATA_RD, GET_NAME(CSR_RDATA_RD));      // CSR read data to be wb in register

    // Interface with DEC2IF :

    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));  // Ifecth say to decod if it wants a pop or no
    sc_trace(tf, DEC2IF_EMPTY_SD, GET_NAME(DEC2IF_EMPTY_SD));
    sc_trace(tf, PC_RD, GET_NAME(PC_RD));  // this value must also be sent to REG

    // Interface with IF2DEC :

    sc_trace(tf, PC_IF2DEC_RI, GET_NAME(PC_IF2DEC_RI));
    sc_trace(tf, INSTR_RI, GET_NAME(INSTR_RI));
    sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
    sc_trace(tf, IF2DEC_POP_SD, GET_NAME(IF2DEC_POP_SD));  // Decod says to IFETCH if it wants a pop or no
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));

    // Interface with DEC2EXE

    sc_trace(tf, DEC2EXE_POP_SE, GET_NAME(DEC2EXE_POP_SE));
    sc_trace(tf, DEC2EXE_EMPTY_SD, GET_NAME(DEC2EXE_EMPTY_SD));
    sc_trace(tf, dec2exe_out_sd, GET_NAME(dec2exe_out_sd));
    sc_trace(tf, MULT_INST_RD, GET_NAME(MULT_INST_RD));
    
    // Interface with CSR :

    sc_trace(tf, CSR_RADR_SD, GET_NAME(CSR_RADR_SD));    // CSR adress sent to CSR to get data
    sc_trace(tf, CSR_RDATA_SC, GET_NAME(CSR_RDATA_SC));  // data read from CSR

    // Bypasses

    sc_trace(tf, BP_DEST_RE, GET_NAME(BP_DEST_RE));
    sc_trace(tf, BP_EXE_RES_RE, GET_NAME(BP_EXE_RES_RE));
    sc_trace(tf, BP_MEM_LOAD_RE, GET_NAME(BP_MEM_LOAD_RE));
    sc_trace(tf, BP_EXE2MEM_EMPTY_SE, GET_NAME(BP_EXE2MEM_EMPTY_SE));
    sc_trace(tf, BP_MEM2WBK_EMPTY_SM, GET_NAME(BP_MEM2WBK_EMPTY_SM));
    sc_trace(tf, BP_DEST_RM, GET_NAME(BP_DEST_RM));
    sc_trace(tf, BP_MEM_RES_RM, GET_NAME(BP_MEM_RES_RM));
    sc_trace(tf, MULT_INST_RE, GET_NAME(MULT_INST_RE));
    sc_trace(tf, MULT_INST_RM, GET_NAME(MULT_INST_RM));
    
    sc_trace(tf, CSR_WENABLE_RE, GET_NAME(CSR_WENABLE_RE));
    sc_trace(tf, CSR_RDATA_RE, GET_NAME(CSR_RDATA_RE));
    sc_trace(tf, CSR_WENABLE_RM, GET_NAME(CSR_WENABLE_RM));
    sc_trace(tf, CSR_RDATA_RM, GET_NAME(CSR_RDATA_RM));

    sc_trace(tf, BP_R1_VALID_RD, GET_NAME(BP_R1_VALID_RD));
    sc_trace(tf, BP_R2_VALID_RD, GET_NAME(BP_R2_VALID_RD));
    sc_trace(tf, BP_RADR1_RD, GET_NAME(BP_RADR1_RD));
    sc_trace(tf, BP_RADR2_RD, GET_NAME(BP_RADR2_RD));

    // Exception :

    sc_trace(tf,
             EXCEPTION_RI,
             GET_NAME(EXCEPTION_RI));  // this signal will be at 0 considering there is no exception in IFETCH

    sc_trace(tf, ENV_CALL_U_MODE_RD, GET_NAME(ENV_CALL_U_MODE_RD));
    sc_trace(tf, ENV_CALL_WRONG_MODE_RD, GET_NAME(ENV_CALL_WRONG_MODE_RD));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RD, GET_NAME(ILLEGAL_INSTRUCTION_RD));  // instruction doesnt exist
    sc_trace(tf, ADRESS_MISSALIGNED_RD, GET_NAME(ADRESS_MISSALIGNED_RD));    // branch offset is misaligned
    sc_trace(tf, ENV_CALL_S_MODE_RD, GET_NAME(ENV_CALL_S_MODE_RD));
    sc_trace(tf, ENV_CALL_M_MODE_RD, GET_NAME(ENV_CALL_M_MODE_RD));

    sc_trace(tf, EXCEPTION_RD, GET_NAME(EXCEPTION_RD));
    // General Interface :

    sc_trace(tf, EXCEPTION_SM, GET_NAME(EXCEPTION_SM));
    sc_trace(tf, MTVEC_VALUE_RC, GET_NAME(MTVEC_VALUE_RC));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, INTERRUPTION_SE, GET_NAME(INTERRUPTION_SE));
    sc_trace(tf, rdata1_sd, GET_NAME(rdata1_sd));
    sc_trace(tf, rdata2_sd, GET_NAME(rdata2_sd));
    sc_trace(tf, r1_valid_sd, GET_NAME(r1_valid_sd));
    sc_trace(tf, r2_valid_sd, GET_NAME(r2_valid_sd));
    sc_trace(tf, stall_sd, GET_NAME(stall_sd));
    sc_trace(tf, block_in_dec, GET_NAME(block_in_dec));
    sc_trace(tf, dec2if_in_sd, GET_NAME(dec2if_in_sd));
    sc_trace(tf, dec2if_push_sd, GET_NAME(dec2if_push_sd));
    sc_trace(tf, dec2if_empty_sd, GET_NAME(dec2if_empty_sd));
    sc_trace(tf, dec2if_full_sd, GET_NAME(dec2if_full_sd));
    sc_trace(tf, dec2if_out_sd, GET_NAME(dec2if_out_sd));
    sc_trace(tf, dec2exe_in_sd, GET_NAME(dec2exe_in_sd));
    sc_trace(tf, dec2exe_push_sd, GET_NAME(dec2exe_push_sd));
    sc_trace(tf, dec2exe_full_sd, GET_NAME(dec2exe_full_sd));
    sc_trace(tf, r_type_inst_sd, GET_NAME(r_type_inst_sd));        // R type format
    sc_trace(tf, i_type_inst_sd, GET_NAME(i_type_inst_sd));        // I type format
    sc_trace(tf, s_type_inst_sd, GET_NAME(s_type_inst_sd));        // S type format
    sc_trace(tf, b_type_inst_sd, GET_NAME(b_type_inst_sd));        // B type format
    sc_trace(tf, u_type_inst_sd, GET_NAME(u_type_inst_sd));        // U type format
    sc_trace(tf, j_type_inst_sd, GET_NAME(j_type_inst_sd));        // J type format
    sc_trace(tf, m_type_inst_sd, GET_NAME(m_type_inst_sd));        // M type format
    sc_trace(tf, jalr_type_inst_sd, GET_NAME(jalr_type_inst_sd));  // JALR has a specific opcode
    sc_trace(tf, system_type_inst_sd, GET_NAME(system_type_inst_sd));
    sc_trace(tf, add_i_sd, GET_NAME(add_i_sd));
    sc_trace(tf, slt_i_sd, GET_NAME(slt_i_sd));
    sc_trace(tf, sltu_i_sd, GET_NAME(sltu_i_sd));
    sc_trace(tf, and_i_sd, GET_NAME(and_i_sd));
    sc_trace(tf, or_i_sd, GET_NAME(or_i_sd));
    sc_trace(tf, xor_i_sd, GET_NAME(xor_i_sd));
    sc_trace(tf, sll_i_sd, GET_NAME(sll_i_sd));
    sc_trace(tf, srl_i_sd, GET_NAME(srl_i_sd));
    sc_trace(tf, sub_i_sd, GET_NAME(sub_i_sd));
    sc_trace(tf, sra_i_sd, GET_NAME(sra_i_sd));
    sc_trace(tf, addi_i_sd, GET_NAME(addi_i_sd));
    sc_trace(tf, slti_i_sd, GET_NAME(slti_i_sd));
    sc_trace(tf, sltiu_i_sd, GET_NAME(sltiu_i_sd));
    sc_trace(tf, andi_i_sd, GET_NAME(andi_i_sd));
    sc_trace(tf, ori_i_sd, GET_NAME(ori_i_sd));
    sc_trace(tf, xori_i_sd, GET_NAME(xori_i_sd));
    sc_trace(tf, jalr_i_sd, GET_NAME(jalr_i_sd));
    sc_trace(tf, slli_i_sd, GET_NAME(slli_i_sd));
    sc_trace(tf, srli_i_sd, GET_NAME(srli_i_sd));
    sc_trace(tf, srai_i_sd, GET_NAME(srai_i_sd));
    sc_trace(tf, lw_i_sd, GET_NAME(lw_i_sd));
    sc_trace(tf, lh_i_sd, GET_NAME(lh_i_sd));
    sc_trace(tf, lhu_i_sd, GET_NAME(lhu_i_sd));
    sc_trace(tf, lb_i_sd, GET_NAME(lb_i_sd));
    sc_trace(tf, lbu_i_sd, GET_NAME(lbu_i_sd));
    sc_trace(tf, beq_i_sd, GET_NAME(beq_i_sd));
    sc_trace(tf, bne_i_sd, GET_NAME(bne_i_sd));
    sc_trace(tf, blt_i_sd, GET_NAME(blt_i_sd));
    sc_trace(tf, bge_i_sd, GET_NAME(bge_i_sd));
    sc_trace(tf, bltu_i_sd, GET_NAME(bltu_i_sd));
    sc_trace(tf, bgeu_i_sd, GET_NAME(bgeu_i_sd));
    sc_trace(tf, lui_i_sd, GET_NAME(lui_i_sd));
    sc_trace(tf, auipc_i_sd, GET_NAME(auipc_i_sd));
    sc_trace(tf, jal_i_sd, GET_NAME(jal_i_sd));
    sc_trace(tf, sw_i_sd, GET_NAME(sw_i_sd));
    sc_trace(tf, sh_i_sd, GET_NAME(sh_i_sd));
    sc_trace(tf, sb_i_sd, GET_NAME(sb_i_sd));
    sc_trace(tf, mul_i_sd, GET_NAME(mul_i_sd));
    sc_trace(tf, mulh_i_sd, GET_NAME(mulh_i_sd));
    sc_trace(tf, mulhsu_i_sd, GET_NAME(mulhsu_i_sd));
    sc_trace(tf, mulhu_i_sd, GET_NAME(mulhu_i_sd));
    sc_trace(tf, div_i_sd, GET_NAME(div_i_sd));
    sc_trace(tf, divu_i_sd, GET_NAME(divu_i_sd));
    sc_trace(tf, rem_i_sd, GET_NAME(rem_i_sd));
    sc_trace(tf, remu_i_sd, GET_NAME(remu_i_sd));
    sc_trace(tf, csrrw_i_sd, GET_NAME(csrrw_i_sd));
    sc_trace(tf, csrrs_i_sd, GET_NAME(csrrs_i_sd));
    sc_trace(tf, csrrc_i_sd, GET_NAME(csrrc_i_sd));
    sc_trace(tf, csrrwi_i_sd, GET_NAME(csrrwi_i_sd));
    sc_trace(tf, csrrsi_i_sd, GET_NAME(csrrsi_i_sd));
    sc_trace(tf, csrrci_i_sd, GET_NAME(csrrci_i_sd));
    sc_trace(tf, csr_wenable_sd, GET_NAME(csr_wenable_sd));
    sc_trace(tf, csr_radr_sd, GET_NAME(csr_radr_sd));
    sc_trace(tf, offset_branch_sd, GET_NAME(offset_branch_sd));
    sc_trace(tf, csr_in_progress, GET_NAME(csr_in_progress));
    sc_trace(tf, mret_i_sd, GET_NAME(mret_i_sd));
    sc_trace(tf, sret_i_sd, GET_NAME(sret_i_sd));

    // PC gestion :

    sc_trace(tf, inc_pc_sd, GET_NAME(inc_pc_sd));
    sc_trace(tf, jump_sd, GET_NAME(jump_sd));

    // Internal signals :

    sc_trace(tf, adr_dest_sd, GET_NAME(adr_dest_sd));
    sc_trace(tf, exe_op1_sd, GET_NAME(exe_op1_sd));
    sc_trace(tf, exe_op2_sd, GET_NAME(exe_op2_sd));
    sc_trace(tf, mem_data_sd, GET_NAME(mem_data_sd));

    sc_trace(tf, mem_size_sd, GET_NAME(mem_size_sd));
    sc_trace(tf, mem_load_sd, GET_NAME(mem_load_sd));
    sc_trace(tf, mem_store_sd, GET_NAME(mem_store_sd));

    sc_trace(tf, exe_cmd_sd, GET_NAME(exe_cmd_sd));
    sc_trace(tf, select_type_operations_sd, GET_NAME(select_type_operations_sd));
    sc_trace(tf, exe_neg_op2_sd, GET_NAME(exe_neg_op2_sd));
    sc_trace(tf, exe_wb_sd, GET_NAME(exe_wb_sd));
    sc_trace(tf, mem_sign_extend_sd, GET_NAME(mem_sign_extend_sd));

    // Exception :

    sc_trace(tf, ecall_i_sd, GET_NAME(ecall_i_sd));
    sc_trace(tf, ebreak_i_sd, GET_NAME(ebreak_i_sd));
    sc_trace(tf, illegal_instruction_sd, GET_NAME(illegal_instruction_sd));  // instruction doesnt exist
    sc_trace(tf,
             instruction_adress_missaligned_sd,
             GET_NAME(instruction_adress_missaligned_sd));  // branch offset is misaligned
    sc_trace(tf, env_call_m_mode_sd, GET_NAME(env_call_m_mode_sd));
    sc_trace(tf, env_call_s_mode_sd, GET_NAME(env_call_s_mode_sd));
    sc_trace(tf, CURRENT_MODE_SM, GET_NAME(CURRENT_MODE_SM));
    sc_trace(tf, MRET_RD, GET_NAME(MRET_RD));
    sc_trace(tf, MRET_SM, GET_NAME(MRET_SM));
    sc_trace(tf, RETURN_ADRESS_SM, GET_NAME(RETURN_ADRESS_SM));
    sc_trace(tf, env_call_u_mode_sd, GET_NAME(env_call_u_mode_sd));
    sc_trace(tf, instruction_access_fault_sd, GET_NAME(instruction_access_fault_sd));
    sc_trace(tf, INSTRUCTION_ACCESS_FAULT_RD, GET_NAME(INSTRUCTION_ACCESS_FAULT_RD));
    sc_trace(tf, MCAUSE_WDATA_SM, GET_NAME(MCAUSE_WDATA_SM));
    sc_trace(tf, env_call_wrong_mode, GET_NAME(env_call_wrong_mode));
    sc_trace(tf, PC_BRANCH_VALUE_RD, GET_NAME(PC_BRANCH_VALUE_RD));
    sc_trace(tf, pc_branch_value_sd, GET_NAME(pc_branch_value_sd));
}