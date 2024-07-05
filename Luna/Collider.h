#ifndef COLLIDER_H
#define COLLIDER_H
#include "Shape.h"

namespace Luna
{
    class Manifold; // Forwatd declaration of the Manifold class
    class RigidBody; // Forward declaration of the RigidBody class

    /**
     * @brief Function pointer type for collision detection callbacks
     */
    typedef void (*CollisionCallback)(Manifold* m, RigidBody* a, RigidBody* b); // Function pointer to the collision functions

    /**
     * @brief 2D array of collision detection function pointers
     */
    extern CollisionCallback Dispatch[Shape::eCount][Shape::eCount]; // Array of function pointers to the collision functions

    /**
     * @brief Detect collision between two circles
     * @param m Manifold to store collision information
     * @param rbA First circle's rigid body
     * @param rbB Second circle's rigid body
     */
    void CircleToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB); // Function to check collision between two circles
    
    /**
     * @brief Detect collision between a circle and a polygon
     * @param m Manifold to store collision information
     * @param rbA Circle's rigid body
     * @param rbB Polygon's rigid body
     */
    void CircleToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB); // Function to check collision between a circle and a polygon
    
    /**
     * @brief Detect collision between a polygon and a circle
     * @param m Manifold to store collision information
     * @param rbA Polygon's rigid body
     * @param rbB Circle's rigid body
     */
    void PolygonToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB); // Function to check collision between a polygon and a circle
    
    /**
     * @brief Detect collision between two polygons
     * @param m Manifold to store collision information
     * @param rbA First polygon's rigid body
     * @param rbB Second polygon's rigid body
     */
    void PolygonToPolygon(Manifold* m, RigidBody* rbA, RigidBody* b); // Function to check collision between two polygons

    /**
     * @brief Clip a line segment against a plane
     * @param n Normal vector of the clipping plane
     * @param c Distance of the plane from the origin
     * @param face Array of two points representing the line segment to be clipped
     * @return Number of points after clipping (0, 1, or 2)
     */
    int Clip(Vector2 n, real c, Vector2* face); // Function to clip the polygon
    
    /**
     * @brief Find the axis of least penetration between two polygons
     * @param faceIndex Pointer to store the index of the face with least penetration
     * @param A First polygon
     * @param B Second polygon
     * @return The penetration depth along the least penetrating axis
     */
    real FindAxisLeastPenetration(size_t* faceIndex, ShapePolygon* A, ShapePolygon* B); // Function to find the axis with the least penetration 
    
    /**
     * @brief Find the incident face of a polygon given a reference face
     * @param v Array to store the two vertices of the incident face
     * @param RefPoly Reference polygon
     * @param IncPoly Incident polygon
     * @param referenceIndex Index of the reference face
     */
    void FindIncidentFace(Vector2* v, ShapePolygon* RefPoly, ShapePolygon* IncPoly, size_t referenceIndex); // Function to find the incident face
}

#endif // !COLLIDER_H