// Filename: 	main.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 3 main program logic

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

int main() {
	
	// Initialization
	int numInputs = 0;
	int numOutputs = 0;
	int rows = 0;
	int cols = 0;
	int testRows = 0;
	int testCols = 0;
	double temp = -99;
	double bias = -1; // -1 is what the book uses.
	double eta = 0.025; // 0.25 is what the book uses. open to tweaking.
	int attempts = 5; // if debugging, SET THIS TO A SMALL NUMBER!
	int num_hidden_layers = 1; // is this a thing
	int num_hidden_nodes = 0;
	double max = 1;
	double min = 1;
	
	
	
	//		Training Data Input 	//
	cin >> numInputs;
	cin >> num_hidden_nodes;
	cin >> rows;
	cin >> cols;
	numOutputs = cols - numInputs;
	
	Matrix* tinput = new Matrix(rows, numInputs + 1);
	Matrix* toutput = new Matrix(rows, numOutputs); 
	
	if(DEBUGINPUT) {
		cout << "Inputs: " << numInputs << "\nHidden Nodes: " << num_hidden_nodes << "\nRows: " << rows << "\nCols: " << cols << endl;
	}
	
	// Fill the matrix
	for(int r = 0; r < rows; r++) {
		for(int c = 0; c < numInputs; c++) {
			cin >> temp;
			if( temp > max ) max = temp;
			else if( temp < min ) min = temp;
			tinput->setValue( r, c, temp);
		}
		
		for(int c = 0; c < numOutputs; c++ ) {
			cin >> temp;
			if( temp > max ) max = temp;
			else if( temp < min ) min = temp;
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
	
	
	//		Test Data Input 	//
	cin >> testRows;
	cin >> testCols;
	Matrix * testInput = new Matrix(testRows, testCols + 1);

	// Fill the matrix
	for(int r = 0; r < testRows; r++) {
		for(int c = 0; c < testCols; c++) {
			cin >> temp;
			if( temp > max ) max = temp;
			else if( temp < min ) min = temp;
			testInput->setValue(r, c, temp);
		}
	}	

	// Append the bias
	for( int r = 0; r < testRows; r++ ) {
		testInput->setValue(r, testCols, bias);
	}
	testCols++;

	if(DEBUGINPUT) {
		cout << "\nTest Inputs matrix" << endl;
		testInput->printAll();
	}
	
	
	
	
	// 						//
	// 		* Training * 	//

	Matrix * hw = new Matrix(numInputs, numOutputs, 2);
	Matrix * ow = new Matrix(numInputs + 1, numOutputs, 2); // +1 for the extra bias between hidden and output layers
	Matrix * tempmat;
	double tResult = -66.6; //init values are paranoia for error catching
	double aResult = -55.0;
	double fResult = -42.0;
	temp = -99;
	
	if(DEBUG) {
		cout << "\nInitialized Hidden Weight matrix..." << endl;
		hw->printAll();
	}

	// Normalize our input (THIS COULD BE IMPROVED) TODO
	tinput->normalize(min, max);
	
	Matrix * deltah; // error in hidden layer
	Matrix * deltao; // error in output layer
	Matrix * a; // hidden layer output
	Matrix * y; // output layer output
	Matrix * hVec;	
	Matrix * inVec;
	Matrix * v;
	Matrix * t;
	Matrix * sumh;
	Matrix * sumo;
	
	
	// re-implementation
	for( int attempt = 0; attempt < attempts; attempt++ ) {
		
		
		for( int r = 0; r < rows; r++ ) {
			inVec = tinput->getRow(r);
			t = toutput->getRow(r);
			
			// hidden layer
			a = inVec->dot(hw);
			a->sigmoid();
			hVec = new Matrix(a);
			hVec->data[0].push_back(bias);
			hVec->numCols++;
			
			// output layer
			y = hVec->dot(hw);
			y->sigmoid();
			
			// output error
			t = t->sub(y);
			t = t->dot(y);
			y->scalarPreSub(1.0);
			deltao = t->dot(y);
				
			// hidden error
			deltah = hVec;
			hVec->scalarPreSub(1.0);
			deltah->dot(hVec);
			hVec->scalarPreSub(1.0);
			tempmat = deltao->dot(ow);
			tempmat = tempmat->transpose();
			deltah = deltah->dot(tempmat);
			
			// update matricies
			tempmat = hVec->transpose();
			tempmat = tempmat->dot(deltao);
			tempmat = tempmat->dot(eta);
			ow = ow->add(tempmat);
			
			deltah->data[0].pop_back();
			deltah->numCols--;
			
			tempmat = inVec->transpose();
			tempmat = inVec->dot(deltah);
			tempmat = tempmat->dot(eta);
			hw = hw->add(tempmat);
			
		} // rows
				
						
	} // attempts	

 
	//							//
	//		 	Testing			//
	tResult = -66.6;
	fResult = -42.0;
	double * tempResults = new double[numOutputs];
	double * finalResults = new double[numOutputs];	
	
	cout <<  "BEGIN TESTING" << endl;
	
	for( int r = 0; r < rows; r++ ) { // Each row in testing set
		for (int i = 0; i < (numInputs - 1); i++) {
			cout << fixed <<  setprecision(2) <<  testInput->getValue(r, i) <<  " ";
		}
		
		for( int out = 0; out < numOutputs; out++ ) { // Each output
			
			tResult = 0.0; // clear tempResults before we add
			
			for( int i = 0; i < numInputs; i++ ) { // Each input
				//tResult += test->getValue(r, i) * w->getValue(i, out);
			}
			
			tResult = sigmoid(tResult);
			fResult = activate(tResult);
			tempResults[out] = tResult;
			finalResults[out] = fResult;
			
		} // per-output loop

		for( int i = 0; i < numOutputs; i++ ) {
			cout <<  fixed <<  setprecision(0) <<  finalResults[i] << " ";
		}
		cout <<  endl; // end the row's output
	} // row in set loop	
	
	
	
	//		Cleanup		//
	delete tempResults;
	delete finalResults;
	delete hw;
	delete ow;
	delete tinput;
	delete toutput;
	delete testInput;
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
