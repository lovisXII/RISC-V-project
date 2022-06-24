.section text
.global _good
.global _bad
.global _exception_occur
_bad :
    nop
_good :
    nop
_exception_occur :
    nop
.section .kernel
.global _exception
.global _instruction_address_misagligned 
.global _instruction_address_fault 
.global _illegal_instruction 
.global _load_adress_missaligned 
.global _load_access_fault 
.global _store_adress_missaligned 
.global _store_access_fault 
.global _env_call_u_mode 
.global _env_call_s_mode 
.global _env_call_m_mode 
.global _env_call_wrong_mode 
.global _exception 
.global _isr_vector
_exception :
    csrrwi  x10, 0x342, 0x0    # reading mcause value
    csrrci  x11,mscratch,0x01  # reading mscratch and clearing bit 0
    # which will do nothing cause mscratch is an adress 
    # so least to significant bits are 0
    sw      x10,0(x11)         # storing mcause value on the stack at @mscratch
    slli    x10,x10,2          # mcause * 4
    la      x11, _isr_vector   # loading starts of _isr_vector 
    add     x11,x11,x10        # jumping at the right isr function
    lw      x12, 0(x11)        # loading the isr function adress
    jalr    x0,0(x12)          # jumping to the right status


_instruction_address_misagligned :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 
_instruction_address_fault :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 
_illegal_instruction :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 
_load_adress_missaligned :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 
_load_access_fault :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 
_store_adress_missaligned :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 
_store_access_fault :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 
_env_call_u_mode :
    nop
    mret 
_env_call_s_mode :
    nop
    mret
_env_call_m_mode :
    nop
    mret
_env_call_wrong_mode :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1)

_isr_vector :
# just a label to load the adresses of the isr function
# @_isr_vector + 0 :_instruction_adress_missaligned