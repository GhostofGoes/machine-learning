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

Matrix::Matrix() {
	numRows = 0;
	numCols = 0;
}

Matrix::Matrix( int rows, int cols ) {
	numRows = rows;
	numCols = cols;
	data.reserve(rows);
	for ( auto x : data )
		x.reserve(cols);
	/*
	data = new double*[numRows];
	for( int i = 0; i < numRows; i++) {
		data[i] = new double[numCols];
	}
	
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			data[r][c] = 0.0;
		}
	}
	*/
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
	
	srand(time(NULL));
	
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			//data[r][c] =  range - ((double)rand() / RAND_MAX);
			data[r][c] =  ((double)rand() / RAND_MAX) * range - (range/2.0);
		}
	}	
}

Matrix::Matrix( Matrix * init ) {
	numRows = init->rows();
	numCols = init->cols();
	
	/*
	data = new double*[numRows];
	for( int i = 0; i < numRows; i++) {
		data[i] = new double[numCols];
	}
	*/
	
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] = init->getValue(i, j);
			// data[i] = init[i];
		}
	}
	
}

Matrix::~Matrix() {
	// empty destructor for now
}


// Scalar multiplication
void Matrix::s_mult( double scalar ) {
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] *= scalar;
		}
	}
}

// Matrix multiplication
Matrix * Matrix::m_mult( Matrix * mat ) {
	
	// Check that matricies are able to be multiplied
	if( numCols != mat->rows() ) {
		cerr << "#cols != #rows for multiplication!" << endl;
		return NULL;
	}
	int newRows = numRows;
	int newCols = mat->cols();
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

// Vector Multiplication
Matrix * Matrix::v_mult( vector<double> vec ) {
	
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
	if( numRows != mat->rows() || numCols != mat->cols() ) {
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


double Matrix::getValue( int row, int col ) const {
	if( row >= numRows || col >= numCols ) {
		cerr << "getValue out of bounds!" << endl;
		return -50.0;
	}
		else {
			return data[row][col];
		}
}

vector<double> Matrix::getRow( int row ) const {
	return data[row];
}

// TODO
vector<double> Matrix::getCol( int col ) const {
	// tbd, dynamic memory issues (which, hey, vectors fix?)
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

void Matrix::setRow( vector<double> x, int row ) {
	if( row >= numRows ) {
		cerr << "setRowToVec out of bounds!" << endl;
		return;
	}	
	else
		data[row] = x;
}

void Matrix::setCol( vector<double> y, int col ) {
	if( col >= numCols ) {
		cerr << "setColToVec out of bounds!" << endl;
		return;
	}	
	for( int i = 0; i < numCols; i++ )
	{
		data[i][col] = y[i];
	}
}

int Matrix::rows() const {
	return numRows;
}

int Matrix::cols() const {
	return numCols;
}


void Matrix::printAll() const {
	for( int i = 0; i < numRows; i++ ) {
		printRow(i);
	}
}
void Matrix::printRow( int row ) const {
	for( int i = 0; i < numCols; i++ ) {
		cout << " " << data[row][i];
	}
	cout << endl;
}

void Matrix::printCol( int col ) const {
	for( int i = 0; i < numRows; i++ ) {
		cout << " " << data[i][col];
	}
	cout << endl;
}

vector<double> Matrix::operator[]( int row ) {
	return this->getRow(row);
}

// TODO will this cause issues with calling constructor?
vector<double> Matrix::operator()( int col ) {
	return this->getCol(col);
}
