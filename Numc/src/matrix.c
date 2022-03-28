#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values, or if any
 * call to allocate memory in this function fails. If you don't set python error messages here upon
 * failure, then remember to set it in numc.c.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    if (rows < 1 || cols < 1) {
        PyErr_SetString(PyExc_TypeError, "Non-positive dimension(s).");
        return -1;
    }
    matrix* ptr = (matrix *) malloc(sizeof(matrix));
    if (!ptr) {
        PyErr_SetString(PyExc_RuntimeError, "ptr to struct allocation fails.");
        return -1;
    }
    *mat = ptr;
    int size = rows * cols;
    double* data = (double *) calloc(size, sizeof(double));
    if (!data) {
        PyErr_SetString(PyExc_RuntimeError, "ptr to data allocation fails.");
        free(ptr);
        return -1;
    }
    ptr->rows = rows;
    ptr->cols = cols;
    ptr->data = data;
    ptr->ref_cnt = 1;
    ptr->parent = NULL;
    return 0;
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`.
 * You should return -1 if either `rows` or `cols` or both are non-positive or if any
 * call to allocate memory in this function fails.
 * If you don't set python error messages here upon failure, then remember to set it in numc.c.
 * Return 0 upon success.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    if (rows < 1 || cols < 1) {
        PyErr_SetString(PyExc_TypeError, "Non-positive dimension(s).");
        return -1;
    }
    matrix* ptr = malloc(sizeof(matrix));
    if (!ptr) {
        PyErr_SetString(PyExc_RuntimeError, "ptr to struct allocation fails.");
        return -1;
    }
    *mat = ptr;
    ptr->rows = rows;
    ptr->cols = cols;
    ptr->data = from->data + offset;
    from->ref_cnt += 1;
    ptr->parent = from;
    return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or if `mat` is the last existing slice of its parent matrix and its parent matrix has no other references
 * (including itself). You cannot assume that mat is not NULL.
 */
void deallocate_matrix(matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL) {
//        PyErr_SetString(PyExc_TypeError, "Null ptr to the matrix.");
        return;
    }

    if (mat->parent != NULL){
        if ((mat->parent)->ref_cnt > 1) {
            (mat->parent)->ref_cnt -= 1;
            free(mat);
        } else {
            (mat->parent)->ref_cnt -= 1;
            free((mat->parent)->data);
            free(mat->parent);
            free(mat);
        }
    } else {
        if (mat->ref_cnt > 1) {
            mat->ref_cnt -= 1;
        } else {
            mat->ref_cnt -= 1;
            free(mat->data);
            free(mat);
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid.
 */
double get(matrix *mat, int row, int col) {
    /* TODO: YOUR CODE HERE */
    return mat->data[row*(mat->cols) + col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid
 */
void set(matrix *mat, int row, int col, double val) {
    /* TODO: YOUR CODE HERE */
    mat->data[row * (mat->cols) + col] = val;
}

/*
 * Sets all entries in mat to val
 */
void fill_matrix(matrix *mat, double val) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL) {
        PyErr_SetString(PyExc_TypeError, "Null ptr to the matrix.");
        return;
    }
    int length = mat->rows * mat->cols;
//    for(int i = 0; i < length; i++) {
//        mat->data[i] = val;
//    }
    __m256d const_vector = _mm256_set1_pd(val);
    #pragma omp parallel for
    for (int i = 0; i < length / 16 * 16; i += 16){
        _mm256_storeu_pd(mat->data + i + 0, const_vector);
        _mm256_storeu_pd(mat->data + i + 4, const_vector);
        _mm256_storeu_pd(mat->data + i + 8, const_vector);
        _mm256_storeu_pd(mat->data + i + 12, const_vector);
    }
//    #pragma omp parallel for
    for (int i = length / 16 * 16; i < length / 4 * 4; i += 4) {
        _mm256_storeu_pd(mat->data + i, const_vector);
    }
    for (int i = length / 4 * 4; i < length; i++){
        mat->data[i] = val;
    }
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    if (result == NULL || mat1 == NULL || mat2 == NULL) {
        PyErr_SetString(PyExc_TypeError, "Null ptr to the matrix(es).");
        return -1;
    }

    int row_num1 = mat1->rows;
    int row_num2 = mat2->rows;
    int row_num3 = result->rows;
    int col_num1 = mat1->cols;
    int col_num2 = mat2->cols;
    int col_num3 = result->cols;

    if (row_num1 != row_num2 || row_num1 != row_num3 || row_num2 != row_num3 ||
        col_num1 != col_num2 || col_num1 != col_num3 || col_num2 != col_num3) {
        PyErr_SetString(PyExc_ValueError, "Incompatible dimension(s).");
        return -1;
    }

    int length = row_num1 * col_num1;
    double* result_data = result->data;
    double* mat1_data = mat1->data;
    double* mat2_data = mat2->data;

//    for (int i = 0; i < length; i++) {
//        result_data[i] = mat1_data[i] + mat2_data[i];
//    }
    __m256d vector1, vector2, vector3, vector4,
            vector1_2, vector2_2, vector3_2, vector4_2,
            result1, result2, result3, result4,
            vector_tail;
    #pragma omp parallel for private(vector1, vector2, vector3, vector4, vector1_2, vector2_2, vector3_2, vector4_2)
    for (int i = 0; i < length / 32 * 32; i += 32){
        vector1 = _mm256_loadu_pd(mat1_data + i + 0);
        vector2 = _mm256_loadu_pd(mat1_data + i + 4);
        vector3 = _mm256_loadu_pd(mat1_data + i + 8);
        vector4 = _mm256_loadu_pd(mat1_data + i + 12);
        vector1_2 = _mm256_loadu_pd(mat2_data + i + 0);
        vector2_2 = _mm256_loadu_pd(mat2_data + i + 4);
        vector3_2 = _mm256_loadu_pd(mat2_data + i + 8);
        vector4_2 = _mm256_loadu_pd(mat2_data + i + 12);
        result1 = _mm256_add_pd(vector1, vector1_2);
        result2 = _mm256_add_pd(vector2, vector2_2);
        result3 = _mm256_add_pd(vector3, vector3_2);
        result4 = _mm256_add_pd(vector4, vector4_2);
        _mm256_storeu_pd(result_data + i + 0, result1);
        _mm256_storeu_pd(result_data + i + 4, result2);
        _mm256_storeu_pd(result_data + i + 8, result3);
        _mm256_storeu_pd(result_data + i + 12, result4);

        vector1 = _mm256_loadu_pd(mat1_data + i + 16);
        vector2 = _mm256_loadu_pd(mat1_data + i + 20);
        vector3 = _mm256_loadu_pd(mat1_data + i + 24);
        vector4 = _mm256_loadu_pd(mat1_data + i + 28);
        vector1_2 = _mm256_loadu_pd(mat2_data + i + 16);
        vector2_2 = _mm256_loadu_pd(mat2_data + i + 20);
        vector3_2 = _mm256_loadu_pd(mat2_data + i + 24);
        vector4_2 = _mm256_loadu_pd(mat2_data + i + 28);
        result1 = _mm256_add_pd(vector1, vector1_2);
        result2 = _mm256_add_pd(vector2, vector2_2);
        result3 = _mm256_add_pd(vector3, vector3_2);
        result4 = _mm256_add_pd(vector4, vector4_2);
        _mm256_storeu_pd(result_data + i + 16, result1);
        _mm256_storeu_pd(result_data + i + 20, result2);
        _mm256_storeu_pd(result_data + i + 24, result3);
        _mm256_storeu_pd(result_data + i + 28, result4);
    }
//    #pragma omp parallel for private(vector_tail)
    for (int i = length / 32 * 32; i < length / 4 * 4; i += 4) {
        vector_tail = _mm256_add_pd(_mm256_loadu_pd(mat1_data + i), _mm256_loadu_pd(mat2_data + i));
        _mm256_storeu_pd(result_data + i, vector_tail);
    }
    for (int i = length / 4 * 4; i < length; i++){
        result_data[i] = mat1_data[i] + mat2_data[i];
    }
    return 0;
}

/*
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    if (result == NULL || mat1 == NULL || mat2 == NULL) {
        PyErr_SetString(PyExc_TypeError, "Null ptr to the matrix(es).");
        return -1;
    }
    int row_num1 = mat1->rows;
    int row_num2 = mat2->rows;
    int row_num3 = result->rows;
    int col_num1 = mat1->cols;
    int col_num2 = mat2->cols;
    int col_num3 = result->cols;

    if (row_num1 != row_num2 || row_num1 != row_num3 || row_num2 != row_num3 ||
        col_num1 != col_num2 || col_num1 != col_num3 || col_num2 != col_num3) {
        PyErr_SetString(PyExc_ValueError, "Incompatible dimension(s).");
        return -1;
    }

    int length = row_num1 * col_num1;
    double* result_data = result->data;
    double* mat1_data = mat1->data;
    double* mat2_data = mat2->data;

//    for (int i = 0; i < length; i++) {
//        result_data[i] = mat1_data[i] - mat2_data[i];
//    }
    __m256d vector1, vector2, vector3, vector4,
            vector1_2, vector2_2, vector3_2, vector4_2,
            result1, result2, result3, result4,
            vector_tail;
    #pragma omp parallel for private(vector1, vector2, vector3, vector4, vector1_2, vector2_2, vector3_2, vector4_2, result1, result2, result3, result4)
    for (int i = 0; i < length / 32 * 32; i += 32){
        vector1 = _mm256_loadu_pd(mat1_data + i + 0);
        vector2 = _mm256_loadu_pd(mat1_data + i + 4);
        vector3 = _mm256_loadu_pd(mat1_data + i + 8);
        vector4 = _mm256_loadu_pd(mat1_data + i + 12);
        vector1_2 = _mm256_loadu_pd(mat2_data + i + 0);
        vector2_2 = _mm256_loadu_pd(mat2_data + i + 4);
        vector3_2 = _mm256_loadu_pd(mat2_data + i + 8);
        vector4_2 = _mm256_loadu_pd(mat2_data + i + 12);
        result1 = _mm256_sub_pd(vector1, vector1_2);
        result2 = _mm256_sub_pd(vector2, vector2_2);
        result3 = _mm256_sub_pd(vector3, vector3_2);
        result4 = _mm256_sub_pd(vector4, vector4_2);
        _mm256_storeu_pd(result_data + i + 0, result1);
        _mm256_storeu_pd(result_data + i + 4, result2);
        _mm256_storeu_pd(result_data + i + 8, result3);
        _mm256_storeu_pd(result_data + i + 12, result4);

        vector1 = _mm256_loadu_pd(mat1_data + i + 16);
        vector2 = _mm256_loadu_pd(mat1_data + i + 20);
        vector3 = _mm256_loadu_pd(mat1_data + i + 24);
        vector4 = _mm256_loadu_pd(mat1_data + i + 28);
        vector1_2 = _mm256_loadu_pd(mat2_data + i + 16);
        vector2_2 = _mm256_loadu_pd(mat2_data + i + 20);
        vector3_2 = _mm256_loadu_pd(mat2_data + i + 24);
        vector4_2 = _mm256_loadu_pd(mat2_data + i + 28);
        result1 = _mm256_sub_pd(vector1, vector1_2);
        result2 = _mm256_sub_pd(vector2, vector2_2);
        result3 = _mm256_sub_pd(vector3, vector3_2);
        result4 = _mm256_sub_pd(vector4, vector4_2);
        _mm256_storeu_pd(result_data + i + 16, result1);
        _mm256_storeu_pd(result_data + i + 20, result2);
        _mm256_storeu_pd(result_data + i + 24, result3);
        _mm256_storeu_pd(result_data + i + 28, result4);
    }
    #pragma omp parallel for private(vector_tail)
    for (int i = length / 32 * 32; i < length / 4 * 4; i += 4) {
        vector_tail = _mm256_sub_pd(_mm256_loadu_pd(mat1_data + i), _mm256_loadu_pd(mat2_data + i));
        _mm256_storeu_pd(result_data + i, vector_tail);
    }
    for (int i = length / 4 * 4; i < length; i++){
        result_data[i] = mat1_data[i] - mat2_data[i];
    }
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    if (result == NULL || mat1 == NULL || mat2 == NULL) {
        PyErr_SetString(PyExc_TypeError, "Null ptr to the matrix(es).");
        return -1;
    }
    int row_num1 = mat1->rows;
    int row_num2 = mat2->rows;
    int row_num3 = result->rows;
    int col_num1 = mat1->cols;
    int col_num2 = mat2->cols;
    int col_num3 = result->cols;
    if (row_num1 != row_num3 || col_num1 != row_num2 || col_num2 != col_num3) {
        PyErr_SetString(PyExc_ValueError, "Incompatible dimension(s).");
        return -1;
    }

//    fill_matrix(result, 0);
//
//    __m256d const_vector = _mm256_set1_pd(0);
    int result_length = row_num3 * col_num3;
//    #pragma omp parallel for
//    for (int i = 0; i < result_length / 16 * 16; i += 16){
//        _mm256_storeu_pd(result->data + i + 0, const_vector);
//        _mm256_storeu_pd(result->data + i + 4, const_vector);
//        _mm256_storeu_pd(result->data + i + 8, const_vector);
//        _mm256_storeu_pd(result->data + i + 12, const_vector);
//    }
////    #pragma omp parallel for
//    for (int i = result_length / 16 * 16; i < result_length / 4 * 4; i += 4) {
//        _mm256_storeu_pd(result->data + i, const_vector);
//    }
//    for (int i = result_length / 4 * 4; i < result_length; i++){
//        result->data[i] = 0;
//    }

    // for small matrices with # of element < 2500
//    if (row_num1 <= 50 && col_num1 <= 50 && col_num2 <= 50) {
//        for (int i = 0; i < row_num1; i++) {
//            for (int j = 0; j < col_num2; j++) {
//                for (int k = 0; k < col_num1; k++) {
//                    result->data[i * col_num3 + j] += get(mat1, i, k) * get(mat2, k, j);
//                }
//            }
//        }
//        return 0;
//    }

    double* mat1_data = mat1->data;
    double* mat2_data = mat2->data;
    double* result_data = result->data;

    int mat2_length = row_num2 * col_num2;
    double* transposed_mat2 = malloc(mat2_length * sizeof(double));
    if(transposed_mat2 == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Allocation to double* failed.");
        return -1;
    }
//    __m256d item1, item2, item3, item4;
//    #pragma omp parallel for private(item1, item2, item3, item4)
//    for (int i = 0; i < mat2_length/16; i+= 16) {
//        item1 = _mm256_loadu_pd(mat2_data + i / row_num2 + (i % row_num2) * col_num2);
//        item2 = _mm256_loadu_pd(mat2_data + (i + 4) / row_num2 + ((i + 4) % row_num2) * col_num2);
//        item3 = _mm256_loadu_pd(mat2_data + (i + 8) / row_num2 + ((i + 8) % row_num2) * col_num2);
//        item4 = _mm256_loadu_pd(mat2_data + (i + 12) / row_num2 + ((i + 12) % row_num2) * col_num2);
//        _mm256_storeu_pd(transposed_mat2 + i, item1);
//        _mm256_storeu_pd(transposed_mat2 + (i + 4), item2);
//        _mm256_storeu_pd(transposed_mat2 + (i + 8), item3);
//        _mm256_storeu_pd(transposed_mat2 + (i + 12), item4);
//    }
//
//    for (int i = mat2_length/16; i < mat2_length; i++) {
//        transposed_mat2[i] = mat2_data[i/row_num2 + (i % row_num2) * col_num2];
//    }

    #pragma omp parallel for
    for (int i = 0; i < mat2_length/4 * 4; i+=4) {
        transposed_mat2[i] = mat2_data[i/row_num2 + (i % row_num2) * col_num2];
        transposed_mat2[i + 1] = mat2_data[(i + 1)/row_num2 + ((i + 1) % row_num2) * col_num2];
        transposed_mat2[i + 2] = mat2_data[(i + 2)/row_num2 + ((i + 2) % row_num2) * col_num2];
        transposed_mat2[i + 3] = mat2_data[(i + 3)/row_num2 + ((i + 3) % row_num2) * col_num2];
    }

    for (int i = mat2_length/4 * 4; i < mat2_length; i ++ ) {
        transposed_mat2[i] = mat2_data[i/row_num2 + (i % row_num2) * col_num2];
    }

    // try transposing with blocking:



    double cell;
    double* mat1_rowptr;
    double* tmat2_rowptr;
//    int fat_mat1 = col_num1 > 500;
    __m256d vector1, vector2, vector3, vector4,
            vector1_2, vector2_2, vector3_2, vector4_2,
            result1, result2, result3, result4,
            sub_cells;
    #pragma omp parallel for private(cell, mat1_rowptr, tmat2_rowptr, sub_cells, vector1, vector2, vector3, vector4, vector1_2, vector2_2, vector3_2, vector4_2, result1, result2, result3, result4)
    for (int i = 0; i < result_length; i++){
        cell = 0;
        sub_cells = _mm256_set1_pd(0.0);
        mat1_rowptr = mat1_data + (i / col_num3) * col_num1;
        // i % col_num2 = column number in mat2 = distance from matrix top to the row in transposed mat2
        tmat2_rowptr = transposed_mat2 + (i % col_num2) * row_num2;
        int j;

        for (j = 0 ; j < col_num1 / 16 * 16; j += 16){
            vector1 = _mm256_loadu_pd (mat1_rowptr + j + 0);
            vector1_2 = _mm256_loadu_pd (tmat2_rowptr + j + 0);
            vector2 = _mm256_loadu_pd (mat1_rowptr + j + 4);
            vector2_2 = _mm256_loadu_pd (tmat2_rowptr + j + 4);
            vector3 = _mm256_loadu_pd (mat1_rowptr + j + 8);
            vector3_2 = _mm256_loadu_pd (tmat2_rowptr + j + 8);
            vector4 = _mm256_loadu_pd (mat1_rowptr + j + 12);
            vector4_2 = _mm256_loadu_pd (tmat2_rowptr + j + 12);
            result1 = _mm256_mul_pd(vector1, vector1_2);
            result2 = _mm256_mul_pd(vector2, vector2_2);
            result3 = _mm256_mul_pd(vector3, vector3_2);
            result4 = _mm256_mul_pd(vector4, vector4_2);
            sub_cells = _mm256_add_pd(sub_cells, result1);
            sub_cells = _mm256_add_pd(sub_cells, result2);
            sub_cells = _mm256_add_pd(sub_cells, result3);
            sub_cells = _mm256_add_pd(sub_cells, result4);

//            vector1 = _mm256_loadu_pd (mat1_rowptr + j + 16);
//            vector1_2 = _mm256_loadu_pd (tmat2_rowptr + j + 16);
//            vector2 = _mm256_loadu_pd (mat1_rowptr + j + 20);
//            vector2_2 = _mm256_loadu_pd (tmat2_rowptr + j + 20);
//            vector3 = _mm256_loadu_pd (mat1_rowptr + j + 24);
//            vector3_2 = _mm256_loadu_pd (tmat2_rowptr + j + 24);
//            vector4 = _mm256_loadu_pd (mat1_rowptr + j + 28);
//            vector4_2 = _mm256_loadu_pd (tmat2_rowptr + j + 28);
//            vector1 = _mm256_mul_pd(vector1, vector1_2);
//            vector2 = _mm256_mul_pd(vector2, vector2_2);
//            vector3 = _mm256_mul_pd(vector3, vector3_2);
//            vector4 = _mm256_mul_pd(vector4, vector4_2);
//            sub_cells = _mm256_add_pd(sub_cells, vector1);
//            sub_cells = _mm256_add_pd(sub_cells, vector2);
//            sub_cells = _mm256_add_pd(sub_cells, vector3);
//            sub_cells = _mm256_add_pd(sub_cells, vector4);
        }

        for (j = col_num1 / 16 * 16; j < col_num1/4 * 4; j+=4) {
            vector1 = _mm256_loadu_pd (mat1_rowptr + j + 0);
            vector1_2 = _mm256_loadu_pd (tmat2_rowptr + j + 0);
            result1 = _mm256_mul_pd(vector1, vector1_2);
            sub_cells = _mm256_add_pd(sub_cells, result1);
        }
        cell = sub_cells[0] + sub_cells[1] + sub_cells[2] + sub_cells[3];

        for (j = col_num1 / 4 * 4; j < col_num1; j++) {
            cell += *(mat1_rowptr + j) * (*(tmat2_rowptr + j));
        }

        result_data[i] = cell;
    }
//    for (int i = 0; i < row_num1; i++) {
//        for (int j = 0; j < col_num2; j++) {
//            for (int k = 0; k < col_num1; k++) {
//                result->data[i * col_num3 + j] += get(mat1, i, k) * get(mat2, k, j);
//            }
//        }
//    }
    free(transposed_mat2);
    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    /* TODO: YOUR CODE HERE */
    if (result == NULL || mat == NULL) {
        PyErr_SetString(PyExc_TypeError, "Null ptr to the matrix(es).");
        return -1;
    }

    int row_num1 = mat->rows;
    int row_num3 = result->rows;
    int col_num1 = mat->cols;
    int col_num3 = result->cols;

    if (row_num1 != col_num1 || row_num1 != row_num3 || col_num1 != col_num3) {
        PyErr_SetString(PyExc_ValueError, "Non-square matrix.");
        return -1;
    }

    if (pow < 0) {
        PyErr_SetString(PyExc_ValueError, "pow < 0.");
        return -1;
    }

    // spare used to store result and avoid interference
    matrix *spare;
    if (allocate_matrix(&spare, row_num3, col_num3)) {
//        PyErr_SetString(PyExc_RuntimeError, "Allocation failed.");
        return -1;
    }

    // spare2 is a spare place for temp_result to move.
    matrix *temp_operand, *temp_store, *spare2, *here1, *here2;
    temp_operand = mat;
    if (allocate_matrix(&temp_store, row_num1, col_num1)) {
//        PyErr_SetString(PyExc_RuntimeError, "Allocation failed.");
        return -1;
    }
    if (allocate_matrix(&spare2, row_num1, col_num1)) {
//        PyErr_SetString(PyExc_RuntimeError, "Allocation failed.");
        return -1;
    }
    here1 = temp_store;
    here2 = spare2;

//    fprintf(stderr, "%s ", "1");

//    fill_matrix(result, 0);
//    for (int i = 0; i < row_num3; i++) {
//        set(result, i, i, 1);
//    }
    double *result_data = result->data;
    #pragma omp parallel for
    for (int i = 0; i < row_num3 / 8 * 8; i += 8){
        result_data[i * col_num3 + i] = 1;
        result_data[(i+1) * col_num3 + i + 1] = 1;
        result_data[(i+2) * col_num3 + i + 2] = 1;
        result_data[(i+3) * col_num3 + i + 3] = 1;
        result_data[(i+4) * col_num3 + i + 4] = 1;
        result_data[(i+5) * col_num3 + i + 5] = 1;
        result_data[(i+6) * col_num3 + i + 6] = 1;
        result_data[(i+7) * col_num3 + i + 7] = 1;
    }
//    #pragma omp parallel for
    for (int i = row_num3 / 8 * 8; i < row_num3; i++){
        result_data[i * col_num3 + i] = 1;
    }

    if (pow == 0) {
        return 0;
    }

//    for (int i = 0; i < row_num3 * col_num3; i ++) {
//        fprintf(stderr, "%f ", result->data[i]);
//    }
    double* temp;
    // Algorithm credit to Addition-chain exponentiation: https://en.wikipedia.org/wiki/Addition-chain_exponentiation
    while (pow > 0) {
        if (pow % 2 == 1) {
            if(mul_matrix(spare, result, temp_operand)) {
                deallocate_matrix(spare);
                deallocate_matrix(here1);
                deallocate_matrix(here2);
                return -1;
            }
            temp = result->data;
            result->data = spare->data;
            spare->data = temp;
        }
        if(mul_matrix(temp_store, temp_operand, temp_operand)) {
            deallocate_matrix(spare);
            deallocate_matrix(here1);
            deallocate_matrix(here2);
            return -1;
        }
//        for (int i = 0; i < row_num3 * col_num3; i ++) {
//            fprintf(stderr, "%f ", temp_operand->data[i]);
//        }
//        for (int i = 0; i < row_num3 * col_num3; i ++) {
//            fprintf(stderr, "%f ", temp_store->data[i]);
//        }
        temp_operand = temp_store;
        temp_store = spare2;
        spare2 = temp_operand;
        pow = pow / 2;
//        for (int i = 0; i < row_num3 * col_num3; i ++) {
//            fprintf(stderr, "%f ", temp_operand->data[i]);
//        }
//        for (int i = 0; i < row_num3 * col_num3; i ++) {
//            fprintf(stderr, "%f ", temp_store->data[i]);
//        }
//        fprintf(stderr, "\n");
    }

//    for (int i = 0; i < row_num3 * col_num3; i ++) {
//        fprintf(stderr, "%f ", result->data[i]);
//    }
    deallocate_matrix(spare);
    deallocate_matrix(here1);
    deallocate_matrix(here2);
    return 0;
}

/*
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (result == NULL || mat == NULL) {
        PyErr_SetString(PyExc_TypeError, "Null ptr to the matrix(es).");
        return -1;
    }

    int row_num1 = mat->rows;
    int row_num3 = result->rows;
    int col_num1 = mat->cols;
    int col_num3 = result->cols;

    if (row_num1 != row_num3 || col_num1 != col_num3) {
        PyErr_SetString(PyExc_ValueError, "Incompatible dimension(s).");
        return -1;
    }

    int result_length = row_num3 * col_num3;
    double* result_data = result->data;
    double* mat1_data = mat->data;

//    for(int i = 0; i < result_length; i++) {
//        result_data[i] = -1 * mat1_data[i];
//    }

    __m256d zeros = _mm256_set1_pd(0.0);
    __m256d vector1, vector2, vector3, vector4, vector_tail,
            medium1, medium2, medium3, medium4, medium_tail;
    #pragma omp parallel for private(vector1, vector2, vector3, vector4, medium1, medium2, medium3, medium4)
    for (int i = 0; i < result_length / 16 * 16; i += 16){
        medium1 = _mm256_loadu_pd(mat1_data + i + 0);
        medium2 = _mm256_loadu_pd(mat1_data + i + 4);
        medium3 = _mm256_loadu_pd(mat1_data + i + 8);
        medium4 = _mm256_loadu_pd(mat1_data + i + 12);
        vector1 = _mm256_sub_pd(zeros, medium1);
        vector2 = _mm256_sub_pd(zeros, medium2);
        vector3 = _mm256_sub_pd(zeros, medium3);
        vector4 = _mm256_sub_pd(zeros, medium4);
        _mm256_storeu_pd(result_data + i + 0, vector1);
        _mm256_storeu_pd(result_data + i + 4, vector2);
        _mm256_storeu_pd(result_data + i + 8, vector3);
        _mm256_storeu_pd(result_data + i + 12, vector4);
    }
//    #pragma omp parallel for private(vector_tail, medium_tail)
    for (int i = result_length / 16 * 16; i < result_length / 4 * 4; i += 4) {
        medium_tail = _mm256_loadu_pd(mat1_data + i);
        vector_tail = _mm256_sub_pd(zeros, medium_tail);
        _mm256_storeu_pd(result_data + i, vector_tail);
    }
    for (int i = result_length / 4 * 4; i < result_length; i++){
        result_data[i] = -mat1_data[i];
    }

    return 0;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int abs_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (result == NULL || mat == NULL) {
        PyErr_SetString(PyExc_TypeError, "Null ptr to the matrix(es).");
        return -1;
    }

    int row_num1 = mat->rows;
    int row_num3 = result->rows;
    int col_num1 = mat->cols;
    int col_num3 = result->cols;

    if (row_num1 != row_num3 || col_num1 != col_num3) {
        PyErr_SetString(PyExc_ValueError, "Incompatible dimension(s).");
        return -1;
    }

    int result_length = row_num3 * col_num3;
    double* result_data = result->data;
    double* mat1_data = mat->data;

//    for(int i = 0; i < result_length; i++) {
//        double mat1_num = mat1_data[i];
//        if (mat1_num >= 0) {
//            result_data[i] = mat1_data[i];
//        } else {
//            result_data[i] = -1 * mat1_data[i];
//        }
//    }


    __m256d zeros = _mm256_set1_pd(0.0);
    __m256d vector1, vector2, vector3, vector4, vector_tail,
            medium1, medium2, medium3, medium4, medium_tail,
            negate1, negate2, negate3, negate4, negate_tail;
    #pragma omp parallel for private(vector1, vector2, vector3, vector4, medium1, medium2, medium3, medium4, negate1, negate2, negate3, negate4)
    for (int i = 0; i < result_length / 16 * 16; i += 16) {
        medium1 = _mm256_loadu_pd(mat1_data + i + 0);
        medium2 = _mm256_loadu_pd(mat1_data + i + 4);
        medium3 = _mm256_loadu_pd(mat1_data + i + 8);
        medium4 = _mm256_loadu_pd(mat1_data + i + 12);
        negate1 = _mm256_sub_pd(zeros, medium1);
        negate2 = _mm256_sub_pd(zeros, medium2);
        negate3 = _mm256_sub_pd(zeros, medium3);
        negate4 = _mm256_sub_pd(zeros, medium4);
        vector1 = _mm256_max_pd(medium1, negate1);
        vector2 = _mm256_max_pd(medium2, negate2);
        vector3 = _mm256_max_pd(medium3, negate3);
        vector4 = _mm256_max_pd(medium4, negate4);
        _mm256_storeu_pd(result_data + i + 0, vector1);
        _mm256_storeu_pd(result_data + i + 4, vector2);
        _mm256_storeu_pd(result_data + i + 8, vector3);
        _mm256_storeu_pd(result_data + i + 12, vector4);
    }
//    #pragma omp parallel for private(vector_tail, medium_tail, negate_tail)
    for (int i = result_length / 16 * 16; i < result_length / 4 * 4; i += 4) {
        medium_tail = _mm256_loadu_pd(mat1_data + i);
        negate_tail = _mm256_sub_pd(zeros, medium_tail);;
        vector_tail = _mm256_max_pd(medium_tail, negate_tail);
        _mm256_storeu_pd(result_data + i, vector_tail);
    }

    double val;
    for (int i = result_length / 4 * 4; i < result_length; i++){
        val = mat1_data[i];
        result_data[i] = val > 0 ? val : -val;
    }
    return 0;
}

