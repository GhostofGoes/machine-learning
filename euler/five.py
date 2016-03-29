#!/usr/bin/env python3

def is_evenly_divisible(num):
    for i in range(2, 20):
        if num % i != 0:
            return False
    return True

answer = 40
while is_evenly_divisible(answer) != True:
    answer += 1

print("Answer: ", answer)
# works