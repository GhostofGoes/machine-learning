// Filename: 	main.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Book: 		Machine Learning: An Algorithmic Perspective
// Description:	Assignments 1 and 2
// Github:		https://github.com/GhostofGoes/cgoes-cs404

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include "matrix.h"

#define DEBUG 0
#define DEBUGINPUT 0

#define HW1 1
#define HW2 1

using namespace std;

double activateStep( double input ); 		// Step Function
double activateSigmoid( double input );     // Sigmoid function

int main() 
{
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
	
	if(HW2)
		tinput->normalize();
	
	/**** Training ****/
	Matrix * w = new Matrix(numInputs, numOutputs, 2);
	double tResult = -66.6; // the init values are paranoia for error catching
	double aResult = -55.5;
	double fResult = -42.0;
	
	if(DEBUG) {
		cout << "\nInitialized Weight matrix..." << endl;
		w->printAll();
	}	
	
	for( int t = 0; t < attempts; t++ ) {
		for( int r = 0; r < rows; r++ ) { 					// Each row in training set
			for( int out = 0; out < numOutputs; out++ ) { 	// Each output neuron
				
				tResult = 0.0;	// Reset tempResults
				
				// Multiply inputs by weight matrix
				for( int i = 0; i < numInputs; i++ ) { 		// Each input
					tResult += tinput->getValue(r, i) * w->getValue(i, out);
				}
				
				// Activate neurons using appropriate function
				if(HW1) {
					aResult =  activateStep(tResult);
				}
				else {
					aResult = activateSigmoid(tResult);
				}
				
				if( aResult != toutput->getValue(r, out) ) {
					for( int i = 0; i < numInputs; i++ ) {
						w->setValue(i, out, (w->getValue(i,out) - ((eta * (aResult - toutput->getValue(r, out))) * tinput->getValue(r, i))));
					}
				}
				
			}
			
		} // row in set loop
	} // attempts loop
	
	// ** Test Data Input **
	
	temp = -99; // paranoia
	cin >> rows;
	cin >> cols;
	Matrix* test = new Matrix(rows, cols + 1);
	tResult = -66.6;
	fResult = -42.0;

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
			cout << fixed <<  setprecision(2) <<  test->getValue(r, i) <<  " ";
		}
		
		for( int out = 0; out < numOutputs; out++ ) { 	// Each output
			
			tResult = 0.0; // clear tempResults before we add
			
			for( int i = 0; i < numInputs; i++ ) { 		// Each input
				tResult += test->getValue(r, i) * w->getValue(i, out);
			}
			
			if(HW1)
				fResult =  activateStep(tResult);
			else
				fResult = activateSigmoid(tResult);
			
			cout <<  fixed <<  setprecision(2) <<  fResult << " ";
		} // per-output loop

		cout <<  endl; // end the row's output
	} // row in set loop	
	
	
	// Cleanup
	delete w;
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
	if( (1 / (1 + exp(-42 * input))) > 0.0 )
		return 1.0;
	else
		return 0.0;
}