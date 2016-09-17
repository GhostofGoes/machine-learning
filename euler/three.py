cut = num = 600851475143  # 13195
chopper = 2

while chopper < cut:
    if cut % chopper != 0:
        chopper += 1
    else:
        cut = cut // chopper
print(cut)
