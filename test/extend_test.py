import unittest
import subprocess
from test_helper import compare_code

class TestExtend(unittest.TestCase):
    pascc_path = "../app/pascc"

    def test_pass_by_ref(self):
        test_file = "right/grammar/pass_by_ref.pas"
        output_file = "right/grammar/pass_by_ref.c"
        
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
        void swap(int *x, int *y) {
            int temp;
            temp = *x;
            *x = *y;
            *y = temp;
        }
        int main() {
            a = 5;
            b = 10;
            printf("Before swapping: a = %d, b = %d\\n", a, b);
            swap(&a, &b);
            printf("After swapping: a = %d, b = %d\\n", a, b);
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))


    def test_loop_while(self):
        test_file = "right/grammar/loop_while.pas"
        output_file = "right/grammar/loop_while.c"
        
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
            i = 0;
            while (i < 10) {
                printf("%d\\n", i);
                i = i + 1;
            }
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))


    def test_format_output(self):
        test_file = "right/grammar/format_output.pas"
        output_file = "right/grammar/format_output.c"
        
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
        double pi;
        int main() {
            pi = 3.14159265358979323846;
            printf("pi = %.2f\\n", pi);
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))

    def test_record(self):
        test_file = "right/grammar/record.pas"
        output_file = "right/grammar/record.c"
        
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
        typedef struct _point {
            int x;
            int y;
        } point;
        point p;
        int main() {
            p.x = 10;
            p.y = 20;
            printf("%d %d\\n", p.x, p.y);
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))

    
    def test_record_as_param(self):
        test_file = "right/grammar/record_as_param.pas"
        output_file = "right/grammar/record_as_param.c"
        
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
        typedef struct _point {
            int x;
            int y;
        } point;
        point p;
        void printpoint(point p) {
            printf("%d %d\\n", p.x, p.y);
        }
        int main() {
            p.x = 10;
            p.y = 20;
            printpoint(p);
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))

    def test_record_as_var_param(self):
        test_file = "right/grammar/record_as_var_param.pas"
        output_file = "right/grammar/record_as_var_param.c"
        
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
        struct point {
            int x;
            int y;
        };
        point p;
        void transform(point *p) {
            p->x = -p->x;
            p->y = -p->y;
        }
        int main() {
            p.x = 10;
            p.y = 20;
            printf("Old Point: %d, %d\\n", p.x, p.y);
            transform(&p);
            printf("New Point: %d, %d\\n", p.x, p.y);
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))

    def test_record_as_return(self):
        test_file = "right/grammar/record_as_return.pas"
        output_file = "right/grammar/record_as_return.c"
        
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
        typedef struct _point {
            int x;
            int y;
        } point;
        point createpoint(int x, int y) {
            point p;
            p.x = x;
            p.y = y;
            return p;
        }
        int main() {
            p = createpoint(10, 20);
            printf("%d %d\\n", p.x, p.y);
        }
        """

        self.assertTrue(compare_code(output_code, expected_output_code))

if __name__ == "__main__":
    unittest.main()