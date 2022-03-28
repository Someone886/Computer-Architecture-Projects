.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 125.
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 126.
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 127.
# =======================================================
matmul:
	# Error checks
	addi t0, x0, 1				# t0 = 1
    blt a1, t0, exit_error1
    blt a2, t0, exit_error1
    blt a4, t0, exit_error2
    blt a5, t0, exit_error2
    bne a2, a4, exit_error3
    
    # Prologue
	addi sp, sp, -32
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw ra, 28(sp)
    
    mv s0, a0
    mv s1, a1
    mv s2, a2
    mv s3, a3
    mv s4, a4
    mv s5, a5
    mv s6, a6
	
    addi t0, x0, 0				# t0 = number of m0's rows processed = 0
    j outer_loop_start
    
outer_loop_start:
	beq t0, s1, outer_loop_end
	addi t1, x0, 0				# t1 = number of m1's columns processed = 0

inner_loop_start:
	beq t1, s5, inner_loop_end
    addi a0, s0, 0				# a0 = s0 = pointer to the right location in m0
	addi a1, s3, 0				# a1 = s3 = pointer to the right location in m1
	addi a2, s2, 0				# a2 = length of the two vectos = s2 = column number of m0
	addi a3, x0, 1				# a3 = stride for m0 = 1
	addi a4, s5, 0				# a4 = stride for m1 = s5 = column number of m1
	
	addi sp, sp, -20
    sw t0, 0(sp)
    sw t1, 4(sp)
    sw t2, 8(sp)
    sw t3, 12(sp)
    sw a6, 16(sp)
	
	jal ra dot
    
    lw t0, 0(sp)
    lw t1, 4(sp)
    lw t2, 8(sp)
    lw t3, 12(sp)
    lw a6, 16(sp)
	addi sp, sp, 20
    
    sw a0, 0(s6)				# s6 points to the next available location in a6; store the result to s6
    
	addi t1, t1, 1				# number of processed m1's column ++
    addi s3, s3, 4				# s3 points to the start of m1's next column
    addi s6, s6, 4				# next available location is 4 bytes apart
    j inner_loop_start

inner_loop_end:
	addi t0, t0, 1				# number of processed m0's row ++
    
    addi t3, x0, -4
    mul t3, t3, s5				# t3 = -(4 * column number of m1)
    add s3, s3, t3				# restore s3 to pointing to the start of m1's first column
    
    addi t3, x0, 4
    mul t3, t3, s2				# t3 = 4 * column number of m0
    add s0, s0, t3				# s0 points to the start of m0's next row
	j outer_loop_start

outer_loop_end:
    # Epilogue
	lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)    
    lw ra, 28(sp)
    addi sp, sp, 32
    ret

exit_error1:
	addi a1, x0, 125
    j exit2
    
exit_error2:
	addi a1, x0, 126
    j exit2

exit_error3:
	addi a1, x0, 127
    j exit2