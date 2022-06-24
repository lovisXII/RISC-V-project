.section .text
.global _start

_start:
    li x2, 0xE              # x2 = 0xE
    csrrwi x0,mepc,0x0      # reseting value inside mepc
    csrrwi x1, mepc, 0xE    # x1 = 0, mepc = 0xE
    nop
    nop
    nop
    bne x1, x0, _bad        # doesnt branch
    csrrw x1, mepc, x0      # x1 = 0xE, mepc = 0
    nop
    nop
    nop
    beq x1, x2, _good       # branch
    nop
