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

int main() {
	
	// Initialization
	int inputs = 0;
	int rows = 0;
	int cols = 0;
	double temp = -99;
	
	
	// *******************
	// ** Training Data Input **
	
	cin >> inputs;
	cin >> rows;
	cin >> cols;
	int outputs = cols - inputs;
	Matrix* tinput = new Matrix(rows, inputs);
	Matrix* toutput = new Matrix(rows, outputs); 
	
	
	if(DEBUG) {
		cout << "Inputs: " << inputs << "\nRows: " << rows << "\nCols: " << cols << endl;
	}

	// Fill the matrix
	for(int r = 0; r < rows; r++) {
		for(int c = 0; c < inputs; c++) {
			cin >> temp;
			tinput->setValue( r, c, temp);
		}
		
		for(int c = 0; c < outputs; c++ ) {
			cin >> temp;
			toutput->setValue( r, c, temp);			
		}
	}

	if(DEBUG) {
		cout << "Training Inputs..." << endl;
		tinput->printAll();
		cout << "Training Outputs..." << endl;
		toutput->printAll();
	}
	
	
	
	// *******************
	// Training
	
	Matrix* w = new Matrix(rows, cols);
	
	
	
	
	
	
	
	
	// *******************
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
		cout << "Test inputs matrix" << endl;
		test->printAll();
	}
	
	// Cleanup
	delete tinput;
	delete toutput;
	delete test;
	return(0);
}