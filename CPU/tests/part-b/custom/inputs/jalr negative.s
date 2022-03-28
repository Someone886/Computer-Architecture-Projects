addi t0, t0, 1
addi a0, x0, 2
addi sp, sp, 2047
addi sp, sp, 1
beq t0, a0, skip
jalr x0, sp, -2048
skip:
addi s0, x0, 10

