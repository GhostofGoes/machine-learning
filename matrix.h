// Filename: 	matrix.h
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Matrix class declarations

#include <vector>
using namespace std;

class Matrix {
	public:
	//Matrix() { numRows = numCols = 0; }
	Matrix( int rows, int cols );
	Matrix( int rows, int cols, double range );
	Matrix( Matrix * initMat );
	~Matrix() {}
	
	// old shitty methods (hey look they're gone)
	
	// new not-shitty methods
	// TODO make sure i'm freeing matricies that are returned in the caller
	Matrix *		dot( double scalar ) 		const;
	Matrix *		dot( Matrix * mat )			const;
	vector<double>	dot( vector<double> vec ) 	const;
	
	Matrix *		add( double scalar )	const;
	Matrix *		add( Matrix * mat )		const;
	
	// Operate on self without modifying self
	Matrix * 		transpose()	const;
	Matrix * 		inverse()	const;
	Matrix *		normalize() const; // the old normalize function from HW1/HW2
	Matrix *		normalize( double min, double max )	const;
	
	// Cool
	void			map( const void * func ); 		// func = function to map to every element in matrix
	void			mapToRow( int row, const void * func );
	void			mapToCol( int col, const void * func );
	
	// Boring
	double 			getValue( int row, int col ) 	const;
	vector<double> 	getRow( int row ) 				const;
	vector<double> 	getCol( int col ) 				const;
	
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
		
	private:
		vector< vector< double > > data;
		int numRows;
		int numCols;
};
