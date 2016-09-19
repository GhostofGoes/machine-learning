#!/usr/bin/env python3

results = []
for a in range(2, 101):
    for b in range(2, 101):
        term = a ** b
        if term not in results:
            results.append(term)
print(len(results))
