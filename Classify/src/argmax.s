.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 120.
# =================================================================
argmax:
    addi t0, x0, 1		# t0 = 1
    blt a1, t0, exit_error
    # Prologue
    addi t0, x0, 0		# t0 = current index = 0
    lw t1, 0(a0)		# t1 = max element = first element
    
    addi t2, x0, 0		# t2 = max index = 0

loop_start:
    beq t0, a1, loop_end
    slli t3, t0, 2		# t3 = t0 * 4
    add t4, a0, t3		# t4 = a0 + offset
    lw t5, 0(t4)		# t5 = a0[t0]
    ble t5, t1, loop_continue
    addi t2, t0, 0		# t2 = max index = t0
    addi t1, t5, 0		# t1 = max element = t5

loop_continue:
	addi t0, t0, 1
    j loop_start

loop_end:
    addi a0, t2, 0		# a0 = max index = t2
    # Epilogue
    ret

exit_error:
    addi a1, x0, 120		# exit with code 120
    j exit2