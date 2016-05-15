// Filename: 	ql.cpp
// Author: 		Christopher Goes
// Course: 		CS 404 Machine Learning and Data Mining
// Semester: 	Spring 2016
// Description:	Assignment 6 - Q-learning - Mountain Car problem
// Book: 		Machine Learning: An Algorithmic Perspective 
// Github:		https://github.com/GhostofGoes/cgoes-cs404

#include <iostream>
#include <random>
#include <cmath>
#include "mat.h"
using namespace std;


typedef struct { double p, v; int x, y; } state;

int maxAction( state s );
double vel( state s, int action ); 				// vt+1 = bound(vt + 0.001at − 0.0025 cos(3xt))
double pos( double currPos, double newVel ); 	// xt+1 = bound(xt + vt+1)
double clampX( double x );						// −1.2 ≤ xt+1 ≤ 0.8
double clampY( double y );						// −0.07 ≤ vt+1 ≤ 0.07
double convertAction( int action );
int vToX( double v );
int pToY( double p );

Matrix q[3]; // path matrix

int main()
{
	int numEpisodes = 10000;	// Number of "episodes" to train and improve the path matrix
	int goal = 0.8;				// Where we want to be
	int numRows = 15; 		// velocity: 	steps of 0.01
	int numCols = 41; 		// position: 	steps of 0.05
	
	state s;				// Current State
	state sprime;			// Next state
	int a = 0; 				// Current Action: 0=reverse, 1=coast, 2=forward
	int aprime = 0; 		// Next action to be taken
	double reward = 0.0;	// Reward from sampling next action
	
	double mu = 0.2;		// 0.7 from book
	double gamma = 0.5;		// 0.4 from book
	double epsilon = 0.2;	// 0.1 from book
	
	double temp = 0.0;		// Improve code readability
	default_random_engine eng; 								// generate random numbers from distributions
	uniform_real_distribution<double> realDist(0.0, 1.0); 	// epsilon decisions
	uniform_int_distribution<int> intDist(0, 2);			// picking an action
		
	q[0] = new Matrix(numRows, numCols, "forward"); // reverse in assignment, forward in test script
	q[1] = new Matrix(numRows, numCols, "coast");	
	q[2] = new Matrix(numRows, numCols, "reverse");	 // forward in assignment, reverse in test script
	for( Matrix &mat : q )
		mat.rand(-1.0, 1.0); // initialize Q-matricies to random values	
	
	
	// Training
	for( int i = 0; i < numEpisodes; i++ )
	{
		s.p = s.v = 0.0;
		s.x = s.y = 0;
		sprime.p = sprime.v = 0.0;
		sprime.x = sprime.y = 0;
		a = aprime = 0;
		
		while(s.p < goal ) // One "episode" or "trace"
		{
			// Select action a using epsilion-greedy
			if(realDist(eng) < epsilon)
				aprime = intDist(eng); 
			else
				aprime = maxAction(s);
						
			// Take action a and recieve reward r 
			reward = q[aprime].get(s.x, s.y);
			
			// Sample new state s'
			sprime.v = vel(s, aprime);
			sprime.x = vToX(sprime.v);
			sprime.p = pos(s.p, sprime.v);
			sprime.y = pToY(sprime.p);
						 
			// Update Q(s, a) <- Q(s, a) + mu(r + gamma max_a' Q(s', a') - Q(s, a))
			temp = q[a].get(s.x, s.y) + (mu * ( reward + gamma * q[aprime].get(sprime.x, sprime.y) - q[a].get(s.x, s.y) ) );
			q[a].set(s.x, s.y, temp); // remember, rows = v, cols = p
			
			// Set s <- s'
			s = sprime;
			a = aprime;
		}
	}
	
	
	// Output: 3 Q-matricies to "run a mountain car"
	for( Matrix &mat : q )
		mat.write();
	
	return 0;
}


// select action with greatest reward
int maxAction( state s )
{
	int action = 0;
	double max = q[action].get(s.x, s.y);
	
	for(int i = 0; i < 3; i++)
	{
		if(q[i].get(s.x, s.y) > max)
		{
			action = i;
			max = q[i].get(s.x, s.y);
		}
	}
	
	return action;
}

// vt+1 = bound(vt + 0.001at − 0.0025 cos(3xt))
double vel( state s, int action )
{
	double newVel = s.v;
	newVel += 0.001 * convertAction(action);
	newVel -= 0.0025 * cos(3.0 * s.p);
	return clampY(newVel);
}

// xt+1 = bound(xt + vt+1)
double pos( double currPos, double newVel )
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

// NOTE: swapped negative since test script is backwards
// Converts [-1, 1] to [0, 2] 
double convertAction( int action )
{
	return action == 0 ? 1.0 : action == 1 ? 0.0 : action == 2 ? -1.0 : -99.0;
}

int vToX( double v )
{
	int x = 0;
	double comp = -0.07;
	
	while(v != comp && comp < 0.07)
	{
		x++;
		comp += 0.01;
	}
	return x;
}

int pToY( double p )
{
	int y = 0;
	double comp = -1.2;
	
	while(p != comp && comp < 0.8)
	{
		y++;
		comp += 0.05;
	}
	
	return y;
}