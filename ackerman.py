#!/usr/bin/python3

import os

import numpy as np
from scipy.optimize import minimize


w = 1700
l = 2500


def diff(x):
    out = os.popen("./a -w {} -h {} -x {} -y {}".format(w, l, x[0], x[1])).read()
    res = float(out)
    print("x y", x[0], x[1], "res", res)
    return res


x0 = np.array([0, -100])

res = minimize(diff, x0, method="nelder-mead", options={"xtol": 1, "disp": True})

print("res", res)

# print("rot", rot10)
# print(Vec2(P2(w2, 0), P2(-1, 0)).rot(rot10))
