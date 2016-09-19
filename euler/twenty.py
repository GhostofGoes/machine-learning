#!/usr/bin/env python3


def factorial(n):
    temp = 1
    for i in range(n, 1, -1):
        temp *= i
    return temp

print("Factorial(10):", factorial(10))
num = factorial(100)
print("Factorial(100):", num)
sum_digits = 0

for i in str(num):
    sum_digits += int(i)

print("Sum of digits:", sum_digits)
