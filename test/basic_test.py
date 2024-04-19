import unittest
import subprocess
from test_helper import compare_code

class TestBasic(unittest.TestCase):
    pascc_path = "../app/pascc"

    def test_function(self):
        test_file = "right/grammar/function.pas"
        output_file = "right/grammar/function.c"
        
        with open(test_file, "r") as file:
            test_code = file.read()

        compile_result = subprocess.run([self.pascc_path, "-i", test_code], capture_output=True, text=True)

        if compile_result.returncode != 0:
            print(compile_result.stderr)
            self.fail("pascc failed to compile the file")

        with open(output_file, "w") as file:
            output_code = file.read()

        expected_output_code = """
        #include <stdio.h>
        int a;
        int b;
        int max(int num1, int num2) {
            int __max;
            if (num1 > num2)
                __max = num1;
            else
                __max = num2;
            return __max;
        }
        int main() {
            a = 100;
            b = 200;
            printf("Max value is : %d\\n", max(a, b));
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))


    def test_pass_by_val(self):
        test_file = "right/grammar/pass_by_val.pas"
        output_file = "right/grammar/pass_by_val.c"
        
        with open(test_file, "r") as file:
            test_code = file.read()

        compile_result = subprocess.run([self.pascc_path, "-i", test_code], capture_output=True, text=True)

        if compile_result.returncode != 0:
            print(compile_result.stderr)
            self.fail("pascc failed to compile the file")

        with open(output_file, "w") as file:
            output_code = file.read()

        expected_output_code = """
        #include <stdio.h>
        int a;
        int b;
        void swap(int x, int y) {
            int temp;
            temp = x;
            x = y;
            y = temp;
        }
        int main() {
            a = 5;
            b = 10;
            printf("Before swapping: a = %d, b = %d\\n", a, b);
            swap(a, b);
            printf("After swapping: a = %d, b = %d\\n", a, b);
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))


    def test_loop_for(self):
        test_file = "right/grammar/loop_for.pas"
        output_file = "right/grammar/loop_for.c"
        
        with open(test_file, "r") as file:
            test_code = file.read()

        compile_result = subprocess.run([self.pascc_path, "-i", test_code], capture_output=True, text=True)

        if compile_result.returncode != 0:
            print(compile_result.stderr)
            self.fail("pascc failed to compile the file")

        with open(output_file, "w") as file:
            output_code = file.read()

        expected_output_code = """
        #include <stdio.h>
        int i;
        int main() {
            for (i = 1; i <= 10; i++) {
                printf("%d\\n", i);
            }
            for (i = 10; i >= 1; i--) {
                printf("%d\\n", i);
            }
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))


    def test_if(self):
        test_file = "right/grammar/if.pas"
        output_file = "right/grammar/if.c"
        
        with open(test_file, "r") as file:
            test_code = file.read()

        compile_result = subprocess.run([self.pascc_path, "-i", test_code], capture_output=True, text=True)

        if compile_result.returncode != 0:
            print(compile_result.stderr)
            self.fail("pascc failed to compile the file")

        with open(output_file, "w") as file:
            output_code = file.read()

        expected_output_code = """
        #include <stdio.h>
        int x;
        int main() {
            scanf("%d", &x);
            if (x > 5)
                printf("x is greater than 5\\n");
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))

    def test_if_else(self):
        test_file = "right/grammar/if_else.pas"
        output_file = "right/grammar/if_else.c"
        
        with open(test_file, "r") as file:
            test_code = file.read()

        compile_result = subprocess.run([self.pascc_path, "-i", test_code], capture_output=True, text=True)

        if compile_result.returncode != 0:
            print(compile_result.stderr)
            self.fail("pascc failed to compile the file")

        with open(output_file, "w") as file:
            output_code = file.read()

        expected_output_code = """
        #include <stdio.h>
        int x;
        int main() {
            scanf("%d", &x);
            if (x > 5)
                printf("x is greater than 5\\n");
            else
                printf("x is less than or equal to 5\\n");
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))

    def test_if_else_if(self):
        test_file = "right/grammar/if_else_if.pas"
        output_file = "right/grammar/if_else_if.c"
        
        with open(test_file, "r") as file:
            test_code = file.read()

        compile_result = subprocess.run([self.pascc_path, "-i", test_code], capture_output=True, text=True)

        if compile_result.returncode != 0:
            print(compile_result.stderr)
            self.fail("pascc failed to compile the file")

        with open(output_file, "w") as file:
            output_code = file.read()

        expected_output_code = """
        #include <stdio.h>
        int x;
        int main() {
            scanf("%d", &x);
            if (x < 0)
                printf("x is negative\\n");
            else if (x == 0)
                printf("x is zero\\n");
            else
                printf("x is positive\\n");
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))


if __name__ == '__main__':
    unittest.main()