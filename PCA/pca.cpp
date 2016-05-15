// Filename: 	pca.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 5 - Principle Component Analysis
// Book: 		Machine Learning: An Algorithmic Perspective
// Github:		https://github.com/GhostofGoes/cgoes-cs404

#include <iostream>
#include "mat.h"

#define DEBUG 0
#define DEBUG_EIGEN 0
#define DEBUG_INPUT 0

using namespace std;

const int NUM_VECTORS = 10;

int main() {
	
	Matrix data("data");
	data.read();
	if(DEBUG_INPUT) { cout << "**data matrix**" << endl; data.print(); }
	
	// Center the data (cov will center for me, I do this for when I need to translate the data later)
	Matrix centered = data.subRowVector(data.meanVec());
	
	// Correlation Matrix
	Matrix corr = data.cov();
	if(DEBUG) { cout << "\nCorrelation Matrix" << endl; corr.print(); }

	// Create eigen vectors and values
	Matrix eVecs(corr);
	Matrix eVals = eVecs.eigenSystem();
	if(DEBUG_EIGEN) { cout << "\nEigen Values" << endl; eVals.print(); }	
	if(DEBUG_EIGEN) { cout << "\nEigen Vectors" << endl; eVecs.print(); }

	
	// Normalize Eigenvectors (apparantly not needed)
	// eVecs.normalize();
	
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
	
	if(DEBUG_EIGEN) { cout << "\nSorted Eigen Values" << endl; eVals.print(); }
	if(DEBUG_EIGEN) { cout << "\nSorted Eigen Vectors" << endl; eVecs.print(); }
	
	

	// Trim columns down for his test script
	eVecs = eVecs.transpose();	
	if( NUM_VECTORS < eVecs.maxCols() )
	{
		eVecs.narrow(NUM_VECTORS);
		eVals.narrow(NUM_VECTORS);
	}
	eVecs = eVecs.transpose();	

	// Compress!
	eVecs = eVecs.transpose();	
	Matrix compressed = centered.dot(eVecs);
	compressed.write();
	
	// Recover the compressed data!
	Matrix recovered = compressed.dot(eVecs);

	
	return(0);
}

