"""
    This module provides some helper functions for the tests
"""
import subprocess
import os


def get_all_files(path: str) -> list:
    """Get all files in a directory

    Args:
        path (str): The path to the directory

    Returns:
        list: A list of all files in the directory
    """
    files = []
    for file in os.listdir(path):
        if os.path.isfile(os.path.join(path, file)):
            files.append(file)
    return files


def run_pascc(pascc_path: str,
              input_pas_path: str,
              output_c_path: str):
    """Run the pascc compiler

    Args:
        pascc_path (str): the path to the pascc compiler
        input_pas_path (str): the path to the input pascal file
        output_c_path (str): the path to the output c file
    """
    with subprocess.Popen(
        [pascc_path, '-i', input_pas_path, '-o', output_c_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    ) as process:
        stdout, _ = process.communicate()
        assert process.returncode == 0, \
            f'Compile {input_pas_path} failed!' + \
            f' pascc output: {stdout}'


def run_cc(c_compiler: str,
           input_c_path: str,
           output_exe_path: str):
    """Run the c compiler

    Args:
        c_compiler (str): the path to the c compiler
        input_c_path (str): the path to the input c file
        output_exe_path (str): the path to the output exe file
    """
    with subprocess.Popen(
        [c_compiler, # gcc-x86_64-linux-gnu 11.4.0 不支持 -fbracket-depth=N
         input_c_path, '-o', output_exe_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    ) as process:
        stdout, _ = process.communicate()
        assert process.returncode == 0, \
            f'Compile {input_c_path} failed!' + \
            f' c compiler output: {stdout}'


def run_exe(exe_path: str,
            user_input_path: str = None) -> str:
    """Run the executable

    Args:
        exe_path (str): the path to the executable
        user_input_path (str, optional): the user input file. Defaults to None.

    Returns:
        str: the output of the executable
    """
    if user_input_path is None:
        with subprocess.Popen(
            [exe_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True
        ) as process:
            stdout, _ = process.communicate()
            return_code = process.returncode
    else:
        with open(user_input_path, 'r', encoding='UTF-8') as f:
            with subprocess.Popen(
                [exe_path],
                stdin=f,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True
            ) as process:
                stdout, _ = process.communicate()
                return_code = process.returncode

    assert return_code == 0, \
        f'Execute {exe_path} failed!' + \
        f' exe output: {stdout}'

    return stdout


def run(pascc_path: str,
        c_compiler: str,
        filename_noext: str,
        input_dir_path: str,
        output_dir_path: str) -> str:
    """Run the pascc compiler, c compiler and the executable

    Args:
        pascc_path (str): the path to the pascc compiler
        c_compiler (str): the path to the c compiler
        filename_noext (str): the filename without extension
        input_dir_path (str): the path to the input directory
        output_dir_path (str): the path to the output directory

    Returns:
        str: the output of the executable
    """
    input_pas_path = os.path.join(input_dir_path, filename_noext + '.pas')
    output_c_path = os.path.join(os.getcwd(), 'output', filename_noext + '.c')
    output_exe_path = os.path.join(os.getcwd(), 'output', filename_noext)
    if os.path.isfile(os.path.join(input_dir_path, filename_noext + '.in')):
        user_input_path = os.path.join(input_dir_path, filename_noext + '.in')
    else:
        user_input_path = None

    run_pascc(pascc_path, input_pas_path, output_c_path)  # pas -> c
    run_cc(c_compiler, output_c_path, output_exe_path)    # c -> exe
    return run_exe(output_exe_path, user_input_path)      # exe output


def read_output_file(output_file: str) -> str:
    """Read the output file

    Args:
        output_file (str): the path to the output file

    Returns:
        str: the content of the output file
    """
    with open(output_file, 'r', encoding='UTF-8') as f:
        return f.read()
