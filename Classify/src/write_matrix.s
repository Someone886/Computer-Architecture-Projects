.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 112.
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 113.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 114.
# ==============================================================================
write_matrix:

    # Prologue
	addi sp, sp, -48
    sw ra, 0(sp)
    sw a0, 4(sp)			# filename
    sw a1, 8(sp)			# ptr to the start of the matrix in memory
    sw a2, 12(sp) 			# number of rows
    sw a3, 16(sp)			# number of columns
    sw s0, 20(sp) 			# file descriptor
    sw s1, 24(sp) 			# 
    sw s2, 28(sp) 			# s2 = address of the number of rows
    sw s3, 32(sp) 			# s3 = address of the number of columns
    sw s4, 36(sp) 			# s4 = number of numbers in the matrix
    sw s5, 40(sp) 			# 
    sw s6, 44(sp) 			# 

	addi a1, a0, 0
	addi a2, x0, 1
	jal fopen
	addi t0, x0, -1
	beq a0, t0, fopen_error
	addi s0, a0, 0
    
    addi s2, sp, 12			# s2 = address of the number of rows
    addi a1, s0, 0
    addi a2, s2, 0
    addi a3, x0, 1
    addi a4, x0, 4
    jal fwrite
    addi t0, x0, 1
    bne a0, t0, fwrite_error
	
    addi s3, sp, 16			# s3 = address of the number of columns
    addi a1, s0, 0
    addi a2, s3, 0
    addi a3, x0, 1
    addi a4, x0, 4
    jal fwrite
    addi t0, x0, 1
    bne a0, t0, fwrite_error
    
    lw t2, 0(s2)			# t2 = number of rows
    lw t3, 0(s3)			# t3 = number of rows
    mul s4, t2, t3
    
    addi a1, s0, 0			
    lw a2, 8(sp)			# a2 = *(sp + 8) = ptr to the start of the matrix
    addi a3, s4, 0
    addi a4, x0, 4
    jal fwrite
    bne a0, s4, fwrite_error
    
    addi a1, s0, 0
    jal fclose
    addi t1, x0, -1
    beq a0, t1, fclose_error
    
    # Epilogue
	lw ra, 0(sp)
    lw a0, 4(sp)			# filename
    lw a1, 8(sp)			# ptr to the start of the matrix in memory
    lw a2, 12(sp) 			# number of rows
    lw a3, 16(sp)			# number of columns
    lw s0, 20(sp) 			# file descriptor
    lw s1, 24(sp) 			# 
    lw s2, 28(sp) 			# s2 = address of the number of rows
    lw s3, 32(sp) 			# s3 = address of the number of columns
    lw s4, 36(sp) 			# s4 = number of numbers in the matrix
    lw s5, 40(sp) 			# 
    lw s6, 44(sp) 			# 
	addi sp, sp, 48
    
    ret

fopen_error:
    addi a1, x0, 112
    j exit2

fwrite_error:
    addi a1, x0, 113
    j exit2

fclose_error:
    addi a1, x0, 114
    j exit2