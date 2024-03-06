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
        # FIXME: a = /* 5; */ 10; will break
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
