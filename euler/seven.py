#!/usr/bin/env python3

import math

def is_prime(num):
    for i in range(2, num): # since range is 1 less than num specified
        if num % i == 0:
            return False
    return True

def new_is_prime(n):
    if n % 2 == 0 and n > 2:
        return False
    return all(n % i for i in range(3, int(math.sqrt(n)) + 1, 2))


prime_count = 1
num = 1

while prime_count != 10002:
    num += 1
    if is_prime(num):
        prime_count += 1

print("prime_count:", prime_count)
print("10001st prime number:", num)