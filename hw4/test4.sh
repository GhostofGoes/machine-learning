#!/usr/bin/env bash

suite="$1"
test="$2"

if [ $suite -eq "1" ] 
then path="./data/";
fi

if [ $suite -eq "2" ] 
then path="./v2testdata/";
fi

if [ $test == "all" ] 
then for f in $path*.in; do python3 id3.py "$f"; done
else python3 id3.py < ./testDataA4/$test.in;
fi

