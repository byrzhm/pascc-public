"""
    This module tests some algorithms
"""

import unittest
import os
from pathlib import Path
import shutil
from test_helper import run, get_all_files, read_output_file


class TestPascc(unittest.TestCase):
    """Test the pascc compiler
    """

    # DON'T CHANGE THIS
    data_dir = os.path.join(os.getcwd(), '..', '..', 'test', 'data')
    pascc = os.path.join(os.getcwd(), 'pascc')
    tmp_output_dir = os.path.join(os.getcwd(), 'output')
    cc = 'gcc'

    @classmethod
    def setUpClass(self) -> None:
        shutil.rmtree(self.tmp_output_dir)
        Path(self.tmp_output_dir).mkdir(parents=True, exist_ok=True)

    def test_basic(self):
        """Test the basic functionality of the compiler
        """
        basic_path = os.path.join(self.data_dir, 'basic')
        input_dir_path = os.path.join(basic_path, 'input')
        output_dir_path = os.path.join(basic_path, 'output')
        basic_outputs = get_all_files(output_dir_path)
        basic_outputs = sorted(basic_outputs)

        for output_file in basic_outputs:
            root, _ = os.path.splitext(output_file)
            actual_output = run(pascc_path=self.pascc,
                                c_compiler=self.cc,
                                filename_noext=root,
                                input_dir_path=input_dir_path,
                                output_dir_path=output_dir_path)
            expected_output = read_output_file(os.path.join(
                output_dir_path, output_file
            ))
            self.assertEqual(actual_output,
                             expected_output,
                             f"{root} failed!")

    def test_algo(self):
        """Test some algorithms
        """
        pass

    def test_extended(self):
        """Test extended functionality of the compiler
        """
        pass


if __name__ == '__main__':
    unittest.main()
