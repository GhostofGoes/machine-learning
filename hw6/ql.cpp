// Filename: 	ql.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 6: Q-learning Mountain Car
// Github:		https://github.com/GhostofGoes/cgoes-cs404

#include <iostream>
#include "mat.h"
using namespace std;

#define DEBUG 0

int main() {
	
	int numRows = 15; 	// position: 	steps of 0.05
	int numCols = 41; 	// velocity: 	steps of 0.01
	Matrix a[3];		// actions:		forward, coast, reverse
	
	// The third dimension: actions. Thanks matt. 
	a[0] = Matrix(numRows, numCols, "forward");
	a[1] = Matrix(numRows, numCols, "coast");	
	a[2] = Matrix(numRows, numCols, "reverse");
	
	for( mat : a )
		mat.rand(-1, 1);
	
	// Physics
	// vt+1 = bound(vt + 0.001at − 0.0025 cos(3xt))
	// xt+1 = bound(xt + vt+1)
	// bound enforces −1.2 ≤ xt+1 ≤ 0.8 and −0.07 ≤ vt+1 ≤ 0.07
	// Steps: 0.05 in position direction, 0.01 in velocity direction
	// Reward: +1 at goal, -1 not at goal
	// action(t) = 1 forward, action(t) = 0 coast, action(t) = -1 reverse

	

	// Output: 3 matricies
	// Format: #rows #cols \n <matrix data> 
	// Possibly another \n, guess I'll have to see once test script is up. This wouldn't matter if we used python ;=)
	for( mat : a )
	{
		cout << numRows << ' ' << numCols << endl;
		mat.write();
	}
	
	return(0);
}

