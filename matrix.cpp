// Filename: 	matrix.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Matrix class definition

#include <iostream>
#include "matrix.h"

using namespace std;

Matrix::Matrix( int rows, int cols ) {
	numRows = rows;
	numCols = cols;
	
	data = new double*[numRows];
	for( int i = 0; i < numRows; i++) {
		data[i] = new double[numCols];
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

void Matrix::multiply( double scalar ) {
	
}

Matrix * Matrix::multiply( Matrix * mat ) {
	
}

void Matrix::add( double scalar ) {
	
}

void Matrix::add( Matrix * mat ) {
	
}

void Matrix::transpose() {
	
}

double Matrix::getValue( int row, int col ) {
	return data[row][col];
}

double * Matrix::getRow( int row ) {
	return data[row];
}

double * Matrix::getCol( int col ) {
	// tbd, dynamic memory issues
}

// possible buffer overflow here
void Matrix::setRowToVec( double * x, int row ) {
	for( int i = 0; i < numCols; i++ )
	{
		data[row][i] = x[i];
	}
}

// and here
void Matrix::setColToVec( double * y, int col ) {
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