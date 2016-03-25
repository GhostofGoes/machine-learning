#!/usr/bin/env python3

terms = [1,2]
sum = 0
while !(4000000 in terms):
	terms.append(terms[-2] + terms[-1])
	if terms[-1] % 2 == 0:
		sum += terms[-1]

print(sum)
