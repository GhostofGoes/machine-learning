#!/usr/bin/env python3


def find_abc():
    for a in range(1, 500):
        for b in range(a + 1, 500):
            for c in range(b + 1, 500):
                if(a ** 2 + b ** 2 == c ** 2) and (a + b + c == 1000):
                    return a, b, c
r = find_abc()
print(r[0] * r[1] * r[2])
