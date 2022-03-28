.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 121.
    # - If malloc fails, this function terminats the program with exit code 122.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
	
    addi t0, x0, 5			# 5 usage
    bne t0, a0, command_error
    
    # Pre
    addi sp, sp, -52
	sw ra, 0(sp)
    sw s0, 4(sp)			# argc
    sw s1, 8(sp) 			# argv - char**
    sw s2, 12(sp)			# print_classification
    sw s3, 16(sp)			# temporarily saving the ptr to row dimension -> saving ptr to m0
    sw s4, 20(sp)			# temporarily saving the ptr to col dimension -> saving ptr to m1
    sw s5, 24(sp)			# temporarily saving ptr to m0 -> ptr to the row of the input matrix
    sw s6, 28(sp)			# temporarily saving ptr to m1 -> ptr to the col of the input matrix
    sw s7, 32(sp)			# ptr to the input matrix
    sw s8, 36(sp)			# number of elements in m0 * input
    sw s9, 40(sp)			# ptr to the output matrix of m0 * input
    sw s10, 44(sp)			# number of elements in the final output matrix -> matched int
    sw s11, 48(sp)			# ptr to the final output matrix
	
    addi s0, a0, 0
    addi s1, a1, 0
    addi s2, a2, 0


	# =====================================
    # LOAD MATRICES
    # =====================================
    
    # Load pretrained m0
    
    addi a0, x0, 4
    jal malloc
    beq a0, x0, malloc_error
    addi s3, a0, 0			# saving the ptr to row dimension for now
    
    addi a0, x0, 4
    jal malloc
    beq a0, x0, malloc_error
    addi s4, a0, 0			# saving the ptr to col dimension for now
    
    lw a0, 4(s1)
	addi a1, s3, 0
    addi a2, s4, 0
    jal read_matrix
    addi s5, a0, 0
    
	addi sp, sp, -16
    sw s3, 0(sp)			# 0(sp) = ptr to m0 row
    sw s4, 4(sp)			# 4(sp) = ptr to m0 col
	
    # Load pretrained m1
	
    addi a0, x0, 4
    jal malloc
    beq a0, x0, malloc_error
    addi s3, a0, 0			# saving the ptr to row dimension for now
    
    addi a0, x0, 4
    jal malloc
    beq a0, x0, malloc_error
    addi s4, a0, 0			# saving the ptr to col dimension for now
    
	lw a0, 8(s1)
	addi a1, s3, 0
    addi a2, s4, 0
    jal read_matrix
    addi s6, a0, 0
	
	sw s3, 8(sp)			# 8(sp) = ptr to m1 row
	sw s4, 12(sp)			# 12(sp) = ptr to m1 col
    
    mv s3, s5				# s3 now pting to m0
    mv s4, s6				# s4 now pting to m1

    # Load input matrix

	addi a0, x0, 4
    jal malloc
    beq a0, x0, malloc_error
    addi s5, a0, 0			
    
    addi a0, x0, 4
    jal malloc
    beq a0, x0, malloc_error
    addi s6, a0, 0			

	lw a0, 12(s1)
    addi a1, s5, 0
    addi a2, s6, 0
    jal read_matrix
    addi s7, a0, 0
	
    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)

	# 1. linear layer
    lw t0, 0(sp)
	lw t0, 0(t0)
    lw t1, 0(s6)
    mul s8, t0, t1			# s8 = number of elements in m0 * input
	slli t3, s8, 2			# t3 = number of bytes needed for m0 * input
	
    addi a0, t3, 0
	jal malloc
	beq a0, x0, malloc_error
    addi s9, a0, 0			# s9 = ptr to m0 * input
	
	addi a0, s3, 0
    lw a1, 0(sp)
    lw a1, 0(a1)
	lw a2, 4(sp)
    lw a2, 0(a2)
	addi a3, s7, 0
	lw a4, 0(s5)
    lw a5, 0(s6)
	addi a6, s9, 0			
	jal matmul				# s9 = ptr to m0 * input
    
    # 2
    addi a0, s9, 0
    addi a1, s8, 0
    jal relu				# s9 = ptr to relued m0 * input
    
    # 3
    lw t0, 8(sp)
    lw t0, 0(t0)
    lw t1, 0(s6)
    mul s10, t0, t1			# s10 = number of elements in the final matrix
    slli t3, s10, 2			# t3 = number of bytes needed for the final matrix
    
    addi a0, t3, 0
    jal malloc
    beq a0, x0, malloc_error
    addi s11, a0, 0			# s11 = ptr to the final matrix
    
    addi a0, s4, 0
    lw a1, 8(sp)
    lw a1, 0(a1)
    lw a2, 12(sp)
    lw a2, 0(a2)
    addi a3, s9, 0
    lw a4, 0(sp)
    lw a4, 0(a4)
    lw a5, 0(s6)
    addi a6, s11, 0
    jal matmul				# s11 = ptr to the final matrix
    
    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix

	lw a0, 16(s1)
	addi a1, s11, 0
    lw a2, 8(sp)
    lw a2, 0(a2)
    lw a3, 0(s6)
	jal write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax
	
    addi a0, s11, 0
	addi a1, s10, 0
	jal argmax
    addi s10, a0, 0			# s10 = max index

    # Print classification
    
    bne s2, x0, no_print
	addi a1, s10, 0
    jal print_int

    # Print newline afterwards for clarity
    addi a1, x0, 10
    jal print_char

no_print:
	lw a0, 0(sp)
    jal free
    lw a0, 4(sp)
    jal free
    lw a0, 8(sp)
    jal free
    lw a0, 12(sp)
    jal free
    addi sp, sp, 16
    
    addi a0, s3, 0
    jal free
    addi a0, s4, 0
    jal free
	addi a0, s5, 0
    jal free
    addi a0, s6, 0
    jal free
	addi a0, s7, 0
    jal free
    addi a0, s9, 0
    jal free
    addi a0, s11, 0
    jal free
    
    addi a0, s10, 0
    
    lw ra, 0(sp)
    lw s0, 4(sp)			# argc
    lw s1, 8(sp) 			# argv - char**
    lw s2, 12(sp)			# print_classification
    lw s3, 16(sp)			# temporarily saving the ptr to row dimension -> saving ptr to m0
    lw s4, 20(sp)			# temporarily saving the ptr to col dimension -> saving ptr to m1
    lw s5, 24(sp)			# temporarily saving ptr to m0 -> ptr to the row of the input matrix
    lw s6, 28(sp)			# temporarily saving ptr to m1 -> ptr to the col of the input matrix
    lw s7, 32(sp)			# ptr to the input matrix
    lw s8, 36(sp)			# number of elements in m0 * input
    lw s9, 40(sp)			# ptr to the output matrix of m0 * input
    lw s10, 44(sp)			# number of elements in the final output matrix -> matched int
    lw s11, 48(sp)			# ptr to the final output matrix
    addi sp, sp, 52
    
    ret

command_error:
	addi a1, x0, 121
    j exit2

malloc_error:
    jal free
    addi a1, x0, 122
    j exit2
