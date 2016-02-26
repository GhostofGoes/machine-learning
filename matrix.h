// Filename: 	matrix.h
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Matrix class declarations

#include <vector>
using namespace std;

class Matrix {
	public:
	Matrix();
	Matrix( int rows, int cols );
	Matrix( int rows, int cols, double range );
	Matrix( Matrix * initMat );
	~Matrix();
	
	void 			s_mult( double scalar );
	Matrix * 		m_mult( Matrix * mat );
	vector<double> 	v_mult( vector<double> vec ) const;
	
	void 		add( double scalar );
	void 		add( Matrix * mat );
	
	Matrix * 	transpose();
	Matrix * 	inverse();
	void 		normalize(); // normalizes data in the matrix
	
	double 			getValue( int row, int col ) const;
	vector<double> 	getRow( int row ) const;
	vector<double> 	getCol( int col ) const;
	
	void 		setValue( int row, int col, double value );
	void 		setRow( vector<double> x, int row );
	void 		setCol( vector<double> y, int col );
	
	int 		rows() const;
	int 		cols() const;
	
	void 		printAll() const;
	void 		printRow( int row ) const;
	void 		printCol( int col ) const;
	
	//vector<double> operator[]( int row );
	//vector<double> operator()( int col );
		
	private:
		vector< vector< double > > data;
		int numRows;
		int numCols;
};
