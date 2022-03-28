addi t0, x0, 1
addi t1, x0, 4

here:
addi t0, t0, 1
beq t0, t1, there
jalr x0, x0, 8
addi s0, x0, 5

there:
jalr, ra, x0, 32
addi s1, x0, 5

end:
addi s0, x0, 5