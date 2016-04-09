#!/usr/bin/env bash

if [ $1 -eq "all" ]; then
	echo "Testing picA.txt"
	./pca < testDataA5/picA.txt
	echo "Testing picB.txt"
	./pca < testDataA5/picB.txt
	echo "Testing picC.txt"
	./pca < testDataA5/picC.txt
else
	echo "Testing picA.txt"
	./pca < testDataA5/picA.txt
fi