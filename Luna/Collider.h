#ifndef COLLIDER_H
#define COLLIDER_H

#include "Manifold.h"
#include "Shape.h"


namespace Luna
{

	typedef void (*CollisionCallback)(Manifold* m, RigidBody* a, RigidBody* b);

	extern CollisionCallback Dispatch[Shape::eCount][Shape::eCount];


    void CircleToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB);
    void CircleToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB);
    void PolygonToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB);
    void PolygonToPolygon(Manifold* m, RigidBody* rbA, RigidBody* b);

    int Clip(Vector2 n, real c, Vector2* face);
    real FindAxisLeastPenetration(uint* faceIndex, ShapePolygon* A, ShapePolygon* B);
    void FindIncidentFace(Vector2* v, ShapePolygon* RefPoly, ShapePolygon* IncPoly, uint referenceIndex);

}

#endif // !COLLIDER_H
