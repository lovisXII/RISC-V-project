

// ######## OPCODE ########
// instruction type constants corresponding too instruction its [6, 0]

const int opcode_R_type       = 0b0110011;
const int opcode_I_type       = 0b0010011;
const int opcode_I_type_load  = 0b0000011;
const int opcode_I_type_jalr  = 0b1100111;
const int opcode_S_type       = 0b0100011;
const int opcode_B_type       = 0b1100011;
const int opcode_U_type       = 0b0110111;
const int opcode_U_type_auipc = 0b0010111;
const int opcode_J_type       = 0b1101111;

// ######## FUNC3 ########
// function3 constants coorresponding too instruction bits [14, 12]

// funct3 - R-type instruction

const int func3_add_i = 0b000;
const int func3_sub_i = 0b000;

const int func3_sll_i = 0b001;

const int func3_slt_i  = 0b010;
const int func3_sltu_i = 0b011;

const int func3_xor_i = 0b100;

const int func3_srl_i = 0b101;
const int func3_sra_i = 0b101;

const int func3_or_i = 0b110;

const int func3_and_i = 0b111;

const int func3_lrd_i = 0b011;
const int func3_scd_i = 0b011;

// funct3 - I-type instruction

const int funct3_add_i   = 0b000;
const int funct3_slti_i  = 0b010;
const int funct3_sltiu_i = 0b011;
const int funct3_addi_i  = 0b111;
const int funct3_ori_i   = 0b110;
const int funct3_xori_i  = 0b100;

// funct3 - I-type shift instructions

const int funct3_slli_i = 0b001;
const int funct3_srli_i = 0b101;
const int funct3_srai_i = 0b101;

// funct3 - I-type load instructions

const int funct3_lw_i  = 0b010;
const int funct3_lh_i  = 0b001;
const int funct3_lhu_i = 0b101;
const int funct3_lb_i  = 0b000;
const int funct3_lbu_i = 0b100;

// funct3 - S_type

const int funct3_sw_i = 0b010;
const int funct3_sh_i = 0b001;
const int funct3_sb_i = 0b000;

// funct3 - B-type

const int funct3_beq_i  = 0b000;
const int funct3_bne_i  = 0b001;
const int funct3_blt_i  = 0b100;
const int funct3_bge_i  = 0b101;
const int funct3_bltu_i = 0b110;
const int funct3_bgeu_i = 0b111;

//  ######## FUNCT7 ########
// function7 constants corresponding too instruction bits [31, 25]

// funct7 - R-type

const int funct7_add_i  = 0b0000000;
const int funct7_slt_i  = 0b0000000;
const int funct7_sltu_i = 0b0000000;
const int funct7_and_i  = 0b0000000;
const int funct7_xor_i  = 0b0000000;
const int funct7_srl_i  = 0b0000000;
const int funct7_sub_i  = 0b0100000;
const int funct7_sra_i  = 0b0100000;
const int funct7_sll_i  = 0b0000000;
const int funct7_or_i   = 0b0000000;

// funct7 - I-type shift instructions

const int funct7_slli_i = 0b0000000;
const int funct7_srli_i = 0b0000000;
const int funct7_srai_i = 0b0100000;
