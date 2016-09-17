with open("eight.txt", "r") as file:
    num = file.read()

adjacent_digits = 13
greatest = 0

for i in range( 0, (len(num) - adjacent_digits) ):
    product = 1
    for j in range(i, i + adjacent_digits):
        product *= int(num[j])
    if product > greatest:
        greatest = product

print(greatest)
