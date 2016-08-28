#!/usr/bin/env python3
"""

For a number written in Roman numerals to be considered valid there are basic rules which must be followed.
Even though the rules allow some numbers to be expressed in more than one way
there is always a "best" way of writing a particular number.

For example, it would appear that there are at least six ways of writing the number sixteen:

IIIIIIIIIIIIIIII
VIIIIIIIIIII
VVIIIIII
XIIIIII
VVVI
XVI

However, according to the rules only XIIIIII and XVI are valid,
and the last example is considered to be the most efficient, as it uses the least number of numerals.

The 11K text file, p089_roman.txt, contains one thousand numbers written in valid, but not necessarily minimal,
Roman numerals; see About... Roman Numerals for the definitive rules for this problem.

Find the number of characters saved by writing each of these in their minimal form.

Note: You can assume that all the Roman numerals in the file contain no more than four consecutive identical units.

-----------------------------------------------------------------------------------------------------------------------

The regular expression, numerical reference, and some of the function patterns are sourced from the roman PyPi package.
"""

import re

# Define pattern to detect valid Roman numerals
romanNumeralPattern = re.compile("""
    ^                   # beginning of string
    M{0,4}              # thousands - 0 to 4 M's
    (CM|CD|D?C{0,3})    # hundreds - 900 (CM), 400 (CD), 0-300 (0 to 3 C's),
                        #            or 500-800 (D, followed by 0 to 3 C's)
    (XC|XL|L?X{0,3})    # tens - 90 (XC), 40 (XL), 0-30 (0 to 3 X's),
                        #        or 50-80 (L, followed by 0 to 3 X's)
    (IX|IV|V?I{0,3})    # ones - 9 (IX), 4 (IV), 0-3 (0 to 3 I's),
                        #        or 5-8 (V, followed by 0 to 3 I's)
    $                   # end of string
    """, re.VERBOSE)

# Define digit mapping
romanNumeralMap = (('M',  1000),
                   ('CM', 900),
                   ('D',  500),
                   ('CD', 400),
                   ('C',  100),
                   ('XC', 90),
                   ('L',  50),
                   ('XL', 40),
                   ('X',  10),
                   ('IX', 9),
                   ('V',  5),
                   ('IV', 4),
                   ('I',  1))


def simplify_numeral(n):
    saved = 0

    return saved


# Get the data
numerals = []
with open('p089_roman.txt', 'r') as romanfile:
    for line in romanfile:
        numerals.append(line)

characters_saved = 0
for numeral in numerals:
    if not romanNumeralPattern.search(numeral):
        print("String %s is not a valid roman numeral!" % numeral)
    else:
        characters_saved += simplify_numeral(numeral)

print("%s characters were saved by simplifying." % characters_saved)
