// Filename: 	hw6.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 6 main program logic
// Github:		https://github.com/GhostofGoes/cgoes-cs404

#include <iostream>
#include "mat.h"

#define DEBUG 0
#define DEBUG_EIGEN 0
#define DEBUG_INPUT 0

using namespace std;

int main() {
	
	int fRows = 0;
	int fCols = 0;
	Matrix forward("forward");
	
	int cRows = 0;
	int cCols = 0;
	Matrix coast("coast");
	
	int rRows = 0;
	int rCols = 0;
	Matrix reverse("reverse");
	
	double position = 0.0;
	cin >> position;
	if(DEBUG_INPUT) { cout << "Position: " << position << endl; }
	//Matrix data("data");
	//data.read();
	//if(DEBUG_INPUT) { cout << "**data matrix**" << endl; data.print(); }
	
	// Physics
	// vt+1 = bound(vt + 0.001at − 0.0025 cos(3xt))
	// xt+1 = bound(xt + vt+1)
	// bound enforces −1.2 ≤ xt+1 ≤ 0.8 and −0.07 ≤ vt+1 ≤ 0.07
	// Steps: 0.05 in position direction, 0.01 in velocity direction
	// Reward: +1 at goal, -1 not at goal
	// Starting position: specified by input (???)
	
	
	
	
	
	// Output: 3 matricies
	// Format: #rows #cols \n <matrix data>
	// Col: position Row: velocity
	
	// Reverse
	cout << rRows << ' ' << rCols << endl;
	reverse.write();
	
	
	// Coast
	cout << cRows << ' ' << cCols << endl;
	coast.write();
	
	// Forward
	cout << fRows << ' ' << fCols << endl;
	forward.write();	
	
	
	return(0);
}

