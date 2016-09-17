
aLow = 2
aHigh = 100 + 1
bLow = 2
bHigh = 100 + 1

results = []
for a in range(aLow, aHigh):
    for b in range(bLow, bHigh):
        term = a ** b
        if term not in results:
            results.append(term)

print(results)
print(len(results))
