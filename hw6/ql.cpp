// Filename: 	ql.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 6: Q-learning Mountain Car
// Github:		https://github.com/GhostofGoes/cgoes-cs404

#include <iostream>
#include <cmath>
#include "mat.h"

using namespace std;

#define DEBUG 0

// Reward: +1 at goal, -1 not at goal
// action(t) = 1 forward, action(t) = 0 coast, action(t) = -1 reverse
// bound enforces −1.2 ≤ xt+1 ≤ 0.8 and −0.07 ≤ vt+1 ≤ 0.07

int next( double pos, double vel, double action );
double v( double currVel, double action, double currPos ); 	// vt+1 = bound(vt + 0.001at − 0.0025 cos(3xt))
double x( double currPos, double newVel ); 					// xt+1 = bound(xt + vt+1)
double clampX( double x );
double clampY( double y );
Matrix r[3]; // reward matrix that has our "environment" state
Matrix q[3]; // path matrix

int main() {
	
	int numEpisodes = 0;
	int goal = 0.8;		// Where we want to be at the end of this insanity
	int numRows = 15; 	// velocity: 	steps of 0.01
	int numCols = 41; 	// position: 	steps of 0.05
	
	double currVel = 0.0, currPos = 0.0; 	// current state
	double newVel = 0.0, newPos = 0.0;		// new state
	double action = 0; 	// actions:	forward = 1, coast = 0, reverse = -1
	
	r[0] = new Matrix(numRows, numCols, "reverse");
	r[1] = new Matrix(numRows, numCols, "coast");	
	r[2] = new Matrix(numRows, numCols, "forward");
	for( Matrix &mat : r ) // Initialize state to -1, 0, or 1
		mat.randInt(-1, 1);
	
	q[0] = new Matrix(numRows, numCols, "reverse");
	q[1] = new Matrix(numRows, numCols, "coast");	
	q[2] = new Matrix(numRows, numCols, "forward");	
	for( Matrix &mat : q )
		mat.constant(0.0); // zero out path matrix
	
	
	
	

	// Output: 3 Q-matricies to "run a mountain car"
	for( Matrix &mat : q )
		mat.write();
	
	return(0);
}

int next( double pos, double vel, int action )
{
	
}


// vt+1 = bound(vt + 0.001at − 0.0025 cos(3xt))
double v( double currVel, double action, double currPos )
{
	double newVel = currVel;
	newVel += 0.001 * action;
	newVel -= 0.0025 * cos(3.0 * currPos);
	return clampY(newVel);
}

// xt+1 = bound(xt + vt+1)
double x( double currPos, double newVel )
{
	return clampX(currPos + newVel);
}

// −1.2 ≤ xt+1 ≤ 0.8
double clampX( double x )
{
	return x > 0.8 ? 0.8 : x < -1.2 ? -1.2 : x;
}

// −0.07 ≤ vt+1 ≤ 0.07
double clampY( double y )
{
	return y > 0.07 ? 0.07 : y < -0.07 ? -0.07 : y;
}