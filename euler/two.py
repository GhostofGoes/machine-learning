#!/usr/bin/env python3

prev1 = 0 # last term calculated
prev2 = 1 # term calculated before last term
sum = 0
curr = 0
go = True

while curr <= 4000000:
    curr = prev1 + prev2
    prev2 = prev1
    prev1 = curr
    if curr % 2 == 0:
        sum += curr

print("Final sum: ", sum)
# works

