with open("thirteen.txt", "r") as file:
    numbers = file.readlines()
print(str(sum(int(num) for num in numbers))[:10])
