
def is_prime(num):
    """ Assumes num > 3 """
    if num % 2 == 0:
        return False
    for p in range(3, int(num**0.5)+1, 2):  # Jumps of 2 to skip odd numbers
        if num % p == 0:
            return False
    return True

max = 2000000 + 1
sum = 2 + 3

for i in range(4, max):
    if is_prime(i):
        sum += i

print(sum)
