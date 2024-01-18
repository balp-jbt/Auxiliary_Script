from z3 import *


def inner_QF_BV():
    print("___inner_QF_BV_test___")
    x = BitVec('x',8)               # Init x in python and z3 as a 8-bit var. 
    y = BitVec('y',8)
    z = BitVec('z',8)
    # x, y, z = BitVecs('x y z', 8) Or use this

    solver = Solver()                # Init a Solver class
    solver.add(x ^ y & z == 12)      # Add constraints
    solver.add(y & z >> 3 == 3)
    solver.add(z ^ y == 4)

    check = solver.check()           # check if the model is sat or unsat
    print(check)

    model = solver.model()           # get a ModelRef class to get result
    print(model)
    # sat
    # [z = 27, y = 31, x = 23]

def solve_smt2_file(file_path):
    print("___Load_File_QF_BV_test___")
    ctx = Context()
    solver = Solver(ctx=ctx)
    
    # Load SMT2 language from file
    with open(file_path, 'r') as file:
        smt_content = file.read()
        solver.from_string(smt_content)

    result = solver.check()
    print(result)
    model = solver.model
    print(model)

if __name__ == "__main__":
    print("___Z3-Python-Demo___")
    inner_QF_BV()
    solve_smt2_file("./source/QF_BV_test.smt2")