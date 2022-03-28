addi t0, x0, 1
addi t1, x0, 1
beq t0, t1, here
addi t2, x0, 10

here:
addi t0, x0, 0
addi t1, x0, 1
beq t0, t1, there
addi t2, x0, 10

there:
