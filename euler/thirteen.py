with open("thirteen.txt", "r") as file:
    numbers = file.read().splitlines()

sum = 0
for num in numbers:
    sum += int(num)

print(str(sum)[:10])
