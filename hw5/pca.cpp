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
#define DEBUGINPUT 1
#define DEBUG_RESULTS 1

using namespace std;

const int NUM_VECTORS = 10;

int main() {
	
	//int numRows = 0;
	//int numCols = 0;
	Matrix data("data");
	
	// Read in the input dataset
	data.read();
	if(DEBUGINPUT) { cout << "**data matrix**" << endl; data.print(); }
	
	//numRows = data.maxRows();
	//numCols = data.maxCols();
	//if(DEBUGINPUT) { cout << "numRows: " << numRows << endl; cout << "numCols: " << numCols << endl; }
	Matrix centered = data.subRowVector(data.meanVec());
	cout << "centered" << endl;
	centered.print();
	
	// Correlation Matrix
	Matrix corr = data.cov();
	if(DEBUG) { cout << "\nCorrelation Matrix" << endl; corr.print(); }

	// Create eigen vectors and values
	Matrix eVecs(corr);
	Matrix eVals = eVecs.eigenSystem();
	if(DEBUG_EIGEN) { cout << "\nEigen Values" << endl; eVals.print(); }	
	if(DEBUG_EIGEN) { cout << "\nEigen Vectors" << endl; eVecs.print(); }

	
	// Normalize Eigenvectors
	//eVecs.normalize();
	//if(DEBUG_EIGEN) { cout << "\nNormalized Eigen Vectors" << endl; eVecs.print(); }
	
	// Sort eigenvectors by eigenvalue
	eVals = eVals.transpose(); // leaky boat
	eVecs.transposeSelf(); // less leaky but very square boat
	
	int index = 0;
	for( int i = 0; i < eVecs.maxRows() - 1; i++ )
	{
		index = i;
		for(int j = i + 1; j < eVecs.maxRows(); j++)
		{
			if(eVals.get(j, 0) > eVals.get(index, 0))
				index = j;
		}
		eVecs.swapRows(index, i);
		eVals.swapRows(index, i);
	}
	
	eVals = eVals.transpose();
	eVecs.transposeSelf();
	
	if(DEBUG_RESULTS) { cout << "\nSorted Eigen Values" << endl; eVals.print(); }
	if(DEBUG_RESULTS) { cout << "\nSorted Eigen Vectors" << endl; eVecs.print(); }
	
	Matrix compressed = centered.dotT(eVecs);
	compressed.write();
	
	Matrix recovered = compressed.dot(eVecs);
	
	/*
	SOME BS
	vector<pair <double, double*> > sorting;
	double * eigenvals = eVals.getRow(0);
	
	for(int i = 0; i < eVecs.maxRows(); i++)
	{
		sorting.push_back(make_pair(eigenvals[i], eVecs.getRow(i))); // This probably leaks memory
	}
	
	sort(sorting.begin(), sorting.end());
	
	cout << "\nSorted eigenvectors" << endl;
	for(int i = 0; i < numRows; i++) // Wormulon runs gcc 4.4 since UI CS dept is trapped in the late 90's. No C++ 11
	{
		cout << "Eigen Value: " << sorting[i].first << endl;
		cout << "\tEigen Vector: ";
		for( int c = 0; c < eVecs.maxCols(); c++ )
		{
			cout << " " << sorting[i].second[c];
		}
		cout << endl;
	}*/
	
	
	return(0);
}

