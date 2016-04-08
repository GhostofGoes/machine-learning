// Filename: 	main.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 2 main program logic

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "matrix.h"

#define DEBUG 0
#define DEBUGINPUT 0
#define DEBUG_RESULTS 0


using namespace std;

double activate( double input ); 	// Activation Function
double sigmoid( double input );     // Sigmoid function

// TODO: change matrix class to use vectors

int main() {
	
	// Initialization
	int numInputs = 0; // do you want a comment for this, non-existance comment monkey?
	int rows = 0;
	int cols = 0;
	double temp = -99; // 99 bottles of brain juice in the bar matrix...
	double bias = -1; // -1 is what the book uses.
	double eta = 0.025; // 0.25 is what the book uses. open to tweaking.
	int attempts = 20000; // if you're debugging the loop, SET THIS TO A SMALL NUMBER!
	
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
	
	// Normalize our input (THIS COULD BE IMPROVED)
	tinput->normalize();
	
	/**** Training ****/
	
	Matrix * aResults = new Matrix(rows, numOutputs);
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
				
				// Reset tempResults
				tResult = 0.0;
				
				// Multiply inputs by weight matrix
				for( int i = 0; i < numInputs; i++ ) { 
					tResult += tinput->getValue(r, i) * w->getValue(i, out);
				}
				
				// Calculate the sigmoid function
				tResult = sigmoid(tResult);
				
				// Activate neurons based on sigmoid result
				aResult = activate(tResult);
				
				// Update the weight matrix
				if( aResult != toutput->getValue(r, out) ) {
					for( int i = 0; i < numInputs; i++ ) {
						w->setValue(i, out, (w->getValue(i,out) - ((eta * (tResult - toutput->getValue(r, out))) * tinput->getValue(r, i))));
					}
				}
				
				if(DEBUG) {
					aResults->setValue(r, out, aResult);
				}
				
			} // outputs loop
		} // row in set loop
	} // attempts loop
	
	if(DEBUG) {
		cout << "\nFinal Weight matrix..." << endl;
		w->printAll();
	}
	
	if (DEBUG) {
		cout << "\nActivated Training Results" << endl;
		aResults->printAll();
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
	tResult = -66.6;
	fResult = -42.0;
	double * tempResults = new double[numOutputs];
	double * finalResults = new double[numOutputs];
	int prevMaxInd = 0;
	double max = 0.0;
	
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
		for (int i = 0; i < (numInputs - 1); i++) {
			cout << fixed <<  setprecision(2) <<  test->getValue(r, i) <<  " ";
		}
		
		for( int out = 0; out < numOutputs; out++ ) { 	// Each output
			
			tResult = 0.0; // clear tempResults before we add
			
			for( int i = 0; i < numInputs; i++ ) { 		// Each input
				tResult += test->getValue(r, i) * w->getValue(i, out);
			}
			
			tResult = sigmoid(tResult);
			fResult = activate(tResult);
			tempResults[out] = tResult;
			finalResults[out] = fResult;
			
		} // per-output loop
		/*
		for( int i = 0; i < numOutputs; i++ ) {
			if(finalResults[i] > 0.0 && tempResults[i] > max) {
				if(i > 0) {
					finalResults[prevMaxInd] = 0.0;
				}
				max = tempResults[i];
				prevMaxInd = i;
			}
			else if(finalResults[i] > 0.0) {
				finalResults[i] = 0.0;
			}
		}
		*/
		for( int i = 0; i < numOutputs; i++ ) {
			cout <<  fixed <<  setprecision(0) <<  finalResults[i] << " ";
		}
		cout <<  endl; // end the row's output
		max = 0.0;
		prevMaxInd = 0;
	} // row in set loop	
	
	
	// Cleanup
	//delete tempResults;
	//delete activatedResults;
	delete tempResults;
	delete finalResults;
	delete aResults;
	delete w;
	delete tinput;
	delete toutput;
	delete test;
	return(0);
}

double activate( double input ) {
	if( input > 0.0 )
		return 1.0;
	else
		return 0.0;
}

double sigmoid( double input ) {
	return (2 / (1 + exp(-25 * input)) - 1);
}