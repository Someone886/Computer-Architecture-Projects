# numc

##### Matrix.c

###### allocate_matrix

1. If rows < 1 or cols < 1, throw a TypeError and return -1 immediately.

2. Try to allocate the space for this matrix structure;

   - If malloc fails, throw a RunTimeError and return -1 immediately.

3. Try to calloc a space of rows*cols as this new matrix's data mat;

   - If calloc fails, throw a RunTimeError;
   - free the new matrix structure, then exit with -1;

4. Assign values to the variables based on the input parameters.

5. ref_cnt would be 1 as this new matrix only has itself as the reference.

   

###### allocate_matrix_ref:

1. If rows < 1 or cols < 1, throw a TypeError and return -1 immediately.
2. Try to allocate the space for the matrix structure;
   - If malloc fails, throw a RunTimeError and return -1 immediately.
3. Assign values to the variables based on the input parameters.
4. Assign from as the parent of this newly allocated matrix.
5. This new matrix's parent's ref_cnt += 1 as the parent now has a new reference.



###### deallocate_matrix:

1. If mat is Null, return immediatelyl
   - However, do not throw any error as it may cover the TypeError throw by the pre-defined init function in numc.c.
2. If the mat has a parent:
   - If the parent has other references besides mat, only free mat and make its parent's ref_cnt -=1;
   - If mat is the only reference of its parent, free mat's parent's data, then free its parent, then free mat.
3. If the mat has no parent:
   - If mat has other references, ref_cnt -= 1 but do not free mat;
   - Otherwise, free mat's data, and then free mat.



###### get / set: 

index = row*(mat->cols) + col



###### fill_matrix:

1. If mat is Null, return a TypeError and exit with -1.
2. For each iteration, using 4 SIMD instructions to store val to16 elements; no possible data race.
3. Use omp parallel for to speed it up.
4. Use two tail iterations to fill the rest of the matrix with val.



###### add_matrix:

1. If result / mat1/ mat2 is Null, return a TypeError and exit with -1.
2. Check for dimensions and return a ValueError if necessary.
3. For each iteration, use omp parallel for and SIMD load 16 elements from mat1, load 16 elements from mat2, add them together and then store the results to result. Repeat this process for another 16 elements.
   - Use private argument to make some variables private and avoid data race.
4. Use two tail iterations to do the addition for the rest of mat1 and mat2.



###### sub_matrix:

Same logic as above, but just doing subtraction instead of addition.



mul_matrix (since I made the post about my approach on Piazza, I would just copy and paste the post and make some minor changes to my words):

1. Throw TypeError/ ValueError/ neither based on the input values.

2. Do the transpose for mat 2 to better exploit cache property;

   - Reusing the same code from lab 7, but to avoid accessing uninitialized space, see if dst_location or src_location is out of the index before accessing them;

   - Blocking = 4*4.

3. For every i loop iteration, find start positions for the proper rows in matrix 1 and transposed matrix 2;

   - Each ith iteration would calculate the result for the ith element in the result matrix;

   - For mat 1, its column # = i / col_num3 -> start index = column # * col_num1;
   - For transposed mat 2, its column # = row # of the original mat 2 = i % col_num2 -> start index = column # * row_num2;

4. Then do element-wise multiplication for this ith element in the result matrix.

5.  When it does element-wise multiplication, it basically just moves from left to right 1 by 1 on those two rows and never go back until the program reach index i+1. By doing this, it is doing implicit blocking as the program would only move to the next "block" after it finishes the calculation in this "block".

6. Another advantage of doing this is that the program can save the element-wise multiplication result into a __mm256d "array", and after it finishes with index i, it just saves the sum of this "array" directly back to the result matrix. That means the program doesn't have to fill the result matrix with 0 at the beginning of the mul function as the number is directly stored instead of being added on.

7. Minor details: pragma on the i loop, two tail iterations to handle the rest element-wise multiplication for each element in the result mat, free transposed_mat2 at the end.

   

###### pow_matrix

1. Throw TypeError or ValueError if necessary.
2. Using square power to speed up the program.
3. Make the result mat as an identity matrix first.
4. Using a spare matrix to swap the original result mat and the new result mat during the process of square power.
5. Using two spare matrix to swap and store the result mat and the operant mat;
   - tmp_operand would point to mat at first, but then it would just become an empty space to shift around the operand and the result;
   - This could avoid changing the original mat.
6. Save the ptrs to temp_store and spare2 for deallocating them at the end;
   - No need to save the ptr to spare as the ptr always points to spare.
7. If calls to mat_mul fails, deallocate spare, temp_store, and spare2 before exiting with -1.



###### neg_matrix

Basically just time -1 for every element in the original mat and store the result into the result Matrix. Using pragma and SIMD to speed up this process.



###### abs_matrix

1. For each element, negate it first, and then compare it to the original value.
2. Store the max of two values into the result matrix.
3. Using pragma and SIMD to speed up this process. 
4. Using private arguments to avoid data races.



##### numc.c

1. Basically for each method, check for possible errors first and throw any necessary error before exiting with -1.
2. If calls to any matrix.c function fails, deallocate the return_matrix (which is the result matrix).
3. If everything went successfully, assign the return_matrix to the Matirx61c ptr that would be returned; change the Matrix61c ptr shape using PyTuple_pack based on the requirements.
4. The check for if the power is a int is done here before calling the pow function in matrix.c.
5. Create a PyNumberMethods struct for overloading operators with all the number methods defined in numc.c;
   - Cast each function to binaryfunc / unaryfunc / ternaryfunc based on the number of arguments.
6. Check for TypeError and IndexError for set and get is also done here.
7. Add set and get to the instance methods;
   - I also added add, sub, ..., and all the functions as instance methods here just in case I may need them in instance methods form (though that never happened).



##### Usage and limitation of my numc package

1. This numc package could be a replacement of the original dumpy matrix operation.
2. The calculation done by my numc package is way faster than the dumpy matrix package;
   - Mul: about 90x speedup;
   - Simple: about 4.5x speedup;
   - Pow: about 1000x speedup;
   - Comprehensive: about 93x speedup.
3. However, partial slices are not supported by my numc package.

