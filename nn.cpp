// Filename: 	main.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignments 1 and 2 main program logic

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "matrix.h"

#define DEBUG 0
#define DEBUGINPUT 0
#define HW1 0
#define HW2 1

using namespace std;

double activateStep( double input ); 		// Step Function
double activateSigmoid( double input );     // Sigmoid function

int main() {
	
	// Initialization
	int numInputs = 0;
	int rows = 0;
	int cols = 0;
	double temp = -99;
	double bias = -1;
	double eta = 0.25; // what the book uses
	int attempts = 1000;
	
	/* Training Data Input */
	
	cin >> numInputs;
	cin >> rows;
	cin >> cols;
	int numOutputs = cols - numInputs;
	Matrix* tinput = new Matrix(rows, numInputs + 1);
	Matrix* toutput = new Matrix(rows, numOutputs); 
	
	
	if(DEBUGINPUT) {
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
	

	if(DEBUGINPUT) {
		cout << "\nTraining Inputs..." << endl;
		tinput->printAll();
		cout << "\nTraining Outputs..." << endl;
		toutput->printAll();
	}	
	
	
	/**** Training ****/
	
	double * tempResults = new double[numOutputs];
	double * activatedResults = new double[numOutputs];
	Matrix * results = new Matrix(rows, numOutputs);
	Matrix * w = new Matrix(numInputs, numOutputs, 2);
	
	if(DEBUG) {
		cout << "\nInitialized Weight matrix..." << endl;
		w->printAll();
	}	
	
	// could put the calculation into a function...
	for( int t = 0; t < attempts; t++ ) {
		for( int r = 0; r < rows; r++ ) { 					// Each row in training set

			for (int i = 0; i < numOutputs; i++) {
				tempResults[i] = 0;
			}
					
			// Multiply (dot product? meh) inputs by weight matrix
			for( int out = 0; out < numOutputs; out++ ) { 	// Each output
				for( int i = 0; i < numInputs; i++ ) { 		// Each input
					tempResults[out] += tinput->getValue(r, i) * w->getValue(i, out);
				}			
			}
			
			
			for( int i = 0; i < numOutputs; i++ ) {
				if(HW1) {
					activatedResults[i] =  activateStep(tempResults[i]);
				}
				else {
					activatedResults[i] = activateSigmoid(tempResults[i]);
				}
			}	

			// UPDATE THE ALL THE WEIGHTS!
			for( int j = 0; j < numOutputs; j++ ) {
				if( activatedResults[j] != toutput->getValue(r, j) ) {
					//cout << "activatedResults[" << j << "]: " << activatedResults[j] << endl;
					//cout <<  "toutput[" <<  j <<  "]: " <<  toutput->getValue(r, j) <<  endl;
					for( int i = 0; i < numInputs; i++ ) {
						w->setValue(i, j, (w->getValue(i,j) - ((eta * (activatedResults[j] - toutput->getValue(r, j))) * tinput->getValue(r, i))));
					}
				}
			}
			
			results->setRowToVec( activatedResults, r);
		} // row in set loop
	} // attempts loop
	
	if (DEBUG) {
		cout <<  "final weight matrix" <<  endl;
		w->printAll();
	}
	
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
	
	if (DEBUG) {
	cout << "\nTraning Results" << endl;
	results->printAll();
 }
 
	if (DEBUG) {
	cout <<  "What they should be" <<  endl;
	toutput->printAll();
 }
	
	// ** Test Data Input **
	
	temp = -99; // paranoia
	cin >> rows;
	cin >> cols;
	Matrix* test = new Matrix(rows, cols + 1);

	// Fill the matrix
	for(int r = 0; r < rows; r++) {
		for(int c = 0; c < cols; c++) {
			cin >> temp;
			test->setValue(r, c, temp);
		}
	}	
	
	// Append the bias
	for( int r = 0; r < rows; r++ ) {
		test->setValue(r, cols, bias);
	}
	cols++;
	
	if(DEBUGINPUT) {
		cout << "\nTest Inputs matrix" << endl;
		test->printAll();
	}
	
	cout <<  "BEGIN TESTING" << endl;
	
	for( int r = 0; r < rows; r++ ) { 					// Each row in training set

		for (int i = 0; i < numOutputs; i++) {
			tempResults[i] = 0;
		}
				
		// Multiply (dot product? meh) inputs by weight matrix
		for( int out = 0; out < numOutputs; out++ ) { 	// Each output
			for( int i = 0; i < numInputs; i++ ) { 		// Each input
				tempResults[out] += test->getValue(r, i) * w->getValue(i, out);
			}			
		}
		
		for( int i = 0; i < numOutputs; i++ ) {
			if(HW1) {
				activatedResults[i] =  activateStep(tempResults[i]);
			}
			else {
				activatedResults[i] = activateSigmoid(tempResults[i]);
			}
		}	
				
		//results->setRowToVec(activatedResults, r);
		for (int i = 0; i < numOutputs; i++) {
			cout << fixed <<  setprecision(2) <<  test->getValue(r, i) <<  " ";
		}
		for (int i = 0; i < numOutputs; i++) {
			cout <<  fixed <<  setprecision(2) <<  activatedResults[i] << " ";
		}
		cout <<  endl;
	} // row in set loop	
	
	
	// Cleanup
	delete tinput;
	delete toutput;
	delete test;
	return(0);
}

double activateStep( double input ) {
	if( input > 0.0 )
		return 1.0;
	else
		return 0.0;
}

double activateSigmoid( double input ) {
		
}