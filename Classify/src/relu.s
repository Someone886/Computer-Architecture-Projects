.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 115.
# ==============================================================================
relu:
    addi t0, x0, 1		# t0 = 1
    blt a1, t0, exit_error
    # Prologue
    add t0, x0, x0		# t0 = current index = 0
    j loop_start

loop_start:
    beq t0, a1, loop_end
    slli t1, t0, 2		# t1 = t0 * 4
    add t2, t1, a0		# t2 = a0 + offset
    lw t3, 0(t2)
    bgt t3, x0, loop_continue
    sw x0, 0(t2)

loop_continue:
    addi t0, t0, 1
    j loop_start

loop_end:
    # Epilogue
    ret

exit_error:
    addi a1, x0, 115		# exit with code 115
    j exit2
