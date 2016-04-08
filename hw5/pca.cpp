// Filename: 	pca.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 5 main program logic
// Github:		https://github.com/GhostofGoes/cgoes-cs404

#include <iostream>
//#include <iomanip>
//#include <cstdlib>
//#include <vector>
//#include <cmath>
//#include "matrix.h"
#include "mat.h"

#define DEBUG 0
#define DEBUGINPUT 1
#define DEBUG_RESULTS 0

using namespace std;

int main() {
	
	// Initialization
	int numRows = 0;
	int numCols = 0;
	int totalDatapoints = 0;
	double temp = 0;
	
	//	Data Input 	//
	cin >> numRows;
	cin >> numCols;
	
	totalDatapoints = numRows * numCols;
	
	if(DEBUGINPUT) 
	{
		cout << "#Rows: " << numRows << "\n#Cols: " << numCols << endl;
	}
	
	//	Input data into a matrix
	//Matrix inputData = new Matrix( numRows, numCols );
	double tempdata [totalDatapoints];
	for(int i = 0; i < totalDatapoints; i++)
	{
		cin >> temp;
		tempdata[i] = temp;
	}
	
	Matrix data(numRows, numCols, tempdata, "data");
	
	/*
	for( int r = 0; r < numRows; r++ )
	{
		for( int c = 0; c < numCols; c++ )
		{
			cin >> temp;
			//inputData.setValue(r, c, temp);
		}
	}
	*/
	
	if(DEBUGINPUT)
	{
		cout << "**data matrix**" << endl;
		data.print();
		//inputData.printAll();
	}
	
	return(0);
}

