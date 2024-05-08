"""
    This module tests some algorithms
"""

import unittest
# import subprocess
import os
from test_helper import run_cc, get_all_files


class TestPascc(unittest.TestCase):
    """Test the pascc compiler
    """

    pascc_path = os.path.join(os.getcwd(), 'pascc')
    cc = 'gcc'

    def test_basic(self):
        """Test the basic functionality of the compiler
        """
        basic_path = os.path.join(os.getcwd(), 'data/basic')
        basic_outputs = get_all_files(os.path.join(basic_path, 'output'))
        basic_outputs = sorted(basic_outputs)

        for output_file in basic_outputs:
            root, _ = os.path.splitext(output_file)
            if os.path.isfile(os.path.join(basic_path, 'input', root + '.in')):
                print(root, ' has stdin')

    def test_algo(self):
        """Test some algorithms
        """
        algo_path = os.path.join(os.getcwd(), 'data/algo')
        algo_outputs = get_all_files(os.path.join(algo_path, 'output'))
        algo_outputs = sorted(algo_outputs)

        for output_file in algo_outputs:
            root, _ = os.path.splitext(output_file)
            if os.path.isfile(os.path.join(algo_path, 'input', root + '.in')):
                print(root, ' has stdin')
