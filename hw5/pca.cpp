// Filename: 	pca.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 5 main program logic
// Github:		https://github.com/GhostofGoes/cgoes-cs404

#include <iostream>
#include "mat.h"

#define DEBUG 1
#define DEBUG_EIGEN 1
#define DEBUGINPUT 0
#define DEBUG_RESULTS 0

using namespace std;

int main() {
	
	int numRows = 0;
	int numCols = 0;
	Matrix data("data");
	
	// Read in the input dataset
	data.read();
	if(DEBUGINPUT) { cout << "**data matrix**" << endl; data.print(); }
	
	numRows = data.maxRows();
	numCols = data.maxCols();
	if(DEBUGINPUT) { cout << "numRows: " << numRows << endl; cout << "numCols: " << numCols << endl; }
	
	// Correlation Matrix
	Matrix corr(data.cov());
	if(DEBUG) { cout << "\nCorrelation Matrix" << endl; corr.print(); }

	// Create eigen vectors and values
	Matrix eVecs(corr);
	Matrix eVals(eVecs.eigenSystem());
	if(DEBUG_EIGEN) { cout << "\nEigen Vectors" << endl; eVecs.print(); }
	if(DEBUG_EIGEN) { cout << "\nEigen Values" << endl; eVals.print(); }
	
	eVecs.normalize();
	if(DEBUG_EIGEN) { cout << "\nNormalized Eigen Vectors" << endl; eVecs.print(); }
	
	return(0);
}

