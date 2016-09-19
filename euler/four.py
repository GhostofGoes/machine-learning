#!/usr/bin/env python3


def check_palindrome(num):
    num = str(num)
    for i in range(0, int((len(num) + 1) / 2)):
        if num[i] != num[-i]:
            return False
    return True


def simple_palindrome(num):
    num = str(num)
    return num == num[::-1]

biggest = 0
for i in range(100, 999):
    for j in range(100, 999):
        temp = i * j
        if temp > biggest:
            if simple_palindrome(temp):
                biggest = temp

print(biggest)
