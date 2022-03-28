.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 116.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 117.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 118.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 119.
# ==============================================================================
read_matrix:

    # Prologue
	addi sp, sp, -44
    sw ra, 0(sp)
    sw a0, 4(sp)			# filename
    sw a1, 8(sp)			# ptr to the number of rows
    sw a2, 12(sp) 			# ptr to the number of columns
    sw s0, 16(sp) 			# ptr to matrix dimension buffer
    sw s1, 20(sp) 			# file discriptor
    sw s2, 24(sp) 			# number of rows
    sw s3, 28(sp) 			# number of columns
    sw s4, 32(sp) 			# number of bytes needed
    sw s5, 36(sp) 			# ptr to matrix buffer
    sw s6, 40(sp) 			# 

	addi a1, a0, 0
	addi a2, x0, 0
    jal fopen
    addi t0, x0, -1
    beq a0, t0, fopen_error
    add s1, x0, a0
	
    addi a0, x0, 4
	jal malloc
	beq a0, x0, malloc_error
    add s0, x0, a0
    
    addi a1, s1, 0
    addi a2, s0, 0
    addi a3, x0, 4
    jal fread 				# read first 4 bytes
    addi t0, x0, 4
    bne a0, t0, fread_error
    lw s2, 0(s0) 			# s2 = number of rows
    
    addi a1, s1, 0
    addi a2, s0, 0
    addi a3, x0, 4
    jal fread 				# read second 4 bytes
    addi t0, x0, 4
    bne a0, t0, fread_error
    lw s3, 0(s0) 			# s3 = number of columns
    
    mul t0, s2, s3			# t0 = number of elements
    addi t1, x0, 4			# t1 = 4
    mul s4, t1, t0			# s4 = number of bytes
    
    addi a0, s4, 0
    jal malloc
    beq a0, x0, malloc_error
    addi s5, a0, 0			# s5 = ptr to matrix buffer
    
    addi a1, s1, 0
    addi a2, s5, 0
    addi a3, s4, 0
    jal fread
    bne a0, s4, fread_error
    
    addi a1, s1, 0
    jal fclose
    addi t0, x0, -1
    beq t0, a0, fclose_error
    
    # Epilogue
    lw a1, 8(sp)
    lw a2, 12(sp)
    
    addi a0, s5, 0
    sw s2, 0(a1)
    sw s3, 0(a2)
    
    lw ra, 0(sp)
	lw s0, 16(sp)
    lw s1, 20(sp)
    lw s2, 24(sp)
    lw s3, 28(sp)
    lw s4, 32(sp)
    lw s5, 36(sp)
    lw s6, 40(sp)
    addi sp, sp, 44

    ret

malloc_error:
	jal free
    addi a1, x0, 116
    j exit2

fopen_error:
    addi a1, x0, 117
    j exit2

fread_error:
    addi a1, x0, 118
    j exit2

fclose_error:
    addi a1, x0, 119
    j exit2