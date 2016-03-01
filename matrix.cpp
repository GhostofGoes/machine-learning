// Filename: 	matrix.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Matrix class definition
// Github:		https://github.com/GhostofGoes/cgoes-cs404

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
	Matrix * result = new Matrix( numRows, mat->cols() );
	int temp = 0;
	
	for( int r = 0; r < newRows; r++ ) { // every row in first matrix == row in result
		for( int c = 0; c < mat->cols(); c++ ) {
			temp = 0;
			for( int j = 0; j < newCols; j++ ) { // every column in first matrix == value in result
				temp += ( data[r][j] * mat->getValue(j, c) );
			}
			result->setValue(r, c, temp);
		}
	}
	return result;
}

Matrix * Matrix::mult( Matrix * mat ) {
	Matrix * result = new Matrix( numRows, numCols );
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(r, c, data[r][j] + mat->getValue(r, c);
		}
	}
	return result;
}

Matrix * Matrix::add( double scalar ) const {
	Matrix * result = new Matrix(numRows, numCols);
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(r, c, data[r][c] + scalar);
		}
	}		
	return result;
}

Matrix * Matrix::add( Matrix * mat ) const {
	Matrix * result = new Matrix( numRows, numCols );
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(r, c, mat->getValue(r, c) + data[r][c]);
		}
	}
	return result;
}

void Matrix::addSelf( Matrix * mat ) {
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			data[r][c] += mat->getValue(r, c);
		}
	}	
}

Matrix * Matrix::sub( double scalar ) const {
	Matrix * result = new Matrix(numRows, numCols);
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(r, c, data[r][c] - scalar);
		}
	}		
	return result;	
}

Matrix * Matrix::sub( Matrix * mat ) const {
	Matrix * result = new Matrix(numRows, numCols);
	
	if( mat->rows() != numRows ) {
		cerr << "Rows don't match for matrix addition" << endl;
		return result;
	} else if ( mat->cols() != numCols ) {
		cerr << "Cols don't match for matrix addition" << endl;
		return result;
	}
	
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(r, c, data[r][c] - mat->getValue(r, c) );
		}
	}
	
	return result;	
}

void Matrix::subSelf( Matrix * mat ) {
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			data[r][c] -= mat->getValue(r, c);
		}
	}	
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
	return result;
}

Matrix * Matrix::normalize() const {
	Matrix * result = new Matrix(numRows, numCols);
	double norm = 0.0;
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			norm += pow(data[r][c], 2);
		}

		norm = sqrt(norm);
		
		for( int c = 0; c < numCols; c++ ) {
			result->setValue(r, c, data[r][c] / norm);
		}
				norm = 0.0;
	}
	return result;	
}

void Matrix::normalize( double min, double max ) {
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			data[r][c] = (data[r][c] - min) / (max - min);
		}
	}
}

void Matrix::sigmoid() {
	for( int r = 0; r < numRows; r++ ) {
		for( int c = 0; c < numCols; c++ ) {
			data[r][c] = (2 / (1 + exp(-25 * data[r][c])) - 1);
		}
	}
}

void Matrix::scalarPreSub(double s) {
	for( vector<double> &x : data )
		for( double &y : x )
			y = s - y;	
}



void Matrix::map( const void * func ) {
	// TODO
}

void Matrix::mapToRow( int row, const void * func ) {
	// TODO
}

void Matrix::mapToCol( int col, const void * func ) {
	// TODO
}

// ****** BORING STUFF ******* //

double Matrix::getValue( int row, int col ) const {
	if( row >= numRows || col >= numCols ) {
		cerr << "getValue out of bounds!" << endl;
		return -50.0;
	}
	else {
		return data[row][col];
	}
}

Matrix * Matrix::getRow( int row )  {
	vector<double> temp(numRows); // col vec size = number of rows
	Matrix * tempmat = new Matrix( 1, numCols );	
	
	if( row < 0 || row >= numRows ) {
		cerr << "getRow out of bounds" << endl;
		return tempmat; // meh
	}	
	
	tempmat->setRow(data[row], 0);
	return tempmat; // we can cheat for rows
}

Matrix * Matrix::getCol( int col ) {
	vector<double> temp(numRows); // col vec size = number of rows
	Matrix * tempmat = new Matrix( numRows, 1 );
	if( col < 0 || col >= numCols ) {
		cerr << "getCol out of bounds" << endl;
		return tempmat;
	}
	
	for( int i = 0; i < numRows; i++ )
		temp[i] = data[i][col];
	
	tempmat->setCol(temp, 0);
	return tempmat;
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
