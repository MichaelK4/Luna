#ifndef LIBS_H
#define LIBS_H

#define ESC 27 // ESC is a macro that represents the ASCII value of the escape key.
#define RIGID_BODY 100
#define SOFT_BODY 101

/* Windows is a library that provides
functions for working with the
Windows operating system. */
#include <Windows.h>

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


#undef max
#undef min

// ENGINE
#include "glut.h"
//#include "Vector2.h"
#include "Mat2.h"
#include "Clock.h"
//#include "RigidBody.h"
#include "SoftBody.h"
#include "Shape.h"
#include "Collider.h"
#include "Manifold.h"
#include "Scene.h"

#endif // !LIBS_H

