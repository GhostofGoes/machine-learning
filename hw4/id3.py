#!/usr/bin/env python3

import fileinput
import math

testing = False
input_debugging = False


# From page 251 in teh book
def calc_entropy(p):
    if p != 0:
        return -p * math.log2(p)
    else:
        return 0


# Based on code from pages 253 - 254 in the book
# Formula: Gain(S, F) = Entropy(S) - sum( len(Sf)/len(S) * Entropy(Sf)
def calc_info_gain(feature, values, examples, example_answers ):
    entropy1 = 0  # Entropy(S)
    entropy2 = 0  # sum( len(Sf) / len(S) * Entropy(Sf )

    # Calculate Entropy for the set of all answers
    for ans in list(set(example_answers)):
        entropy1 += calc_entropy(float(example_answers.count(ans) / len(example_answers)))

    # for each possible value of a given feature
    #    sum( prob. of that value appearing * entropy of each subset that has that value )
    for val in range(len(values[feature])):
        ents = 0
        temp = []

        for e in range(len(examples)):
            if examples[e][feature] == values[feature][val]:
                temp.append(example_answers[e])
        # So I was doing this like an idiot...
        # for exp, ans in zip(examples, example_answers):
        #    if val in exp:  # data sets are small enough i can get away with this shit
        #        temp.append(ans)

        for exp in list(set(temp)):  # Calc entropy of subset by calc. for each possible answer
            ents += calc_entropy(float(temp.count(exp) / len(temp)))
        entropy2 += (len(temp) / len(examples)) * ents  # Add entropy of subset to sum

    return entropy1 - entropy2 # Calculate the information gain


# Based on algorithm on pages 255-256 in the book
def make_tree(data, data_answers, features, labels):

    if not data: # No more data
        return None
    elif not features: # No more features, empty branch
        return max(set(data_answers), key=data_answers.count) # http://stackoverflow.com/a/1518632/2214380
    elif len(set(data_answers)) == 1: # One class remaining
        return set(data_answers).pop()
    else:
        gains = []

        # Choose best feature based on information gain
        for feature in range(len(features)):
            # TODO Need to handle continuous
            gains.append(calc_info_gain(feature, features, data, data_answers))
        best_feature = gains.index(max(gains))

        tree = {labels[best_feature]: {}}

        # Find possible feature values TODO continuous
        for feature in features[best_feature]:
            index = 0
            new_data = []
            new_answers = []
            new_features = []
            new_labels = []

            for datapoint in data:
                if datapoint[best_feature] == feature:
                    if best_feature == 0:
                        datapoint = datapoint[1:]
                        new_labels = labels[1:]
                        new_features = features[1:]
                    elif best_feature == len(features):
                        datapoint = datapoint[:-1]
                        new_labels = labels[:-1]
                        new_features = features[:-1]
                    else:  # Error in books code: datapoint is being overwritten before reuse. Thanks Keith!
                        new_datapoint = datapoint[:best_feature]
                        new_datapoint.extend(datapoint[best_feature+1:])
                        datapoint = new_datapoint
                        new_labels = labels[:best_feature]
                        new_labels.extend(labels[best_feature+1:])
                        new_features = features[:best_feature]
                        new_features.extend(features[best_feature+1:])

                    new_data.append(datapoint)
                    new_answers.append(data_answers[index])
                index += 1

            subtree = make_tree(new_data, new_answers, new_features, new_labels)
            tree[labels[best_feature]][feature] = subtree
        return tree


def print_tree(tree, depth=0):
    if tree is None:
        pass
    elif type(tree) == str or type(tree) == int:
        print("    " * depth, tree)
    else:
        for key in tree:
            for val in tree[key]:
                print("    " * depth, key, "=", val, ":")
                print_tree(tree[key][val], depth + 1)


def id3():
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
            temp = line.split()
            answers = temp[1:]
        else:
            temp = line.split()
            examples.append(temp[:-1])
            example_answers.append((temp[-1]))

    if input_debugging:
        print("Input filename:", fileinput.filename())
        print("num_features:", num_features)
        for label, feature in zip(labels, features):
            print('{:15}'.format(label + ': '), end="", flush=True)
            print(feature)

        print('\n', answers)
        for example, ans in zip(examples, example_answers):
            print('{:15}'.format(ans + ': '), end="", flush=True)
            print(example)

    if testing:
        for i in range(0, len(features)):
            print("calc_info_gain of", '{:10}'.format(labels[i]), ":", calc_info_gain(i, features, examples, example_answers ))

    tree = make_tree(examples, example_answers, features, labels)
    print_tree(tree)


# Execute Order 66
id3()
