#include "Libs.h"

namespace Luna
{
    // 2D array of collision detection function pointers
    CollisionCallback Dispatch[Shape::eCount][Shape::eCount] =
    {
        { CircleToCircle, CircleToPolygon },
        { PolygonToCircle, PolygonToPolygon },
    };

    void CircleToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB) // Circle to Circle collision
    {
        // Cast the shapes to circle type
        ShapeCircle* A = (ShapeCircle*)rbA->shape;
        ShapeCircle* B = (ShapeCircle*)rbB->shape;

        // Calculate the translational vector (normal) from circle A to circle B
        Vector2 normal = rbB->position - rbA->position;

        // Compute the squared distance between the circles' centers
        real distSqr = normal.LengthSqr();
        // Sum of the radii of both circles
        real radius = A->radius + B->radius;

        // Check if the circles are colliding by comparing the squared distance to the squared radii sum
        if (distSqr >= radius * radius)
        {
            m->contactCount = 0; // No collision
            return;
        }
        // Compute the actual distance between the circles' centers
        real dist = std::sqrt(distSqr); // perform actual sqrt
        m->contactCount = 1; // There is one contact point

        if (dist == 0.0f) // If the circles are exactly on top of each other
        {
            // The circles are coincident; use an arbitrary normal and set penetration to the radius of A
            m->penetration = A->radius; // Penetration depth
            m->normal = Vector2(1, 0); // Arbitrary normal
            m->contacts[0] = rbA->position; // Contact point is the position of circle A
        }
        else
        {
            // Circles are overlapping but not coincident
            m->penetration = radius - dist; // Penetration depth
            m->normal = normal / dist; // Normalize the collision normal
            m->contacts[0] = m->normal * A->radius + rbA->position; // Compute the contact point
        }
    }

    void CircleToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB) // Circle to Polygon collision
    {
        // Cast shapes to Circle and Polygon types
        ShapeCircle* A = (ShapeCircle*)rbA->shape;
        ShapePolygon* B = (ShapePolygon*)rbB->shape;

        m->contactCount = 0;

        // Transform circle center to Polygon model space
        Vector2 center = rbA->position;
        center = B->mat.Transpose() * (center - rbB->position);

        // Find edge with minimum penetration - this is the edge that needs to be clipped
        // The concept is using support points in polygon v polygon to find the closest edge to the circle
        real separation = -FLT_MAX;
        size_t faceNormal = 0;

        for (size_t i = 0; i < B->vertex_count; i++)
        {
            real dist = Dot(B->normals[i], center - B->vertices[i]);

            if (dist > A->radius)
                return;

            if (dist > separation)
            {
                separation = dist;
                faceNormal = i;
            }
        }

        // Grab face's vertices - we need to find the closest edge
        Vector2 v1 = B->vertices[faceNormal];
        size_t i2 = faceNormal + 1 < B->vertex_count ? faceNormal + 1 : 0;
        Vector2 v2 = B->vertices[i2];

        // Check to see if the center is within the polygon
        if (separation < EPSILON)
        {
            m->contactCount = 1;
            m->normal = (B->mat * B->normals[faceNormal]);
            m->normal = -m->normal;
            m->contacts[0] = m->normal * A->radius + rbA->position;
            m->penetration = A->radius;
            return;
        }

        // Determine which vertex of the edge is closest to the circle
        real dot1 = Dot(center - v1, v2 - v1);
        real dot2 = Dot(center - v2, v1 - v2);
        m->penetration = A->radius - separation;

        // Closest to v1
        if (dot1 <= 0.0f)
        {
            if (DistSqr(center, v1) > A->radius * A->radius)
                return;

            m->contactCount = 1;
            Vector2 n = v1 - center;
            n = B->mat * n;
            n.Normalise();
            m->normal = n;
            v1 = B->mat * v1;// +rbB->position;
            v1 += rbB->position;
            m->contacts[0] = v1;
        }
        else if (dot2 <= 0.0f) // Closest to v2
        {
            if (DistSqr(center, v2) > A->radius * A->radius)
                return;

            m->contactCount = 1;
            Vector2 n = v2 - center;
            v2 = B->mat * v2;// +rbB->position;
            v2 += rbB->position;
            m->contacts[0] = v2;
            n = B->mat * n;
            n.Normalise();
            m->normal = n;
        }
        else // Closest to face
        {
            Vector2 n = B->normals[faceNormal];
            if (Dot((center - v1), n) > A->radius)
                return;

            n = B->mat * n;
            m->normal = -n;
            m->contacts[0] = m->normal * A->radius + rbA->position;
            m->contactCount = 1;
        }
    }

    void PolygonToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB) // Polygon to Circle collision
    {
        CircleToPolygon(m, rbB, rbA);
        m->normal = (-m->normal);
    }

    void PolygonToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB) // Polygon to Polygon collision
    {
        ShapePolygon* A = (ShapePolygon*)rbA->shape;
        ShapePolygon* B = (ShapePolygon*)rbB->shape;

        m->contactCount = 0;
        size_t faceA;
        real penetrationA = FindAxisLeastPenetration(&faceA, A, B);
        if (penetrationA >= 0.0f)
            return;

        size_t faceB;
        real penetrationB = FindAxisLeastPenetration(&faceB, B, A);
        if (penetrationB >= 0.0f)
            return;

        size_t referenceIndex;
        bool flip; // point from a to b all the time

        ShapePolygon* RefPoly;
        ShapePolygon* IncPoly;

        if (BiasGreaterThan(penetrationA, penetrationB))
        {
            RefPoly = A;
            IncPoly = B;
            referenceIndex = faceA;
            flip = false;
        }
        else
        {
            RefPoly = B;
            IncPoly = A;
            referenceIndex = faceB;
            flip = true;
        }

        Vector2 incidentFace[2];
        FindIncidentFace(incidentFace, RefPoly, IncPoly, referenceIndex);

        Vector2 v1 = RefPoly->vertices[referenceIndex];
        referenceIndex = referenceIndex + 1 == RefPoly->vertex_count ? 0 : referenceIndex + 1;
        Vector2 v2 = RefPoly->vertices[referenceIndex];

        v1 = RefPoly->mat * v1;
        v1 += RefPoly->rb->position;
        v2 = RefPoly->mat * v2;
        v2 += RefPoly->rb->position;

        Vector2 sidePlaneNormal = (v2 - v1);
        sidePlaneNormal.Normalise();

        Vector2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

        real refC = Dot(refFaceNormal, v1);
        real negSide = (-Dot(sidePlaneNormal, v1));
        real posSide = Dot(sidePlaneNormal, v2);

        if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
            return;

        if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
            return;

        m->normal = flip ? (-refFaceNormal) : refFaceNormal;

        size_t cp = 0;
        real separation = Dot(refFaceNormal, incidentFace[0]) - refC;
        if (separation <= 0.0f)
        {
            m->contacts[cp] = incidentFace[0];
            m->penetration = -separation;
            cp++;
        }
        else
            m->penetration = 0;
        separation = Dot(refFaceNormal, incidentFace[1]) - refC;
        if (separation <= 0.0f)
        {
            m->contacts[cp] = incidentFace[1];
            m->penetration += (-separation);
            cp++;
            m->penetration /= (real)cp;
        }
        m->contactCount = cp;
    }

    void FindIncidentFace(Vector2* v, ShapePolygon* RefPoly, ShapePolygon* IncPoly, size_t referenceIndex) // Find the incident face 
    {
        Vector2 referenceNormal = RefPoly->normals[referenceIndex];

        referenceNormal = RefPoly->mat * referenceNormal;
        referenceNormal = IncPoly->mat.Transpose() * referenceNormal;

        int incidentFace = 0;
        real minDot = FLT_MAX;
        for (size_t i = 0; i < IncPoly->vertex_count; i++)
        {
            real dot = Dot(referenceNormal, IncPoly->normals[i]);
            if (dot < minDot)
            {
                minDot = dot;
                incidentFace = i;
            }
        }
        v[0] = IncPoly->mat * IncPoly->vertices[incidentFace];
        v[0] += IncPoly->rb->position;
        incidentFace = incidentFace + 1 >= (int)IncPoly->vertex_count ? 0 : incidentFace + 1;
        v[1] = IncPoly->mat * IncPoly->vertices[incidentFace];
        v[1] += IncPoly->rb->position;
    }

    real FindAxisLeastPenetration(size_t* faceIndex, ShapePolygon* A, ShapePolygon* B) // Find the axis with the least penetration
    {
        real bestDistance = (-FLT_MAX);
        size_t bestIndex = 0;

        for (size_t i = 0; i < A->vertex_count; i++)
        {
            Vector2 n = A->normals[i];
            Vector2 nw = A->mat * n;

            Mat2 buT = B->mat.Transpose();
            n = buT * nw;

            Vector2 s = B->GetBest(-n);

            Vector2 v = A->vertices[i];
            v = A->mat * v;
            v += A->rb->position;
            v -= B->rb->position;
            v = buT * v;

            real d = Dot(n, s - v);

            if (d > bestDistance)
            {
                bestDistance = d;
                bestIndex = i;
            }
        }
        *faceIndex = bestIndex;
        return bestDistance;
    }

    int Clip(Vector2 n, real c, Vector2* face) // Clip the polygon to the line
    {
        size_t sp = 0;
        Vector2 tmp = { 0,0 };
        Vector2 out[2] = { face[0], face[1] };
        real d1 = Dot(n, face[0]) - c;
        real d2 = Dot(n, face[1]) - c;

        if (d1 <= 0.0f) out[sp++] = face[0];
        if (d2 <= 0.0f) out[sp++] = face[1];

        if (d1 * d2 < 0.0f)
        {
            real alpha = d1 / (d1 - d2);
            tmp = (face[1] - face[0]);
            if (sp < 2)
            {
                out[sp] = tmp * alpha + face[0];
                sp++;
            }
        }
        face[0] = out[0];
        face[1] = out[1];

        assert(sp != 3);
        return sp;
    }
}