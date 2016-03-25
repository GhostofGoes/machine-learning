#!/usr/bin/env python3

import fileinput
import math

testing = True


# From page 251 in teh book
def calc_entropy(p):
    if p != 0:
        return -p * math.log2(p)
    else:
        return 0


# Based on code from page 253 in the book
# Formula: Gain(S, F) = Entropy(S) - sum( len(Sf)/len(S) * Entropy(Sf)
def calc_info_gain(feature, values, examples, example_answers ):
    entropy1 = 0 # Entropy(S)
    entropy2 = 0 # sum( len(Sf) / len(S) * Entropy(Sf )

    # Calculate Entropy for the set of all answers
    for ans in example_answers:
        entropy1 += calc_entropy(float(example_answers.count(ans))/len(example_answers))

    # for each possible value of a given feature
    #    sum( prob. of that value appearing * entropy of each subset that has that value )
    for val in values[feature]:
        ents = 0
        temp = []

        for exp, ans in zip(examples, example_answers):
            if val in exp:  # data sets are small enough i can get away with this shit
                temp.append(ans)

        for exp in list(set(temp)):  # Calc entropy of subset by calc. for each possible answer
            ents += calc_entropy(float(temp.count(exp) / len(temp)))

        entropy2 += (len(temp) / len(examples)) * ents  # Add entropy of subset to sum

    return entropy1 - entropy2 # Calculate the information gain


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

    for i in range(0, len(features)):
        print("Calc_info_gain of ", i, " : ", calc_info_gain(i, features, examples, example_answers ))

main()
