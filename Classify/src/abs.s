.globl abs

.text
# =================================================================
# FUNCTION: Given an int return its absolute value.
# Arguments:
# 	a0 (int) is input integer
# Returns:
#	a0 (int) the absolute value of the input
# =================================================================
abs:
    blt a0, x0, neg
    ret

neg:
    addi t0, x0, -1
    mul a0, a0, t0
    ret