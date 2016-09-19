#!/usr/bin/env python3

prev1 = 0  # last term calculated
prev2 = 1  # term calculated before last term
s = 0
curr = 0

while curr <= 4000000:
    curr = prev1 + prev2
    prev2 = prev1
    prev1 = curr
    if curr % 2 == 0:
        s += curr

print(s)
