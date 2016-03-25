#!/usr/bin/env python3

import fileinput
import math


# From page 251 in teh book
def calc_entropy(p):
    if p != 0:
        return -p * math.log2(p)
    else:
        return 0


# Based on code from page 253 in the book
def calc_info_gain(feature, features, examples, example_answers ):
    gain = 0
    entropy = 0

    for ans in example_answers:
        entropy += calc_entropy(float(example_answers.count(ans))/len(example_answers))


    for f in features[feature]:
        for example in examples:
            pass


    return entropy - gain


def build_tree():
    pass


def print_tree(tree):
    if tree is None:
        pass
    else:
        print(tree)


# Globals make naming a pain
def main():
    input_debugging = True
    labels = []
    features = []
    answers = []
    examples = []
    example_answers = []


    for line in fileinput.input():
        linenum = fileinput.lineno()
        if linenum == 1:
            num_features = int(line)
        elif linenum <= num_features + 1:
            temp = line.split()
            labels.append(temp[0])
            features.append(temp[1:])
        elif linenum == num_features + 2:
            answers = line.split()
        else:
            temp = line.split()
            examples.append(temp[:-1])
            example_answers.append((temp[-1]))

    if input_debugging:
        print("Input filename: " + fileinput.filename())
        print("num_features: " + str(num_features))
        for label, feature in zip(labels, features):
            print('{:15}'.format(label + ': '), end="", flush=True)
            print(feature)

        print('{:15}'.format("\nPossible Answers: ") + str(answers))
        for example, ans in zip(examples, example_answers):
            print('{:15}'.format(ans + ': '), end="", flush=True)
            print(example)


main()
