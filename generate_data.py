import random

path = './data/data.txt'
line_lenght = 1000
line_count = 1000

with open(path, 'w') as f:
    for i in range(line_count):
        line = ''.join(random.choice('ab') for _ in range(line_lenght))
        f.write(line + '\n')