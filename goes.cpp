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
	// ** Training Data **
	// *******************
	
	cin >> inputs;
	cin >> rows;
	cin >> cols;
	Matrix* train = new Matrix(rows, cols);
	
	// Fill the matrix
	for(int i = 0; i < rows; i++) {
		for(int j = 0; i < cols; j++) {
			cin >> temp;
			train->setValue( i, j, temp);
		}
	}
	
	
	// ***************
	// ** Test Data **
	// ***************
	
	temp = -99; // paranoia
	cin >> rows;
	cin >> cols;
	Matrix* test = new Matrix(rows, cols);
	
	// Fill the matrix
	for(int i = 0; i < rows; i++) {
		for(int j = 0; i < cols; j++) {
			cin >> temp;
			test->setValue(i, j, temp);
		}
	}	
	
	
	// Cleanup
	delete train;
	delete test;
	return(0);
}