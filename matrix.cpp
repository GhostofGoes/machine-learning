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
	
	vector< vector< double > > temp (rows, vector<double>(cols, 0));
	for ( vector<double> x : temp )
		temp.reserve(cols);
	data = temp;
}

Matrix::Matrix( int rows, int cols, double range ) {
	numRows = rows;
	numCols = cols;
	
	vector< vector< double > > temp (rows, vector<double>(cols, 0));
	for ( vector<double> x : temp )
		temp.reserve(cols);
	data = temp;
		
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
	
	vector< vector< double > > temp (numRows, vector<double>(numCols, 0));
	for ( vector<double> x : temp )
		temp.reserve(numCols);
	data = temp;
	
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] = init->getValue(i, j);
		}
	}
}

void Matrix::s_mult( double scalar ) {
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] *= scalar;
		}
	}
}
  
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

vector<double> Matrix::v_mult( vector<double> vec ) const {
	vector<double> tempvec;
	
	if( vec.size() == numCols ) {
		int temp = 0;
		
		for( int i = 0; i < numRows; i++ ) {
			for( int j = 0; j < numCols; j++ ) {
				temp += (data[i][j] * vec[j]);
			}
			tempvec[i] = temp;
			temp = 0;
		}
	}
	else {
		cerr << "Vector size did not match number of columns in matrix" << endl;
	}
	return tempvec;
}

void Matrix::add( double scalar ) {
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] += scalar;
		}
	}	
}

void Matrix::add( Matrix * mat ) {
	if( numRows != mat->rows() || numCols != mat->cols() ) {
		cerr << "Matrix sizes don't match for addition!" << endl;
		return;
	}	
	
	for( int i = 0; i < numRows; i++ ) {
		for( int j = 0; j < numCols; j++ ) {
			data[i][j] += mat->getValue(i, j);
		}
	}	
	
}




Matrix * Matrix::dot( double scalar ) const {
	Matrix * result = new Matrix(numRows, numCols);
	
	for( int i = 0; i < numRows; i++ ) {
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(i, c, data[i][c] * scalar);
		}
	}
	return result;
}

Matrix * Matrix::dot( Matrix * mat ) const {
	int newRows = numRows;
	int newCols = mat->cols();
	Matrix * result = new Matrix(newRows, newCols);
	int temp = 0;
	
	if( numCols != mat->rows() ) {
		cerr << "#cols != #rows for multiplication!" << endl;
		return result;
	}

	for( int r = 0; r < newRows; r++ ) { // every row in first matrix == row in result
		for( int c = 0; c < newCols; c++ ) {
			temp = 0;
			for( int j = 0; j < newCols; j++ ) { // every column in first matrix == value in result
				temp += ( data[r][j] * mat->getValue(j, c) );
			}
			result->setValue(r, c, temp);
		}
	}
	return result;	
}

vector<double> Matrix::dot( vector<double> vec ) const {
	vector<double> result;
	
	if( vec.size() == numCols ) {
		int temp = 0;
		
		for( int i = 0; i < numRows; i++ ) {
			for( int j = 0; j < numCols; j++ ) {
				temp += (data[i][j] * vec[j]);
			}
			result[i] = temp;
			temp = 0;
		}
	}
	else {
		cerr << "Vector size of " << vec.size() << "did not match number of columns " << numCols << " in matrix" << endl;
	}
	return result;	
}

Matrix * Matrix::transpose() const {
	Matrix * result = new Matrix(numRows, numCols);
	
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(c, r, data[r][c]);
		}
	}
		
	return result;
}

Matrix * Matrix::inverse() const {
	Matrix * result = new Matrix(numRows, numCols);
	
	// TODO
	
}

void Matrix::normalize() {
	double norm = 0.0;
	// TODO
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
	if( row < 0 || row >= numRows ) {
		cerr << "getRow out of bounds" << endl;
		return data[0]; // meh
	}	
	return data[row]; // we can cheat for rows
}

vector<double> Matrix::getCol( int col ) const {
	vector<double> temp(numRows); // col vec size = number of rows
	
	if( col < 0 || col >= numCols )
		cerr << "getCol out of bounds" << endl;
	else
		for( int i = 0; i < numRows; i++ )
			temp[i] = data[i][col];

	return temp;
}


void Matrix::setValue( int row, int col, double value ) {
	if( row >= numRows || col >= numCols )
		cerr << "setValue out of bounds!" << endl;
	else
		data[row][col] = value;
}

void Matrix::setRow( vector<double> x, int row ) {
	if( row >= numRows )
		cerr << "setRowToVec out of bounds!" << endl;
	else
		data[row] = x;
}

void Matrix::setCol( vector<double> y, int col ) {
	if( col >= numCols ) 
		cerr << "setColToVec out of bounds!" << endl;
	else	
		for( int i = 0; i < numCols; i++ )
			data[i][col] = y[i];
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

/*
vector<double> Matrix::operator[]( int row ) {
	return this->getRow(row);
}

// TODO will this cause issues with calling constructor?
vector<double> Matrix::operator()( int col ) {
	return this->getCol(col);
}
*/
