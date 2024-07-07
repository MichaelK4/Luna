#ifndef LIBS_H
#define LIBS_H

#ifndef GL_BGR // GL_BGR is a macro that represents the blue, green, and red color order.
#define GL_BGR GL_BGR_EXT // GL_BGR is a macro that represents the blue, green, and red color order.
#endif 

#define ESC 27 // ESC is a macro that represents the ASCII value of the escape key.
#define WIDTH 800 // 800
#define HEIGHT 600 // 600
#define TITLE "Luna2D - Physics Engine" // Luna2D - Physics Engine
#define LOGO "Luna2D-Logo.bmp" // Luna2D-Logo.bmp
#define POX_X 350 // 350
#define POX_Y 100 // 100 
// gluOrtho2D(left, right, bottom, top)
#define LEFT_ORTHO 0 // 0
#define RIGHT_ORTHO 80 // 80
#define BOTTOM_ORTHO 60 // 60
#define TOP_ORTHO 0 // 0 
#define DELTA_TIME 1.0f / 60.0f // 1.0f / 60.0f
#define ITERATIONS 10 // 10

/* algorithm is a library that provides a
collection of functions for working with
arrays and other data structures.*/
#include <algorithm>

#include <string> // string is a library that provides functions for working with strings in C++ programs.
#include <cstring> // cstring is a library that provides functions for working with C-style strings in C++ programs.

#include <vector> // vector is a library that provides functions for working with arrays in C++ programs.
#include <cfloat> // cfloat is a library that provides functions for working with floating point numbers in C++ programs.
#include <cstdlib> // cstdlib is a library that provides functions for performing general tasks in C++ programs.
#include <iostream> // iostream is a library that provides basic input and output services for C++ programs.

#include <ctime> // ctime is a library that provides functions to work with date and time information.
#include <cmath> // cmath is a library that provides mathematical functions for floating point numbers.
#include <cassert> // cassert is a library that provides a macro that can be used to verify assumptions made by the programmer.

#undef max // max is a macro that represents the maximum value of a number.
#undef min // min is a macro that represents the minimum value of a number.

// ENGINE
#include "glut.h"
#include "Mat2.h"
#include "Clock.h" 
#include "RigidBody.h"
#include "Shape.h"
#include "Collider.h"
#include "Manifold.h"
#include "Scene.h"

//extern Luna::Vector2 GRAVITY; 
//extern Luna::real GRAVITY_S;
//extern Luna::real gravityScale;

#endif // !LIBS_H