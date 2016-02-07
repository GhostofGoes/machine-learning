// Filename: 	main.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignments 1 and 2 main program logic

#include <iostream>
#include <cstdlib>
#include <vector>
//#include <cmath>
#include "matrix.h"

#define DEBUG 1

using namespace std;

int activate( double input ); // activates a thing


int main() {
	
	// Initialization
	int numInputs = 0;
	int rows = 0;
	int cols = 0;
	double temp = -99;
	double bias = -1;
	double eta = 0.25; // what the book uses
	int attempts = 10;
	
	/* Training Data Input */
	
	cin >> numInputs;
	cin >> rows;
	cin >> cols;
	int numOutputs = cols - numInputs;
	Matrix* tinput = new Matrix(rows, numInputs + 1);
	Matrix* toutput = new Matrix(rows, numOutputs); 
	
	
	if(DEBUG) {
		cout << "Inputs: " << numInputs << "\nRows: " << rows << "\nCols: " << cols << endl;
	}

	// Fill the matrix
	for(int r = 0; r < rows; r++) {
		for(int c = 0; c < numInputs; c++) {
			cin >> temp;
			tinput->setValue( r, c, temp);
		}
		
		for(int c = 0; c < numOutputs; c++ ) {
			cin >> temp;
			toutput->setValue( r, c, temp);			
		}
	}
	
	// Append the bias
	for( int r = 0; r < rows; r++ ) {
		tinput->setValue(r, numInputs, bias);
	}
	numInputs++;
	

	if(DEBUG) {
		cout << "\nTraining Inputs..." << endl;
		tinput->printAll();
		cout << "\nTraining Outputs..." << endl;
		toutput->printAll();
	}	
	
	
	/**** Training ****/
	
	double * tempResults = new double[numOutputs];
	double * activatedResults = new double[numOutputs];
	Matrix * results = new Matrix(rows, numOutputs);
	Matrix * w = new Matrix(numInputs, numOutputs, 1);
	if(DEBUG) {
		cout << "\nInitialized Weight matrix..." << endl;
		w->printAll();
	}	
	
	for( int t = 0; t < attempts; t++ ) {
		for( int r = 0; r < rows; r++ ) { 					// Each row in training set

			// Multiply (dot product? meh) inputs by weight matrix
			for( int out = 0; out < numOutputs; out++ ) { 	// Each output
				for( int i = 0; i < numInputs; i++ ) { 		// Each input
					tempResults[out] += tinput->getValue(r, i) * w->getValue(i, out);
				}			
			}
			
			// ACTIVATE ALL NEURONZZZZZZZZZZZZZ
			// bwuahahah		
			for( int i = 0; i < numOutputs; i++ ) {
				activatedResults[i] =  activate(tempResults[i]);
			}	

			// UPDATE THE ALL THE WEIGHTS!
			for( int i = 0; i < numInputs; i++ ) {
				for( int j = 0; j < numOutputs; j++ ) {
					w->setValue(i, j, (w->getValue(i,j) - eta * (activatedResults[j] - toutput->getValue(r, j) * tinput->getValue(r, j) ) ));
				}
			}			
			
			results->setRowToVec( activatedResults, r); 
		} // row in set loop
	} // attempts loop
	
	if(DEBUG) {
		cout << "\ntempResults..." << endl;
		for( int i = 0; i < numOutputs; i++ ) {
			cout << "tempResults[" << i << "]: " << tempResults[i] << endl;
		}
	}

	if(DEBUG) {
		cout << "\nactivatedResults..." << endl;
		for( int i = 0; i < numOutputs; i++ ) {
			cout << "activatedResults[" << i << "]: " << activatedResults[i] << endl;
		}
	}	
	
	if(DEBUG) {
		cout << "\nFinal Weight matrix..." << endl;
		w->printAll();
	}
	
	cout << "\nResults" << endl;
	results->printAll();
	
	
	// ** Test Data Input **
	
	temp = -99; // paranoia
	cin >> rows;
	cin >> cols;
	Matrix* test = new Matrix(rows, cols);
	
	// Fill the matrix
	for(int r = 0; r < rows; r++) {
		for(int c = 0; c < cols; c++) {
			cin >> temp;
			test->setValue(r, c, temp);
		}
	}	
	
	if(DEBUG) {
		cout << "\nTest Inputs matrix" << endl;
		test->printAll();
	}
	
	// Cleanup
	delete tinput;
	delete toutput;
	delete test;
	return(0);
}

int activate( double input ) {
	if( input > 0 )
		return 1;
	else
		return 0;
}