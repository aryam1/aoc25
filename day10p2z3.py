#import z3
from pprint import pprint as pp
from z3 import *
from concurrent.futures import ThreadPoolExecutor

jolts = []
steps = []

with open("d10key.txt") as f:
    row = [r.split() for r in f.readlines()]
    for r in row:
        steps.append([list(map(int,step[1:-1].split(','))) for step in r[1:-1]])
        jolts.append(list(map(int,r[-1][1:-1].split(','))))

def solve_system(target, steps):
    """
    target : list of integers, length d
    steps  : list of step vectors (each a list of length d)
    
    Returns: dict of step -> coefficient, or None if no solution
    """

    n_steps = len(steps)
    # Create Z3 integer variables for each step coefficient
    coeffs = [Int(f'n{i}') for i in range(n_steps)]

    # Create Z3 solver / optimizer
    opt = Optimize()

    # Constraint: all coefficients >= 0
    for c in coeffs:
        opt.add(c >= 0)

    # Constraint: sum of step vectors times their coefficient equals target
    d = len(target)
    for i in range(d):
        expr = Sum([coeffs[j] * steps[j][i] for j in range(n_steps)])
        opt.add(expr == target[i])

    # Optional: minimize total number of steps
    opt.minimize(Sum(coeffs))

    if opt.check() == sat:
        model = opt.model()
        solution = [model[coeffs[i]].as_long() for i in range(n_steps)]
        return solution
    else:
        return None

def pad_step(step, target_len):
    vec = [0] * target_len
    for pos in step:
        vec[pos] += 1
    return vec

fin = 0
for i in range(len(jolts)):
    # Convert steps from something like (0,1,2,4) to [1,1,1,0,1,0] for a 5 long jolts
    padded_steps = [pad_step(step, len(jolts[i])) for step in steps[i]]
    # Solve with z3 for coefficients
    solved = solve_system(jolts[i],padded_steps)
    fin += sum(solved)
print(f"Part 2 Total: {fin}")
