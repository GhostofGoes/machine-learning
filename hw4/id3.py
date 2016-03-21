#!/usr/bin/env python3

import fileinput
import math

def calc_entropy(p):
    if p != 0:
        return -p * math.log2(p)
    else:
        return 0

input_debugging = True
features = []
answers = []

for line in fileinput.input():
    linenum = fileinput.lineno()
    if linenum == 1:
        num_features = int(line)
    elif linenum <= num_features + 1:
        features.append(line.split())
    elif linenum == num_features + 2:
        possible_answers = line.split()
    else:
        answers.append(line.split())

if input_debugging:
    print("num_features: " + str(num_features))
    for feature in features:
        print(feature)

    print("possible_answers: " + str(possible_answers))
    for answer in answers:
        print(answer)

