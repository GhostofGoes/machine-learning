// Filename: 	matrix.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Matrix class definition

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "matrix.h"

using namespace std;
// TODO: write a unit test?

Matrix::Matrix( int rows, int cols ) {
	numRows = rows;
	numCols = cols;
	
	data = new double*[numRows];
	for( int i = 0; i < numRows; i++) {
		data[i] = new double[numCols];
	}
	
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			data[r][c] = 0.0;
		}
	}
	
		/*
	for( auto &x : data ) {
		for( auto &y : x ) {
			cout << y << endl;
		}
	}*/
}

Matrix::Matrix( int rows, int cols, double range ) {
	numRows = rows;
	numCols = cols;
	
	data = new double*[numRows];
	for( int i = 0; i < numRows; i++) {
		data[i] = new double[numCols];
	}
	
	srand(time(NULL));
	
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			//data[r][c] =  range - ((double)rand() / RAND_MAX);
			data[r][c] =  ((double)rand() / RAND_MAX) * range - (range/2.0);
		}
	}	
}

Matrix::Matrix( Matrix * init ) {
	numRows = init->getNumRows();
	numCols = init->getNumCols();
	
	data = new double*[numRows];
	for( int i = 0; i < numRows; i++) {
		data[i] = new double[numCols];
	}
	
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] = init->getValue(i, j);
		}
	}
	
}

Matrix::~Matrix() {
	for( int i = 0; i < numRows; i++ ) {
		delete data[i];
	}
	delete data;
}

// primary reason for using vectors, right here.
// #BufferOverflow #BetterCodingPractices
void Matrix::initByRow( double * x, int row ) {
	if(x == NULL || row >= numRows ) {
		return;
	}
	for( int c = 0; c < numCols; c++ ) {
		data[row][c] = x[c];
	}
}

void Matrix::initByCol( double * y, int col ) {
	if(y == NULL || col >= numCols ) {
		return;
	}
	for( int r = 0; r < numRows; r++ ) {
		data[r][col] = y[r];
	}	
}


// Scalar multiplication
void Matrix::multiply( double scalar ) {
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] *= scalar;
		}
	}
}

// Matrix multiplication
Matrix * Matrix::multiply( Matrix * mat ) {
	
	// Check that matricies are able to be multiplied
	if( numCols != mat->getNumRows() ) {
		cerr << "#cols != #rows for multiplication!" << endl;
		return NULL;
	}
	int newRows = numRows;
	int newCols = mat->getNumCols();
	int temp = 0;
	Matrix * result = new Matrix(newRows, newCols);
	
	for( int r = 0; r < newRows; r++ ) { // every row in first matrix == row in result
		for( int c = 0; c < newCols; c++ ) {
			temp = 0;
			for( int j = 0; j < newCols; j++ ) { // every column in first matrix == value in result
				temp += ( data[r][j] * mat->getValue(j, c) ); // this.getValue?
			}
			result->setValue(r, c, temp);
		}
	}
	
	return result;
}

// Scalar addition
void Matrix::add( double scalar ) {
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] += scalar;
		}
	}	
}

// Matrix addition
void Matrix::add( Matrix * mat ) {
	
	// Check matricies are the same size
	if( numRows != mat->getNumRows() || numCols != mat->getNumCols() ) {
		cerr << "Matrix sizes don't match for addition!" << endl;
		return;
	}	
	
	// Add the matricies
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] += mat->getValue(i, j);
		}
	}	
	
}

Matrix * Matrix::transpose() {
	int newRows = numCols;
	int newCols = numRows;
	Matrix * result = new Matrix(newRows, newCols);
	
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(c, r, data[r][c]);
		}
	}

}

// TODO
Matrix * Matrix::inverse() {
	int newRows = 0;
	int newCols = 0;	
	Matrix * result = new Matrix(newRows, newCols);
	
}

void Matrix::normalize() {
	double norm = 0.0;
	
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			norm += pow(data[r][c], 2);
		}
		
		norm = sqrt(norm);
		
		for( int c = 0; c < numCols; c++ ) {
			data[r][c] /= norm;
		}
		
		norm = 0.0;
	}
	
}


double Matrix::getValue( int row, int col ) {
	if( row >= numRows || col >= numCols ) {
		cerr << "getValue out of bounds!" << endl;
		return -50.0;
	}
	else {
		return data[row][col];
	}
}

double * Matrix::getRow( int row ) {
	return data[row];
}

double * Matrix::getCol( int col ) {
	// tbd, dynamic memory issues
}

void Matrix::setValue( int row, int col, double value ) {
	if( row >= numRows || col >= numCols ) {
		cerr << "setValue out of bounds!" << endl;
		return;
	}
	else {
		data[row][col] = value;
	}
}

// possible buffer overflow here
void Matrix::setRowToVec( double * x, int row ) {
	if( row >= numRows ) {
		cerr << "setRowToVec out of bounds!" << endl;
		return;
	}	
	for( int i = 0; i < numCols; i++ )
	{
		data[row][i] = x[i];
	}
}

// and here
void Matrix::setColToVec( double * y, int col ) {
	if( col >= numCols ) {
		cerr << "setColToVec out of bounds!" << endl;
		return;
	}	
	for( int i = 0; i < numCols; i++ )
	{
		data[i][col] = y[i];
	}	
}

int Matrix::getNumRows() {
	return numRows;
}

int Matrix::getNumCols() {
	return numCols;
}


void Matrix::printAll() {
	for( int i = 0; i < numRows; i++ ) {
		printRow(i);
	}
}
void Matrix::printRow( int row ) {
	for( int i = 0; i < numCols; i++ ) {
		cout << " " << data[row][i];
	}
	cout << endl;
}

void Matrix::printCol( int col ) {
	for( int i = 0; i < numRows; i++ ) {
		cout << " " << data[i][col];
	}
	cout << endl;
}