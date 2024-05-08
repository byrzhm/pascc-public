"""
    This module provides some helper functions for the tests
"""
import subprocess
import os


def strip_c_line_comments(code: str) -> str:
    """
    strip c line comments from the code

    TODO: handle comments in strings
    """
    code = code.split("\n")
    new_code = ""
    for line in code:
        if "//" in line:
            line = line.split("//")[0]
        new_code += line + "\n"
    return new_code


def strip_c_block_comments(code: str) -> str:
    """
    strip c block comments from the code

    TODO: handle comments in strings
    """
    code = code.split("\n")
    new_code = ""
    in_comment = False
    for line in code:
        if "/*" in line:
            in_comment = True
            line = line.split("/*")[0]
        if "*/" in line:
            in_comment = False
            line = line.split("*/")[1]
        if not in_comment:
            new_code += line + "\n"
    return new_code


def strip_c_comments(code: str) -> str:
    """
    strip c comments from the code
    """
    code = strip_c_line_comments(code)
    code = strip_c_block_comments(code)
    return code


def compare_code(actual_output: str, expect_output: str) -> bool:
    """
    Compare two strings of code, ignoring whitespace and newlines
    """
    actual_output = actual_output.replace(" ", "")
    actual_output = actual_output.replace("\n", "")
    actual_output = actual_output.replace("\t", "")
    actual_output = strip_c_comments(actual_output)
    expect_output = expect_output.replace(" ", "")
    expect_output = expect_output.replace("\n", "")
    expect_output = expect_output.replace("\t", "")

    return actual_output == expect_output


def get_all_files(path: str) -> list:
    """Get all files in a directory

    Args:
        path (str): the path to the directory

    Returns:
        list: the list of files in the directory
    """
    files = []
    for file in os.listdir(path):
        if os.path.isfile(os.path.join(path, file)):
            files.append(file)
    return files


def run_pascc(pascc_path: str, input_path: str, output_path: str):
    """_summary_

    Args:
        pascc_path (str): _description_
        input_path (str): _description_
        output_path (str): _description_

    Returns:
        _type_: _description_
    """
    process = subprocess.Popen(
        [pascc_path, '-i', input_path, '-o', output_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    process.stdin.write(input_path)
    process.stdin.close()
    process.wait()
    return process.stdout.read()


def run_cc(c_compiler: str, input_path: str):
    """_summary_

    Args:
        c_compiler (str): _description_
        input_path (str): _description_
    """
    with open('test.txt', '+a', encoding='UTF-8') as f:
        f.write(c_compiler + ' ' + input_path)
