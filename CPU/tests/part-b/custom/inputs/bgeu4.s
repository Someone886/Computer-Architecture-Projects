addi t0, x0, 1
addi t1, x0, 4

loop:
addi t0, t0, 1
bgeu t1, t0, loop
blt t1, t0, end
addi s0, x0, 10

end:
addi s1, x0, 10


