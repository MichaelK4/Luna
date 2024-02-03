#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

#include "Libs.h"


typedef float real; // Real is a floating point number that represents a real number.


/*Pi is a mathematical constant that represents 
the ratio of a circle's circumference to its diameter.*/
const real PI = 3.14159265358979323846f; 
// Epsilon is a small number that is used to compare floating point numbers.
const real EPSILON = 0.0001f;

real Radius; // Radius is the distance from the center of an object to its edge.
real Inertia; // Inertia is the resistance of any physical object to any change in its velocity.
real InverseInertia; // Inverse inertia is the reciprocal of the inertia.
real Mass; // Mass is the amount of matter in an object.
real InverseMass; // Inverse mass is the reciprocal of the mass.

/*Static friction is the friction that exists
between a stationary object and 
the surface on which it's resting.*/
real StaticFriction; 
/*Dynamic friction is the friction that exists 
between a moving object and 
the surface on which it's moving.*/
real DynamicFriction;
/*Restitution is the measure of how 
much an object bounces off a surface.*/
real Restitution;

const real REAL_GRAVITY = 9.8f; // Real gravity is the force that attracts a body toward the center of the earth.

const real GRAVITY_S = 10.0f; 
const float deltaTime = 1.0f / 60.0f; // Delta time is the amount of time that has passed since the last frame.

// Equal is a function that compares two floating point numbers and returns true if they are equal.
bool Equal(real a, real b); 

// Random is a function that returns a random number between two floating point numbers.
real Random(real a, real b); 

// Round is a function that rounds a floating point number to the nearest integer.
int Round(real a);

// Sqr is a function that returns the square of a floating point number.
real sqr(real a); 


#endif // !ENGINE_MATH_H

