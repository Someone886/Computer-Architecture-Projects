#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "../src/matrix.h"
#include <stdio.h>

/* Test Suite setup and cleanup functions: */
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test case functions ****************/
void add_test(void) {
  matrix *result = NULL;
  matrix *mat1 = NULL;
  matrix *mat2 = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 1000, 1000), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 1000, 1000), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 1000, 1000), 0);
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      set(mat1, i, j, i * 2 + j);
      set(mat2, i, j, i * 2 + j);
    }
  }
  add_matrix(result, mat1, mat2);
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), 2 * (i * 2 + j));
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);
}

void sub_test(void) {
  matrix *result = NULL;
  matrix *mat1 = NULL;
  matrix *mat2 = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 1000, 1000), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 1000, 1000), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 1000, 1000), 0);
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      set(mat1, i, j, i * 2 + j);
      set(mat2, i, j, (i * 2 + j) * 3);
    }
  }
  sub_matrix(result, mat1, mat2);
  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < 1000; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), (-2) * (i * 2 + j));
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);
}

void mul_test(void) {
  matrix *result = NULL;
  matrix *mat1 = NULL;
  matrix *mat2 = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 3, 3), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 3, 3), 0);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      set(mat1, i, j, i * 3 + j + 1);
      set(mat2, i, j, i * 2 + j * 2 + 3);
    }
  }
  mul_matrix(result, mat1, mat2);
  CU_ASSERT_EQUAL(get(result, 0, 0), 34);
  CU_ASSERT_EQUAL(get(result, 0, 1), 46);
  CU_ASSERT_EQUAL(get(result, 0, 2), 58);
  CU_ASSERT_EQUAL(get(result, 1, 0), 79);
  CU_ASSERT_EQUAL(get(result, 1, 1), 109);
  CU_ASSERT_EQUAL(get(result, 1, 2), 139);
  CU_ASSERT_EQUAL(get(result, 2, 0), 124);
  CU_ASSERT_EQUAL(get(result, 2, 1), 172);
  CU_ASSERT_EQUAL(get(result, 2, 2), 220);
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);

  CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 3, 4), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 4, 3), 0);
  for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
          set(mat1, i, j, i * 3 + j + 1);
          set(mat2, j, i, i * 2 + j * 2 + 3);
      }
  }
  mul_matrix(result, mat1, mat2);
  CU_ASSERT_EQUAL(get(result, 0, 0), 70);
  CU_ASSERT_EQUAL(get(result, 0, 1), 90);
  CU_ASSERT_EQUAL(get(result, 0, 2), 110);
  CU_ASSERT_EQUAL(get(result, 1, 0), 142);
  CU_ASSERT_EQUAL(get(result, 1, 1), 186);
  CU_ASSERT_EQUAL(get(result, 1, 2), 230);
  CU_ASSERT_EQUAL(get(result, 2, 0), 214);
  CU_ASSERT_EQUAL(get(result, 2, 1), 282);
  CU_ASSERT_EQUAL(get(result, 2, 2), 350);
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);
}

void mul_fail_test(void) {
    matrix *result = NULL;
    matrix *mat1 = NULL;
    matrix *mat2 = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat1, 3, 2), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat2, 3, 1), 0);
    CU_ASSERT_EQUAL(mul_matrix(result, mat1, mat2), -1);
    CU_ASSERT_EQUAL(mul_matrix(result, mat2, mat1), -1);
}

void mul_large_test(void) {
    matrix *result = NULL;
    matrix *mat1 = NULL;
    matrix *mat2 = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 1000, 2300), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat1, 1000, 1000), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat2, 1000, 2300), 0);
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 2300; j++) {
            set(mat2, i, j, i * 2 + j * 2 + 3);
        }
    }
    for (int i = 0; i < 1000; i++) {
        set(mat1, i, i, 1);
    }
    mul_matrix(result, mat1, mat2);
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 2300; j++) {
            CU_ASSERT_EQUAL(get(result, i, j), (i * 2 + j * 2 + 3));
        }
    }
    deallocate_matrix(result);
    deallocate_matrix(mat1);
    deallocate_matrix(mat2);
}

void neg_test(void) {
  matrix *result = NULL;
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 100, 999), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 100, 999), 0);
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 999; j++) {
      set(mat, i, j, i * 2 + j);
    }
  }
  neg_matrix(result, mat);
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 999; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), -(i * 2 + j));
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat);
}

void abs_test(void) {
  matrix *result = NULL;
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 250, 999), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 250, 999), 0);
  for (double i = 0; i < 250; i++) {
    for (double j = 0; j < 999; j++) {
      if ((int)j % 2 == 0)
        set(mat, i, j, i * 2 + j);
      else
        set(mat, i, j, -(i * 2 + j));
    }
  }
  abs_matrix(result, mat);
  for (double i = 0; i < 250; i++) {
    for (double j = 0; j < 999; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), i * 2 + j);
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat);
}

void pow_test(void) {
  matrix *result = NULL;
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 3, 3), 0);
  set(mat, 0, 0, 448);
  set(mat, 0, 1, 583);
  set(mat, 0, 2, 290);
  set(mat, 1, 0, 547);
  set(mat, 1, 1, 634);
  set(mat, 1, 2, 756);
  set(mat, 2, 0, 72);
  set(mat, 2, 1, 161);
  set(mat, 2, 2, 678);
  pow_matrix(result, mat, 3);
  CU_ASSERT_EQUAL(get(result, 0, 0), 667972328);
  CU_ASSERT_EQUAL(get(result, 0, 1), 868168587);
  CU_ASSERT_EQUAL(get(result, 0, 2), 1189148890);
  CU_ASSERT_EQUAL(get(result, 1, 0), 833259703);
  CU_ASSERT_EQUAL(get(result, 1, 1), 1096206842);
  CU_ASSERT_EQUAL(get(result, 1, 2), 1604448484);
  CU_ASSERT_EQUAL(get(result, 2, 0), 257643208);
  CU_ASSERT_EQUAL(get(result, 2, 1), 356108989);
  CU_ASSERT_EQUAL(get(result, 2, 2), 648821398);

  CU_ASSERT_EQUAL(get(mat, 0, 0), 448);
  CU_ASSERT_EQUAL(get(mat, 0, 1), 583);
  CU_ASSERT_EQUAL(get(mat, 0, 2), 290);
  CU_ASSERT_EQUAL(get(mat, 1, 0), 547);
  CU_ASSERT_EQUAL(get(mat, 1, 1), 634);
  CU_ASSERT_EQUAL(get(mat, 1, 2), 756);
  CU_ASSERT_EQUAL(get(mat, 2, 0), 72);
  CU_ASSERT_EQUAL(get(mat, 2, 1), 161);
  CU_ASSERT_EQUAL(get(mat, 2, 2), 678);
  deallocate_matrix(result);
  deallocate_matrix(mat);
}

void pow_large_test(void) {
    matrix *result = NULL;
    matrix *mat = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat, 3, 3), 0);
    set(mat, 0, 0, 4);
    set(mat, 0, 1, 5);
    set(mat, 0, 2, 2);
    set(mat, 1, 0, 5);
    set(mat, 1, 1, 6);
    set(mat, 1, 2, 7);
    set(mat, 2, 0, 7);
    set(mat, 2, 1, 1);
    set(mat, 2, 2, 6);
    pow_matrix(result, mat, 10);
    CU_ASSERT_EQUAL(get(result, 0, 0), 91532547922);
    CU_ASSERT_EQUAL(get(result, 0, 1), 67629703829);
    CU_ASSERT_EQUAL(get(result, 0, 2), 82232790212);
    CU_ASSERT_EQUAL(get(result, 1, 0), 143463116343);
    CU_ASSERT_EQUAL(get(result, 1, 1), 105999309930);
    CU_ASSERT_EQUAL(get(result, 1, 2), 128887294835);
    CU_ASSERT_EQUAL(get(result, 2, 0), 98231234457);
    CU_ASSERT_EQUAL(get(result, 2, 1), 72579193915);
    CU_ASSERT_EQUAL(get(result, 2, 2), 88251064448);
    pow_matrix(result, mat, 12);
    CU_ASSERT_EQUAL(get(result, 0, 0), 17897090080681);
    CU_ASSERT_EQUAL(get(result, 0, 1), 13223453492280);
    CU_ASSERT_EQUAL(get(result, 0, 2), 16078751337720);
    CU_ASSERT_EQUAL(get(result, 1, 0), 28050950194560);
    CU_ASSERT_EQUAL(get(result, 1, 1), 20725737982321);
    CU_ASSERT_EQUAL(get(result, 1, 2), 25200982337880);
    CU_ASSERT_EQUAL(get(result, 2, 0), 19206887926320);
    CU_ASSERT_EQUAL(get(result, 2, 1), 14191209407040);
    CU_ASSERT_EQUAL(get(result, 2, 2), 17255472796681);
    deallocate_matrix(result);
    deallocate_matrix(mat);
}

void alloc_fail_test(void) {
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 0, 0), -1);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 0, 1), -1);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 1, 0), -1);
}

void alloc_success_test(void) {
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 3, 2), 0);
  CU_ASSERT_EQUAL(mat->parent, NULL);
  CU_ASSERT_EQUAL(mat->ref_cnt, 1);
  CU_ASSERT_EQUAL(mat->rows, 3);
  CU_ASSERT_EQUAL(mat->cols, 2);
  CU_ASSERT_NOT_EQUAL(mat->data, NULL);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      CU_ASSERT_EQUAL(get(mat, i, j), 0);
    }
  }
  deallocate_matrix(mat);
}

void alloc_ref_fail_test(void) {
  matrix *mat = NULL;
  matrix *from = NULL;
  CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 0, 0, 0), -1);
  CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 0, 0, 1), -1);
  CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 0, 1, 0), -1);
}

void alloc_ref_success_test(void) {
  matrix *mat = NULL;
  matrix *from = NULL;
  allocate_matrix(&from, 3, 2);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      set(from, i, j, i * 2 + j);
    }
  }
  CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 2, 2, 2), 0);
  CU_ASSERT_PTR_EQUAL(mat->data, from->data + 2);
  CU_ASSERT_PTR_EQUAL(mat->parent, from);
  CU_ASSERT_EQUAL(mat->parent->ref_cnt, 2);
  CU_ASSERT_EQUAL(mat->rows, 2);
  CU_ASSERT_EQUAL(mat->cols, 2);
  deallocate_matrix(from);
  deallocate_matrix(mat);
}

void dealloc_null_test(void) {
  matrix *mat = NULL;
  deallocate_matrix(mat); // Test the null case doesn't crash
}

void dealloc_success_test1(void) {
    matrix *mat = NULL;
    matrix *from = NULL;
    allocate_matrix(&from, 3, 2);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            set(from, i, j, i * 2 + j);
        }
    }
    CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 2, 2, 2), 0);
    CU_ASSERT_PTR_EQUAL(mat->data, from->data + 2);
    CU_ASSERT_PTR_EQUAL(mat->parent, from);
    CU_ASSERT_EQUAL(mat->parent->ref_cnt, 2);
    CU_ASSERT_EQUAL(mat->rows, 2);
    CU_ASSERT_EQUAL(mat->cols, 2);
    deallocate_matrix(from);
    CU_ASSERT_EQUAL(mat->parent->ref_cnt, 1);
    CU_ASSERT_EQUAL(mat->rows, 2);
    CU_ASSERT_EQUAL(mat->cols, 2);
    CU_ASSERT_EQUAL(get(mat, 0, 0), 2);
    CU_ASSERT_EQUAL(get(mat, 0, 1), 3);
    CU_ASSERT_EQUAL(get(mat, 1, 0), 4);
    CU_ASSERT_EQUAL(get(mat, 1, 1), 5);
    deallocate_matrix(mat);
}

void dealloc_success_test2(void) {
    matrix *mat = NULL;
    matrix *from = NULL;
    allocate_matrix(&from, 3, 2);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            set(from, i, j, i * 2 + j);
        }
    }
    CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 2, 2, 2), 0);
    CU_ASSERT_PTR_EQUAL(mat->data, from->data + 2);
    CU_ASSERT_PTR_EQUAL(mat->parent, from);
    CU_ASSERT_EQUAL(mat->parent->ref_cnt, 2);
    CU_ASSERT_EQUAL(mat->rows, 2);
    CU_ASSERT_EQUAL(mat->cols, 2);
    deallocate_matrix(mat);
    CU_ASSERT_EQUAL(get(from, 0, 0), 0);
    CU_ASSERT_EQUAL(get(from, 0, 1), 1);
    CU_ASSERT_EQUAL(get(from, 1, 0), 2);
    CU_ASSERT_EQUAL(get(from, 1, 1), 3);
    deallocate_matrix(from);
}

void get_test(void) {
  matrix *mat = NULL;
  allocate_matrix(&mat, 2, 2);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      set(mat, i, j, i * 2 + j);
    }
  }
  CU_ASSERT_EQUAL(get(mat, 0, 0), 0);
  CU_ASSERT_EQUAL(get(mat, 0, 1), 1);
  CU_ASSERT_EQUAL(get(mat, 1, 0), 2);
  CU_ASSERT_EQUAL(get(mat, 1, 1), 3);
  deallocate_matrix(mat);
}

void set_test(void) {
  matrix *mat = NULL;
  allocate_matrix(&mat, 2, 2);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      set(mat, i, j, i * 2 + j);
    }
  }
  CU_ASSERT_EQUAL(get(mat, 0, 0), 0);
  CU_ASSERT_EQUAL(get(mat, 0, 1), 1);
  CU_ASSERT_EQUAL(get(mat, 1, 0), 2);
  CU_ASSERT_EQUAL(get(mat, 1, 1), 3);
  deallocate_matrix(mat);
}

/************* Test Runner Code goes here **************/

int main (void)
{
  Py_Initialize(); // Need to call this so that Python.h functions won't segfault
  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("mat_test_suite", init_suite, clean_suite);
  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   /* add the tests to the suite */
   if ((CU_add_test(pSuite, "add_test", add_test) == NULL) ||
        (CU_add_test(pSuite, "sub_test", sub_test) == NULL) ||
        (CU_add_test(pSuite, "mul_test", mul_test) == NULL) ||
        (CU_add_test(pSuite, "neg_test", neg_test) == NULL) ||
        (CU_add_test(pSuite, "abs_test", abs_test) == NULL) ||
        (CU_add_test(pSuite, "pow_test", pow_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_fail_test", alloc_fail_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_success_test", alloc_success_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_ref_fail_test", alloc_ref_fail_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_ref_success_test", alloc_ref_success_test) == NULL) ||
        (CU_add_test(pSuite, "dealloc_null_test", dealloc_null_test) == NULL) ||
        (CU_add_test(pSuite, "get_test", get_test) == NULL) ||
        (CU_add_test(pSuite, "set_test", set_test) == NULL) ||
        (CU_add_test(pSuite, "dealloc_success_test1", dealloc_success_test1) == NULL) ||
        (CU_add_test(pSuite, "dealloc_success_test2", dealloc_success_test2) == NULL) ||
        (CU_add_test(pSuite, "mul_fail_test", mul_fail_test) == NULL) ||
        (CU_add_test(pSuite, "pow_large_test", pow_large_test) == NULL) ||
        (CU_add_test(pSuite, "mul_large_test", mul_large_test) == NULL)
     )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

  // Run all tests using the basic interface
  CU_basic_set_mode(CU_BRM_NORMAL);
//  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  printf("\n");
  CU_basic_show_failures(CU_get_failure_list());
  printf("\n\n");

  /* Clean up registry and return */
  CU_cleanup_registry();
  return CU_get_error();
}