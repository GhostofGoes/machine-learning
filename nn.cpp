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
	
	
	//		Test Data Input 	//
	cin >> testRows;
	cin >> testCols;
	Matrix * testInput = new Matrix(testRows, testCols + 1);

	// Fill the matrix
	for(int r = 0; r < testRows; r++) {
		for(int c = 0; c < testCols; c++) {
			cin >> temp;
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
	vector<double> inVec;
	vector<double> outVec (numOutputs);
	vector<double> testVec;
	vector<double> oError (numOutputs);
	vector<double> hError (numOutputs);
	double tResult = -66.6; //init values are paranoia for error catching
	double aResult = -55.0;
	double fResult = -42.0;
	temp = -99;
	
	if(DEBUG) {
		cout << "\nInitialized Hidden Weight matrix..." << endl;
		hw->printAll();
	}

	// Normalize our input (THIS COULD BE IMPROVED) TODO
	tinput->normalize();
	
	vector<double> deltah; // error in hidden layer
	vector<double> deltao; // error in output layer
	vector<double> a; // hidden layer output
	vector<double> y; // output layer output
	vector<double> hVec;	
	vector<double> v;
	vector<double> t;
	Matrix * sumh;
	Matrix * sumo;
	
	
	// re-implementation
	for( int attempt = 0; attempt < attempts; attempt++ ) {
		
		
		for( int r = 0; r < rows; r++ ) {
			v = tinput->getRow(r);
			t = toutput->getRow(r);
			sumh = new Matrix(hw);
			sumo = new Matrix(ow);
			
			// hidden layer
			a = hw->dot(t);
			for( double &x : a ) {
				x = sigmoid(x);
			}
			hVec.push_back(-1);
			
			// output layer
			y = ow->dot(hVec); // TODO probably a issue with num outputs here...
			for( double &x : y ) {
				x = sigmoid(x);
			}
			
			// output error
			for( int i = 0; i < numOutputs; i++) {
				t[i] -= y[i];
				t[i] *= y[i];
				y[i] = 1 - y[i];
				deltao[i] = t[i] * y[i];
			}
			
			
			// hidden error
			deltah = hVec;
			for( int i = 0; i < numInputs + 1; i++ ) {
				deltah[i] *= (1 - deltah[i]);
			}
			tempmat = ow->transpose();
			deltao = tempmat->dot(deltao);
			for( int i = 0; i < numInputs + 1; i++ ) {
				deltah[i] *= deltao[i];
			}
			
			// update sums?
			hw->add(
			
			// update matricies
			sumh->dot(eta);
			hw->add(sumh);
			sumo->dot(eta);
			ow->add(sumo);
			
		} // rows
				
						
	} // attempts	
	
	
	
		
	// TODO randomize order rows are processed for each attempt
	for( int attempt = 0; attempt < attempts; attempt++ ) {
		// could use a range-based for using getRow...somehow
		for( int r = 0; r < rows; r++ ) { //row in training set
			testVec = toutput->getRow(r);
			inVec = tinput->getRow(r);
			
			// Hidden Layer (s)
			// Will eventually put a loop to handle more than one output
			for( int out = 0; out < (numInputs - 1); out++ ) {
				tResult = 0.0;
				
				// Multiply inputs by weight matrix
				for( int i = 0; i < numInputs; i++ ) { 
					tResult += inVec[i] * hw->getValue(i, out);
				}
				
				tResult = sigmoid(tResult);
				hVec[out] = tResult;
			}
			
			hVec[numInputs] = bias;
			
			// Output Layer
			for( int out = 0; out < numOutputs; out++ ) {
				tResult = 0.0;
				
				for( int i = 0; i < numInputs; i++ ) { 
					tResult += hVec[i] * ow->getValue(i, out);
				}
				tResult = sigmoid(tResult);
				outVec[out] = tResult;
			}
			
			// Compute error in output
			for( int out = 0; out < numOutputs; out++ ) {
				oError[out] = (outVec[out] - testVec[out]) * outVec[out] * (1 - outVec[out]);
			}
			
			// Compute error in hidden
			for( int out = 0; out < numOutputs; out++ ) {
				for( int i = 0; i < numOutputs; i++ ) {
					tResult = ow->getValue(out, i) * oError[i];
				}
				hError[out] = hVec[out] * (1 - hVec[out]) * tResult;
			}			
			
			// Update output layer weight matrix
			for( int out = 0; out < numOutputs; out++ ) {
				for( int i = 0; i < numInputs; i++ ) {
					tResult =  ow->getValue(i, out) - (eta * oError[out] * hVec[out]);
					ow->setValue(i, out, tResult);
				}
			}
			
			// Update hidden layer weight matrix
			for( int out = 0; out < numOutputs; out++ ) {
				for( int i = 0; i < numInputs; i++ ) {
					tResult = hw->getValue(i,  out) - (eta * hError[out] * inVec[out]);
					hw->setValue(i, out, tResult);
				}
			}	
		} // row in set loop
	} // attempts loop

	

	
 
 
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
