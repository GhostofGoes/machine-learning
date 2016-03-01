// Filename: 	matrix.h
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Matrix class declarations

#include <vector>
using namespace std;

class Matrix {
	public:
	Matrix( int rows, int cols );
	Matrix( int rows, int cols, double range );
	Matrix( Matrix * initMat );
	~Matrix() {} // token destructor
		
	// new not-shitty methods
	// TODO make sure i'm freeing matricies that are returned in the caller (lmao leaking everywhere)
	Matrix *		dot( double scalar ) 		const;
	Matrix *		dot( Matrix * mat )			const;
	
	Matrix *		add( double scalar )	const;
	Matrix *		add( Matrix * mat )		const;
	
	Matrix *		sub( double scalar )	const;
	Matrix * 		sub( Matrix * mat ) 	const;
	
	// Operate on self without modifying self
	Matrix * 		transpose()	const;
	Matrix * 		inverse()	const;
	Matrix *		normalize() const; // the old normalize function from HW1/HW2
	void			normalize( double min, double max );
	
	void			sigmoid();
	void			scalarPreSub(double s);
	// Cool
	void			map( const void * func ); 		// func = function to map to every element in matrix
	void			mapToRow( int row, const void * func );
	void			mapToCol( int col, const void * func );
	
	// Boring
	double 			getValue( int row, int col ) 	const;
	Matrix * 		getRow( int row ) 				;
	Matrix * 		getCol( int col ) 				;
	
	void 		setValue( int row, int col, double value );
	void 		setRow( vector<double> x, int row );
	void 		setCol( vector<double> y, int col );
	
	// Really boring
	int 		rows() const;
	int 		cols() const;
	
	void 		printAll() 			const;
	void 		printRow( int row ) const;
	void 		printCol( int col ) const;
	
	//vector<double> operator[]( int row );
	//vector<double> operator()( int col );
	vector< vector< double > > data; // SCREW IT ALL
	int numRows;
	int numCols;

};
