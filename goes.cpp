// Filename: 	main.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignments 1 and 2 main program logic

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "matrix.h"

using namespace std;

int main() {
	
	// Initialization
	int inputs = 0;
	int rows = 0;
	int cols = 0;
	int temp = -99;
	
	
	// *******************
	// ** Training Data Input **
	
	cin >> inputs;
	cin >> rows;
	cin >> cols;
	Matrix* train = new Matrix(rows, cols);
	
	
	cout << "Inputs: " << inputs << "\nRows: " << rows << "\nCols: " << cols << endl;
	//return(0);
	// Fill the matrix
	for(int r = 0; r < rows; r++) {
		for(int c = 0; c < cols; c++) {
			cin >> temp;
			train->setValue( r, c, temp);
		}
	}

	cout << "Training matrix..." << endl;
	train->printAll();
	
	
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
	
	cout << "Testing matrix..." << endl;
	test->printAll();
	
	// Cleanup
	delete train;
	delete test;
	return(0);
}