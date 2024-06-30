#ifndef COLLIDER_H
#define COLLIDER_H
#include "Shape.h"

namespace Luna
{
    class Manifold; // Manifold is a class that stores the information of the collision
    class RigidBody; // RigidBody is a class that stores the information of the body

    typedef void (*CollisionCallback)(Manifold* m, RigidBody* a, RigidBody* b); // Function pointer to the collision functions

    extern CollisionCallback Dispatch[Shape::eCount][Shape::eCount]; // Array of function pointers to the collision functions

    void CircleToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB); // Function to check collision between two circles
    void CircleToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB); // Function to check collision between a circle and a polygon
    void PolygonToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB); // Function to check collision between a polygon and a circle
    void PolygonToPolygon(Manifold* m, RigidBody* rbA, RigidBody* b); // Function to check collision between two polygons

    int Clip(Vector2 n, real c, Vector2* face); // Function to clip the polygon
    real FindAxisLeastPenetration(uint* faceIndex, ShapePolygon* A, ShapePolygon* B); // Function to find the axis with the least penetration
    void FindIncidentFace(Vector2* v, ShapePolygon* RefPoly, ShapePolygon* IncPoly, uint referenceIndex); // Function to find the incident face
}

#endif // !COLLIDER_H
