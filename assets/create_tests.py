from random import randint

with open("test.txt", "w") as f:
    for i in range(10000):
        f.write(f'{randint(0, 10000)}\n')
