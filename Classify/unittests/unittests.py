from unittest import TestCase
from framework import AssemblyTest, print_coverage


class TestAbs(TestCase):
    def test_zero(self):
        t = AssemblyTest(self, "abs.s")
        # load 0 into register a0
        t.input_scalar("a0", 0)
        # call the abs function
        t.call("abs")
        # check that after calling abs, a0 is equal to 0 (abs(0) = 0)
        t.check_scalar("a0", 0)
        # generate the `assembly/TestAbs_test_zero.s` file and run it through venus
        t.execute()

    def test_one(self):
        # same as test_zero, but with input 1
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", 1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()

    def test_minus_one(self):
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", -1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("abs.s", verbose=False)


class TestRelu(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "relu.s")
        # create an array in the data section
        array0 = t.array([1, -2, 3, -4, 5, -6, 7, -8, 9])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the relu function
        t.call("relu")
        # check that the array0 was changed appropriately
        t.check_array(array0, [1, 0, 3, 0, 5, 0, 7, 0, 9])
        # generate the `assembly/TestRelu_test_simple.s` file and run it through venus
        t.execute()

    def test_error(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([1, 2, 3, 4, 5])
        t.input_array("a0", array0)
        t.input_scalar("a1", 0)
        t.call("relu")
        t.execute(115)

    def test_all_pos(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.execute()

    def test_all_neg(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([-1, -2, -3, -4, -5, -6, -7, -8, -9])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [0, 0, 0, 0, 0, 0, 0, 0, 0])
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("relu.s", verbose=False)


class TestArgmax(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "argmax.s")
        # create an array in the data section
        array0 = t.array([1,2,3,4,5,6,7,8,9])
        # load address of the array into register a0
        t.input_array("a0", array0)
        # set a1 to the length of the array
        t.input_scalar("a1", len(array0))
        # call the `argmax` function
        t.call("argmax")
        # check that the register a0 contains the correct output
        t.check_scalar("a0", 8)
        # generate the `assembly/TestArgmax_test_simple.s` file and run it through venus
        t.execute()

    def test_first(self):
        t = AssemblyTest(self, "argmax.s")
        # create an array in the data section
        array0 = t.array([1, 1, 1, 1, 1, 1, 1, 1])
        # load address of the array into register a0
        t.input_array("a0", array0)
        # set a1 to the length of the array
        t.input_scalar("a1", len(array0))
        # call the `argmax` function
        t.call("argmax")
        # check that the register a0 contains the correct output
        t.check_scalar("a0", 0)
        # generate the `assembly/TestArgmax_test_simple.s` file and run it through venus
        t.execute()

    def test_error(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([1, 2, 3, 4, 5])
        t.input_array("a0", array0)
        t.input_scalar("a1", 0)
        t.call("argmax")
        t.execute(120)

    @classmethod
    def tearDownClass(cls):
        print_coverage("argmax.s", verbose=False)


class TestDot(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        v0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        v1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load array addresses into argument registers
        t.input_array("a0", v0)
        t.input_array("a1", v1)
        # load array attributes into argument registers
        t.input_scalar("a2", 9)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 1)
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.check_scalar("a0", 285)
        t.execute()

    def test_stride(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        v0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        v1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load array addresses into argument registers
        t.input_array("a0", v0)
        t.input_array("a1", v1)
        # load array attributes into argument registers
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 2)
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.check_scalar("a0", 22)
        t.execute()

    def test_error1(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        v0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        v1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load array addresses into argument registers
        t.input_array("a0", v0)
        t.input_array("a1", v1)
        # load array attributes into argument registers
        t.input_scalar("a2", 0)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 1)
        # call the `dot` function
        t.call("dot")
        t.execute(123)

    def test_error2(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        v0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        v1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load array addresses into argument registers
        t.input_array("a0", v0)
        t.input_array("a1", v1)
        # load array attributes into argument registers
        t.input_scalar("a2", 9)
        t.input_scalar("a3", 0)
        t.input_scalar("a4", 1)
        # call the `dot` function
        t.call("dot")
        t.execute(124)

    def test_error3(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        v0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        v1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        # load array addresses into argument registers
        t.input_array("a0", v0)
        t.input_array("a1", v1)
        # load array attributes into argument registers
        t.input_scalar("a2", 9)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 0)
        # call the `dot` function
        t.call("dot")
        t.execute(124)

    @classmethod
    def tearDownClass(cls):
        print_coverage("dot.s", verbose=False)


class TestMatmul(TestCase):

    def do_matmul(self, m0, m0_rows, m0_cols, m1, m1_rows, m1_cols, result, code=0):
        t = AssemblyTest(self, "matmul.s")
        # we need to include (aka import) the dot.s file since it is used by matmul.s
        t.include("dot.s")

        # create arrays for the arguments and to store the result
        array0 = t.array(m0)
        array1 = t.array(m1)
        array_out = t.array([0] * len(result))

        # load address of input matrices and set their dimensions
        t.input_array("a0", array0)
        t.input_scalar("a1", m0_rows)
        t.input_scalar("a2", m0_cols)
        t.input_array("a3", array1)
        t.input_scalar("a4", m1_rows)
        t.input_scalar("a5", m1_cols)
        # load address of output array
        t.input_array("a6", array_out)
        # call the matmul function
        t.call("matmul")

        # check the content of the output array
        t.check_array(array_out, result)

        # generate the assembly file and run it through venus, we expect the simulation to exit with code `code`
        t.execute(code=code)

    def test_simple(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150]
        )

    def test_more(self):
        self.do_matmul(
            [0, 2, 3, 5, 3, 5, 6, 9, 9, 5, 4, 0, 9, 9, 9], 5, 3,
            [4, 7, 2, 3, 1, 2, 2, 4, 7, 2, 8, 9, 6, 2, 3, 8, 2, 5], 3, 6,
            [22, 14, 23, 28, 22, 33, 56, 57, 46, 61, 39, 62, 96, 96, 102, 108, 96, 138, 28, 51, 38, 23, 37, 46, 108,
             117, 108, 117, 99, 144]
        )

    def test_thin(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 1, 9,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 9, 1,
            [285]
        )

    def test_error1_1(self):
        self.do_matmul(
            [0, 2, 3, 5, 3, 5, 6, 9, 9, 5, 4, 0, 9, 9, 9], 0, 3,
            [4, 7, 2, 3, 1, 2, 2, 4, 7, 2, 8, 9, 6, 2, 3, 8, 2, 5], 3, 6,
            [22, 14, 23, 28, 22, 33, 56, 57, 46, 61, 39, 62, 96, 96, 102, 108, 96, 138, 28, 51, 38, 23, 37, 46, 108,
             117, 108, 117, 99, 144], 125
        )

    def test_error1_2(self):
        self.do_matmul(
            [0, 2, 3, 5, 3, 5, 6, 9, 9, 5, 4, 0, 9, 9, 9], 5, 0,
            [4, 7, 2, 3, 1, 2, 2, 4, 7, 2, 8, 9, 6, 2, 3, 8, 2, 5], 3, 6,
            [22, 14, 23, 28, 22, 33, 56, 57, 46, 61, 39, 62, 96, 96, 102, 108, 96, 138, 28, 51, 38, 23, 37, 46, 108,
             117, 108, 117, 99, 144], 125
        )

    def test_error2_1(self):
        self.do_matmul(
            [0, 2, 3, 5, 3, 5, 6, 9, 9, 5, 4, 0, 9, 9, 9], 5, 3,
            [4, 7, 2, 3, 1, 2, 2, 4, 7, 2, 8, 9, 6, 2, 3, 8, 2, 5], 0, 6,
            [22, 14, 23, 28, 22, 33, 56, 57, 46, 61, 39, 62, 96, 96, 102, 108, 96, 138, 28, 51, 38, 23, 37, 46, 108,
             117, 108, 117, 99, 144], 126
        )

    def test_error2_2(self):
        self.do_matmul(
            [0, 2, 3, 5, 3, 5, 6, 9, 9, 5, 4, 0, 9, 9, 9], 5, 3,
            [4, 7, 2, 3, 1, 2, 2, 4, 7, 2, 8, 9, 6, 2, 3, 8, 2, 5], 3, 0,
            [22, 14, 23, 28, 22, 33, 56, 57, 46, 61, 39, 62, 96, 96, 102, 108, 96, 138, 28, 51, 38, 23, 37, 46, 108,
             117, 108, 117, 99, 144], 126
        )

    def test_error3(self):
        self.do_matmul(
            [0, 2, 3, 5, 3, 5, 6, 9, 9, 5, 4, 0, 9, 9, 9], 5, 3,
            [4, 7, 2, 3, 1, 2, 2, 4, 7, 2, 8, 9, 6, 2, 3, 8, 2, 5], 4, 6,
            [22, 14, 23, 28, 22, 33, 56, 57, 46, 61, 39, 62, 96, 96, 102, 108, 96, 138, 28, 51, 38, 23, 37, 46, 108,
             117, 108, 117, 99, 144], 127
        )

    @classmethod
    def tearDownClass(cls):
        print_coverage("matmul.s", verbose=False)


class TestReadMatrix(TestCase):

    def do_read_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        # load address to the name of the input file into register a0
        t.input_read_filename("a0", "inputs/test_read_matrix/test_input.bin")

        # allocate space to hold the rows and cols output parameters
        rows = t.array([-1])
        cols = t.array([-1])

        # load the addresses to the output parameters into the argument registers
        t.input_array("a1", rows)
        t.input_array("a2", cols)

        # call the read_matrix function
        t.call("read_matrix")

        # check the output from the function
        t.check_array_pointer("a0", [1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.check_array(rows, [3])
        t.check_array(cols, [3])

        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)

    def test_simple(self):
        self.do_read_matrix()

    def test_malloc_error(self):
        self.do_read_matrix(fail = 'malloc', code = 116)
    def test_fopen_error(self):
        self.do_read_matrix(fail = 'fopen', code = 117)
    def test_fread_error(self):
        self.do_read_matrix(fail = 'fread', code = 118)
    def test_fclose_error(self):
        self.do_read_matrix(fail = 'fclose', code = 119)

    def do_more(self, fail = '', code = 0):
        t = AssemblyTest(self, "read_matrix.s")
        # load address to the name of the input file into register a0
        t.input_read_filename("a0", "inputs/test_read_matrix/test1.bin")
        # allocate space to hold the rows and cols output parameters
        rows = t.array([-1])
        cols = t.array([-1])

        # load the addresses to the output parameters into the argument registers
        t.input_array("a1", rows)
        t.input_array("a2", cols)

        # call the read_matrix function
        t.call("read_matrix")

        # check the output from the function
        t.check_array(rows, [3])
        t.check_array(cols, [4])
        t.check_array_pointer("a0", [-1, -2, -3, -4, 5, 6, 7, 8, 9, 10, 11, -12])

        t.execute(fail=fail, code=code)

    def test_more(self):
        self.do_more()

    @classmethod
    def tearDownClass(cls):
        print_coverage("read_matrix.s", verbose=False)


class TestWriteMatrix(TestCase):

    def do_write_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "write_matrix.s")
        outfile = "outputs/test_write_matrix/student.bin"
        # load output file name into a0 register
        t.input_write_filename("a0", outfile)
        # load input array and other arguments
        t.input_array("a1", t.array([1, 2, 3, 4, 5, 6, 7, 8, 9]))
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 3)
        # call `write_matrix` function
        t.call("write_matrix")
        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
        # compare the output file against the reference
        if code == 0:
            t.check_file_output(outfile, "outputs/test_write_matrix/reference.bin")

    def test_simple(self):
        self.do_write_matrix()

    def test_fopen_error(self):
        self.do_write_matrix(fail = 'fopen', code = 112)
    def test_fwrite_error(self):
        self.do_write_matrix(fail = 'fwrite', code = 113)
    def test_fclose_error(self):
        self.do_write_matrix(fail = 'fclose', code = 114)

    def do_more(self, fail='', code=0):
        t = AssemblyTest(self, "write_matrix.s")
        outfile = "outputs/test_write_matrix/student2.bin"
        # load output file name into a0 register
        t.input_write_filename("a0", outfile)
        # load input array and other arguments
        t.input_array("a1", t.array([-1, -2, -3, -4, 5, 6, 7, 8, 9, 10, 11, -12]))
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 4)
        # call `write_matrix` function
        t.call("write_matrix")
        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
        # compare the output file against the reference
        if code == 0:
            t.check_file_output(outfile, "outputs/test_write_matrix/test1.bin")

    def test_more(self):
        self.do_more()

    @classmethod
    def tearDownClass(cls):
        print_coverage("write_matrix.s", verbose=False)


class TestClassify(TestCase):

    def make_test(self):
        t = AssemblyTest(self, "classify.s")
        t.include("argmax.s")
        t.include("dot.s")
        t.include("matmul.s")
        t.include("read_matrix.s")
        t.include("relu.s")
        t.include("write_matrix.s")
        return t

    def test_simple0_input0(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]
        t.input_scalar("a2", 0)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args)

        # compare the output file and
        t.check_scalar("a0", 2)
        t.check_file_output(out_file, ref_file)

        # compare the classification output with `check_stdout`
        t.check_stdout("2")

    def test_simple0_input0_1(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]
        t.input_scalar("a2", 1)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args)

        # compare the output file and
        t.check_scalar("a0", 2)
        t.check_file_output(out_file, ref_file)

    def test_malloc_error(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]

        t.input_scalar("a2", 1)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, fail = "malloc", code = 122)

    def test_arg_error(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin"]

        t.input_scalar("a2", 1)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, code=121)

    @classmethod
    def tearDownClass(cls):
        print_coverage("classify.s", verbose=False)


# The following are some simple sanity checks:
import subprocess, pathlib, os
script_dir = pathlib.Path(os.path.dirname(__file__)).resolve()

def compare_files(test, actual, expected):
    assert os.path.isfile(expected), f"Reference file {expected} does not exist!"
    test.assertTrue(os.path.isfile(actual), f"It seems like the program never created the output file {actual}!")
    # open and compare the files
    with open(actual, 'rb') as a:
        actual_bin = a.read()
    with open(expected, 'rb') as e:
        expected_bin = e.read()
    test.assertEqual(actual_bin, expected_bin, f"Bytes of {actual} and {expected} did not match!")

class TestMain(TestCase):
    """ This sanity check executes src/main.S using venus and verifies the stdout and the file that is generated.
    """

    def run_main(self, inputs, output_id, label):
        # args = ["src/main.S", f"{inputs}/m0.bin", f"{inputs}/m1.bin",
        #         f"{inputs}/inputs/input0.bin",
        #         f"outputs/test_basic_main/student{output_id}.bin"]
        args = [f"{inputs}/m0.bin", f"{inputs}/m1.bin",
                f"{inputs}/inputs/input0.bin",
                f"outputs/test_basic_main/student{output_id}.bin"]
        reference = f"outputs/test_basic_main/reference{output_id}.bin"

        t= AssemblyTest(self, "main.s", no_utils=True)
        """t.include("classify.s")"""
        t.call("main")
        t.execute(args=args, verbose=False)
        t.check_stdout(label)
        t.check_file_output(args[-1], reference)

    def test0(self):
        self.run_main("inputs/simple0/bin", "0", "2")

    def test1(self):
        self.run_main("inputs/simple1/bin", "1", "1")

class TestHard(TestCase):
    def run_mnist(self, inputs, output_id, label):
        args = [f"{inputs}/m0.bin", f"{inputs}/m1.bin",
                f"{inputs}/inputs/mnist_input{output_id}.bin",
                f"outputs/test_hard/mnist_output{output_id}.bin"]
        t = AssemblyTest(self, "main.s", no_utils=True)
        t.call("main")
        t.execute(args=args, verbose=False)
        t.check_stdout(label)

    def test_0(self):
        self.run_mnist("inputs/mnist/bin", "0", "6")

    def test_1(self):
        self.run_mnist("inputs/mnist/bin", "1", "9")

    def test_2(self):
        self.run_mnist("inputs/mnist/bin", "2", "9")

    def test_3(self):
        self.run_mnist("inputs/mnist/bin", "3", "2")

    def test_4(self):
        self.run_mnist("inputs/mnist/bin", "4", "9")

    def test_5(self):
        self.run_mnist("inputs/mnist/bin", "5", "4")

    def test_6(self):
        self.run_mnist("inputs/mnist/bin", "6", "4")

    def test_7(self):
        self.run_mnist("inputs/mnist/bin", "7", "8")

    def test_8(self):
        self.run_mnist("inputs/mnist/bin", "8", "7")