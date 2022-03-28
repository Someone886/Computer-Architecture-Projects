.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 123.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 124.
# =======================================================
dot:
    addi t0, x0, 1		# t0 = 1
    blt a2, t0, exit_error1
    blt a3, t0, exit_error2
    blt a4, t0, exit_error2
    # Prologue
    addi sp, sp, -16
    sw s1, 0(sp)
    sw s2, 4(sp)
    sw s3, 8(sp)
    sw s4, 12(sp)
    
	addi t0, x0, 0		# t0 = entire product = 0
    addi t1, x0, 0		# t1 = length counter = 0
    addi t4, x0, 0		# t4 = index of v0 = 0
    addi t5, x0, 0		# t5 = index of v1 = 0

loop_start:
    beq t1, a2, loop_end
    addi t3, x0, 0		# t3 = this product = 0
    slli s1, t4, 2		# s1 = t4 * 4
    slli s2, t5, 2		# s2 = t5 * 4
    add s1, s1, a0		# s1 = offset + a0
    add s2, s2, a1		# s2 = offset + a1
    lw s3, 0(s1)		# s3 = v0[t4]
    lw s4, 0(s2)		# s4 = v1[t5]
    mul t3, s3, s4		# t3 = s3 * s4
    add t4, t4, a3		# t4 = t4 + stride of v0 (a3)
    add t5, t5, a4		# t5 = t5 + stride of v1 (a4)
    add t0, t3, t0
    addi t1, t1, 1
    j loop_start

loop_end:
    add a0, t0, x0		# a0 = sum = t0
    # Epilogue
    lw s1, 0(sp)
    lw s2, 4(sp)
    lw s3, 8(sp)
    lw s4, 12(sp)
    addi sp, sp, 16
    ret

exit_error1:
    addi a1, x0, 123		# exit with code 123
    j exit2
    
exit_error2:
    addi a1, x0, 124		# exit with code 124
    j exit2
