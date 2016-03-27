#!/usr/bin/env python3

import fileinput
import math

testing = True
input_debugging = True


# From page 251 in teh book
def calc_entropy(p):
    if p != 0:
        return -p * math.log2(p)
    else:
        return 0


# Based on code from pages 253 - 254 in the book
# Formula: Gain(S, F) = Entropy(S) - sum( len(Sf)/len(S) * Entropy(Sf)
def calc_info_gain(feature, values, examples, example_answers ):
    entropy_ans = 0 # Entropy(S)
    entropy2 = 0    # sum( len(Sf) / len(S) * Entropy(Sf )
    ents = 0        # Used for calculating Entropy(Sf)
    temp = []       # Temporary array (wow!)

    # Calculate Entropy for the set of all answers
    for ans in list(set(example_answers)):
        entropy_ans += calc_entropy(float(example_answers.count(ans) / len(example_answers)))

    # for each possible value of a given feature
    #    sum( prob. of that value appearing * entropy of each subset that has that value )
    for val in range(len(values[feature])):
        for e in range(len(examples)):
            if examples[e][feature] == values[feature][val]:
                temp.append(example_answers[e])

        for exp in list(set(temp)):  # Calc entropy of subset by calc. for each possible answer
            ents += calc_entropy(float(temp.count(exp) / len(temp)))
        entropy2 += (len(temp) / len(examples)) * ents  # Add entropy of subset to sum

    return entropy_ans - entropy2 # Calculate the information gain


# Calculates the information gain for continuous values
def calc_continuous_info_gain(feature, features, data, data_answers):
    entropy_ans = 0 # Entropy(S)
    ents_less = 0
    ents_more = 0
    values = []
    temp_less = []
    temp_more = []
    gains = []

    # Calculate entropy for all the answers (same as in the normal info gain function)
    for ans in list(set(data_answers)):
        entropy_ans += calc_entropy(float(data_answers.count(ans) / len(data_answers)))

    # Get all the continuous values
    for val in range(len(data)):
        values.append(data[val][feature])

    for val in values:
        index = 0
        for i in values:
            if i <= val:
                temp_less.append(i)
                temp_less.append(data_answers[index])
            elif i > val:
                temp_more.append(i)
                temp_more.append(data_answers[index])
            index += 1

        for l in list(set(temp_less)):
            ents_less += calc_entropy(float(temp_less.count(l) / len(temp_less)))
        gains.append((len(temp_less) / len(data)) * ents_less)

        for m in list(set(temp_more)):
            ents_more += calc_entropy(float(temp_more.count(m) / len(temp_more)))
        gains[-1] += (len(temp_more) / len(data)) * ents_more

    return max(gains), values[gains.index(max(gains))] # Gain, Value we selected


# Based on algorithm on pages 255-256 in the book
def make_tree(data, data_answers, features, labels):

    if not data:        # No more data
        return None
    elif not features:  # No more features, empty branch
        return max(set(data_answers), key=data_answers.count) # http://stackoverflow.com/a/1518632/2214380
    elif len(set(data_answers)) == 1: # One class remaining
        return set(data_answers).pop()
    else:
        gains = []
        cont_val = 0

        # Choose best feature based on information gain
        for feature in range(len(features)):
            if "continuous" in features[feature]:
                temp, cont_val = calc_continuous_info_gain(feature, features, data, data_answers)
                print("temp:", temp)
                print("cont_val:", cont_val)
                gains.append(temp)
            else:
                gains.append(calc_info_gain(feature, features, data, data_answers))
        best_feature = gains.index(max(gains))
        tree = {labels[best_feature]: {}}

        # Find possible feature values
        for feature in features[best_feature]:
            index = 0
            new_data = []
            less_new_data = []
            more_new_data = []
            new_answers = []
            less_new_answers = []
            more_new_answers = []
            new_features = []
            new_labels = []

            if features == "continuous":
                for datapoint in data:
                    if datapoint[best_feature] <= cont_val:
                        if best_feature == 0:
                            datapoint = datapoint[1:]
                            new_labels = labels[1:]
                            new_features = features[1:]
                        elif best_feature == len(features):
                            datapoint = datapoint[:-1]
                            new_labels = labels[:-1]
                            new_features = features[:-1]
                        else:
                            new_datapoint = datapoint[:best_feature]
                            new_datapoint.extend(datapoint[best_feature + 1:])
                            datapoint = new_datapoint
                            new_labels = labels[:best_feature]
                            new_labels.extend(labels[best_feature + 1:])
                            new_features = features[:best_feature]
                            new_features.extend(features[best_feature + 1:])
                        less_new_data.append(datapoint)
                        less_new_answers.append(data_answers[index])
                    elif datapoint[best_feature] > cont_val:
                        if best_feature == 0:
                            datapoint = datapoint[1:]
                            new_labels = labels[1:]
                            new_features = features[1:]
                        elif best_feature == len(features):
                            datapoint = datapoint[:-1]
                            new_labels = labels[:-1]
                            new_features = features[:-1]
                        else:
                            new_datapoint = datapoint[:best_feature]
                            new_datapoint.extend(datapoint[best_feature + 1:])
                            datapoint = new_datapoint
                            new_labels = labels[:best_feature]
                            new_labels.extend(labels[best_feature + 1:])
                            new_features = features[:best_feature]
                            new_features.extend(features[best_feature + 1:])
                        more_new_data.append(datapoint)
                        more_new_answers.append(data_answers[index])
                    index += 1
                less_subtree = make_tree(less_new_data, less_new_answers, new_features, new_labels)
                more_subtree = make_tree(more_new_data, more_new_answers, new_features, new_labels)
                tree[labels[best_feature]]["less " + str(cont_val)] = less_subtree
                tree[labels[best_feature]]["more " + str(cont_val)] = more_subtree

            # Not continuous
            else:
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
                if "less" in val:
                    print("    " * depth, key, "<=", val.split()[-1], ":")
                elif "more" in val:
                    print("    " * depth, key, ">", val.split()[-1], ":")
                else:
                    print("    " * depth, key, "=", val, ":")
                print(val)
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

    tree = make_tree(examples, example_answers, features, labels)
    print_tree(tree)


# Execute Order 66
id3()
