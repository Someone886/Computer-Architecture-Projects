from utils import *
from unittest import TestCase

"""
- For each operation, you should write tests to test  on matrices of different sizes.
- Keep in mind that the tests provided in the starter code are NOT comprehensive. That is, we strongly
advise you to modify them and add new tests.
- Hint: use dp_mc_matrix to generate dumbpy and numc matrices with the same data and use
      cmp_dp_nc_matrix to compare the results
"""
class TestAdd(TestCase):
    def test_small_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 2, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium1_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(350, 467, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(350, 467, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium2_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(853, 947, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(853, 947, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large1_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1314, 2588, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1314, 2588, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large2_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(4062, 6042, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(4062, 6042, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_very_large1_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(8848, 9627, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(8848, 9627, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_very_large2_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(11111, 23333, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(11111, 23333, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_exception1_add(self):
        try:
            nc.Matrix(3, 3) + nc.Matrix(2, 3)
            self.assertTrue(False)
        except ValueError as e:
            print(e)
            pass

    def test_exception2_add(self):
        try:
            nc.Matrix(3, 3) + None
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception3_add(self):
        try:
            nc.Matrix(3, 3) + nc.Matrix(3, 2)
            self.assertTrue(False)
        except ValueError as e:
            print(e)
            pass

class TestSub(TestCase):
    def test_small_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 2, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)

        print_speedup(speed_up)

    def test_medium1_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(350, 467, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(350, 467, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium2_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(853, 947, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(853, 947, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large1_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1314, 2588, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1314, 2588, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large2_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(4062, 6042, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(4062, 6042, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_very_large1_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(8848, 9627, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(8848, 9627, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_very_large2_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(11111, 23333, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(11111, 23333, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_exception1_sub(self):
        try:
            nc.Matrix(3, 3) - nc.Matrix(3, 2)
            self.assertTrue(False)
        except ValueError as e:
            print(e)
            pass

    def test_exception2_sub(self):
        try:
            nc.Matrix(3, 3) - None
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception3_sub(self):
        try:
            nc.Matrix(3, 3) - nc.Matrix(2, 3)
            self.assertTrue(False)
        except ValueError as e:
            print(e)
            pass

class TestAbs(TestCase):
    def test_small_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium1_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(350, 467, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium2_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(853, 947, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large1_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1314, 2588, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large2_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(4062, 6042, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_very_large1_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(8848, 9627, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_very_large2_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(11111, 23333, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestNeg(TestCase):
    def test_small_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium1_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(350, 467, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium2_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(853, 947, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large1_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1314, 2588, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large2_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(4062, 6042, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_very_large1_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(8848, 9627, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_very_large2_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(11111, 23333, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestMul(TestCase):
    def test_small_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(10, 12, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(12, 14, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium1_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(111, 222, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(222, 333, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium2_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(555, 666, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(666, 777, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large1_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1001, 1111, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1111, 1110, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    # def test_large2_mul(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat1, nc_mat1 = rand_dp_nc_matrix(8848, 2333, seed=0)
    #     dp_mat2, nc_mat2 = rand_dp_nc_matrix(2333, 7777, seed=1)
    #     is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)
    #
    #
    # def test_large3_mul(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat1, nc_mat1 = rand_dp_nc_matrix(4003, 4777, seed=0)
    #     dp_mat2, nc_mat2 = rand_dp_nc_matrix(4777, 5002, seed=1)
    #     is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)
    #
    # def test_strange1_mul(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat1, nc_mat1 = rand_dp_nc_matrix(8848, 69, seed=0)
    #     dp_mat2, nc_mat2 = rand_dp_nc_matrix(69, 8848, seed=1)
    #     is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)
    #
    # def test_strange2_mul(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat1, nc_mat1 = rand_dp_nc_matrix(12345, 1, seed=0)
    #     dp_mat2, nc_mat2 = rand_dp_nc_matrix(1, 54321, seed=1)
    #     is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

    def test_exception1_mul(self):
        try:
            nc.Matrix(3, 2) * nc.Matrix(3, 2)
            self.assertTrue(False)
        except ValueError as e:
            print(e)
            pass

    def test_exception2_mul(self):
        try:
            nc.Matrix(3, 2) * None
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

class TestPow(TestCase):
    def test_small1_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        is_correct, speed_up = compute([dp_mat, 3], [nc_mat, 3], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small2_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(7, 7, seed=0)
        is_correct, speed_up = compute([dp_mat, 7], [nc_mat, 7], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium1_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(30, 30, seed=0)
        is_correct, speed_up = compute([dp_mat, 5], [nc_mat, 5], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium2_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(10, 10, seed=0)
        is_correct, speed_up = compute([dp_mat, 900], [nc_mat, 900], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium3_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(111, 111, seed=0)
        is_correct, speed_up = compute([dp_mat, 900], [nc_mat, 900], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium4_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(120, 120, seed=0)
        is_correct, speed_up = compute([dp_mat, 1200], [nc_mat, 1200], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    # def test_large1_pow(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(233, 233, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 13], [nc_mat, 13], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)
    #
    # def test_large2_pow(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(233, 233, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 211], [nc_mat, 211], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)
    #
    # def test_large3_pow(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(47, 47, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 233], [nc_mat, 233], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

    # def test_very_large1_pow(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(876, 876, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 477], [nc_mat, 477], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)
    #
    # def test_very_large2_pow(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(997, 997, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 999], [nc_mat, 999], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

    def test_exception1_pow(self):
        try:
            nc.Matrix(3, 2) ** 3
            self.assertTrue(False)
        except ValueError as e:
            print(e)
            pass

    def test_exception2_pow(self):
        try:
            nc.Matrix(3, 3) ** -1
            self.assertTrue(False)
        except ValueError as e:
            print(e)
            pass

    def test_exception3_pow(self):
        try:
            None ** 4
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception4_pow(self):
        try:
            nc.Matrix(3, 3) ** 1.5
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

class TestGet(TestCase):
    def test_get(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEqual(round(dp_mat.get(rand_row, rand_col), decimal_places),
            round(nc_mat.get(rand_row, rand_col), decimal_places))

class TestSet(TestCase):
    def test_set(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        dp_mat.set(rand_row, rand_col, 2)
        nc_mat.set(rand_row, rand_col, 2)
        self.assertTrue(cmp_dp_nc_matrix(dp_mat, nc_mat))
        self.assertEquals(nc_mat.get(rand_row, rand_col), 2)

class TestShape(TestCase):
    def test_shape(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        self.assertTrue(dp_mat.shape == nc_mat.shape)

class TestIndexGet(TestCase):
    def test_index_get(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEqual(round(dp_mat[rand_row][rand_col], decimal_places),
            round(nc_mat[rand_row][rand_col], decimal_places))

    def test_exception1_get(self):
        try:
            (nc.Matrix(3, 3)).get(0)
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception2_get(self):
        try:
            (nc.Matrix(3, 3)).get(1, 1.5)
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception3_get(self):
        try:
            (nc.Matrix(3, 3)).get(1.5, 1)
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception4_get(self):
        try:
            (nc.Matrix(3, 3)).get(1, 3)
            self.assertTrue(False)
        except IndexError as e:
            print(e)
            pass

    def test_exception5_get(self):
        try:
            (nc.Matrix(3, 3)).get(3, 1)
            self.assertTrue(False)
        except IndexError as e:
            print(e)
            pass

class TestIndexSet(TestCase):
    def test_set(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        dp_mat[rand_row][rand_col] = 2
        nc_mat[rand_row][rand_col] = 2
        self.assertTrue(cmp_dp_nc_matrix(dp_mat, nc_mat))
        self.assertEquals(nc_mat[rand_row][rand_col], 2)

    def test_exception1_set(self):
        try:
            (nc.Matrix(3, 3)).set(0, 1)
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception2_set(self):
        try:
            (nc.Matrix(3, 3)).set(1, 1.5, 1)
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception3_set(self):
        try:
            (nc.Matrix(3, 3)).set(1.5, 1, 1)
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception6_set(self):
        try:
            (nc.Matrix(3, 3)).set(1, 1, None)
            self.assertTrue(False)
        except TypeError as e:
            print(e)
            pass

    def test_exception4_set(self):
        try:
            (nc.Matrix(3, 3)).set(1, 3, 1)
            self.assertTrue(False)
        except IndexError as e:
            print(e)
            pass

    def test_exception5_set(self):
        try:
            (nc.Matrix(3, 3)).set(3, 1, 1)
            self.assertTrue(False)
        except IndexError as e:
            print(e)
            pass

class TestSlice(TestCase):
    def test_slice(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        self.assertTrue(cmp_dp_nc_matrix(dp_mat[0], nc_mat[0]))
        self.assertTrue(cmp_dp_nc_matrix(dp_mat[1], nc_mat[1]))
