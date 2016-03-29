#!/usr/bin/env python3

sum_of_squares = 0
sum = 0
square_of_the_sum = 0

for i in range(1, 101):
    sum_of_squares += i**2
    sum += i
    print("i", i)

square_of_the_sum = sum ** 2

print("Sum of the squares:", sum_of_squares)
print("Sum:", sum)
print("Square of the sum:", square_of_the_sum)

print("Difference between", sum_of_squares, "and", square_of_the_sum, "is", abs(sum_of_squares - square_of_the_sum))