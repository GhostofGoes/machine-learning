#!/usr/bin/env python3


def is_evenly_divisible(num):
    for i in range(2, 20):
        if num % i != 0:
            return False
    return True

answer = 40
while not is_evenly_divisible(answer):
    answer += 1

print(answer)
