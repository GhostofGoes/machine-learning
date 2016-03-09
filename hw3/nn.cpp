// Filename: 	main.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 3 main program logic
// Github:		https://github.com/GhostofGoes/cgoes-cs404

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

double sigmoid( double input );     // Sigmoid function

int main() {
	
	// Initialization
	// weird init values are paranoia for error catching
	int numInputs = 0;
	int numOutputs = 0;
	int rows = 0;
	int cols = 0;
	int testRows = 0;
	int testCols = 0;
	double temp = -99;
	double bias = -1.0; // -1 is what the book uses.
	double eta = 0.025; // 0.25 is what the book uses. open to tweaking.
	int attempts = 1; // if debugging, SET THIS TO A SMALL NUMBER!
	//int num_hidden_layers = 1; // is this a thing
	int num_hidden_nodes = 0;
	double max = 1;
	double min = 1;
	
	
	//		Training Data Input 	//
	cin >> numInputs;
	cin >> num_hidden_nodes;
	cin >> rows;
	cin >> cols;
	numOutputs = cols - numInputs;
	
	Matrix* tinput = new Matrix(rows, numInputs + 1); // +1 for bias
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
	testCols;

	if(DEBUGINPUT) {
		cout << "\nTest Inputs matrix" << endl;
		testInput->printAll();
	}
	
	
	// 						//
	// 		* Training * 	//

	Matrix * hw = new Matrix(num_hidden_nodes, numOutputs, 2);
	Matrix * ow = new Matrix(num_hidden_nodes + 1, numOutputs, 2); // +1 for the extra bias between hidden and output layers
	Matrix * tempmat;
	Matrix * deltah; // error in hidden layer
	Matrix * deltao; // error in output layer
	Matrix * a; // hidden layer output
	Matrix * y; // output layer output
	Matrix * hVec;	
	Matrix * inVec;
	Matrix * t;
	temp = -99;
	
	if(DEBUG) {
		cout << "\nInitialized Hidden Weight matrix..." << endl;
		hw->printAll();
	}

	// Normalize our input
	tinput->normalize(min, max);
	
	// re-re-implementation
	for( int attempt = 0; attempt < attempts; attempt++ ) {
		// TODO randomize order rows are done each attempt?
		for( int r = 0; r < rows; r++ ) {
			inVec = tinput->getRow(r);
			t = toutput->getRow(r);
		
			if(DEBUG) {
			cout << "inVec" << endl;
			inVec->printAll();
			cout << endl;
			}
			// hidden layer
			a = inVec->dot(hw);
			a->sigmoid();
			
			if(DEBUG) {
			cout << "a" << endl;
			a->printAll();
			cout << endl;
			}
			hVec = new Matrix(a);
			hVec->data[0].push_back(bias);
			hVec->numCols++;
			
			if(DEBUG) {
			cout << "hVec" << endl;
			hVec->printAll();
			cout << endl;
			}
			// output layer
			y = hVec->dot(hw);
			y->sigmoid();
			
			if(DEBUG) {
			cout << "y" << endl;
			y->printAll();
			cout << endl;
			}
			// output error
			t = t->sub(y);
			t = t->dot(y);
			
			if(DEBUG) {
			cout << "y" << endl;
			t->printAll();
			cout << endl;
			}
			
			y->scalarPreSub(1.0);
			deltao = t->mult(y);
			
			if(DEBUG) {
			cout << "deltao" << endl;
			deltao->printAll();
			cout << endl;	
			}
			
			// hidden error
			deltah = hVec;
			hVec->scalarPreSub(1.0);
			deltah->mult(hVec);
			hVec->scalarPreSub(1.0);
			tempmat = deltao->dot(ow);
			tempmat = tempmat->transpose();
			deltah = deltah->mult(tempmat);
			
			if(DEBUG) {
			cout << "deltah" << endl;
			deltah->printAll();
			cout << endl;
			}
			
			// update matricies
			hVec = hVec->transpose();
			hVec = hVec->dot(deltao);
			hVec = hVec->dot(eta);
			ow = ow->add(tempmat);
			
			deltah->data[0].pop_back();
			deltah->numCols--;
			
			tempmat = inVec->transpose();
			tempmat = inVec->dot(deltah);
			tempmat = tempmat->dot(eta);
			hw = hw->add(tempmat);
			
			delete deltao;
			delete deltah;
			delete a;
			delete y;
			delete hVec;
			delete inVec;
			delete t;
		} // rows
				
						
	} // attempts	

 
	//							//
	//		 	Testing			//

	Matrix * normTestInput;
	
	cout <<  "BEGIN TESTING" << endl;
	normTestInput = new Matrix(testInput);
	normTestInput->normalize(min, max);
	
	for( int r = 0; r < testRows; r++ ) { // Each row in testing set
		inVec = normTestInput->getRow(r);
		
		for (int i = 0; i < testCols; i++) {
			cout << fixed <<  setprecision(2) <<  testInput->getValue(r, i) <<  " ";
		}

		// hidden layer
		a = inVec->dot(hw);
		a->sigmoid();
		hVec = new Matrix(a);
		hVec->data[0].push_back(bias);
		hVec->numCols++;
		
		// output layer
		y = hVec->dot(hw);
		y->sigmoid();		

		for( int i = 0; i < numOutputs; i++ ) {
			cout <<  fixed <<  setprecision(2) <<  y->getValue(0, i) << " ";
		}
		cout <<  endl; // end the row's output
	} // row in set loop	
	
	
	
	//		Final Cleanup		//
	delete hw;
	delete ow;
	delete tinput;
	delete toutput;
	delete testInput;
	return(0);
}

double sigmoid( double input ) {
	return (2 / (1 + exp(-25 * input)) - 1);
}
