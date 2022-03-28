addi t0, x0, 1
addi t1, x0, 4

here:
addi t0, t0, 1
bltu t0, t1, here
addi s0, x0, 10

there:
bne t0, t1, end
addi t1, t0, 1
jal ra, there
addi s1, x0, 5

end:
	

