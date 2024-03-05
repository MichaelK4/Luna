#include "Collider.h"

namespace Luna
{

    CollisionCallback Dispatch[Shape::eCount][Shape::eCount] =
    {
        {
        CircleToCircle, CircleToPolygon
        },
        {
        PolygonToCircle, PolygonToPolygon
        },
    };


    void CircleToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB)
    {
        Circle* A = (Circle*)rbA->shape;
        Circle* B = (Circle*)rbB->shape;

        // Calculate translational vector - the direction of collision from A to B
        Vector2 normal = rbB->position - rbA->position;

        real distSqr = normal.LengthSqr();
        real radius = A->radius + B->radius;

        // Check if circles are colliding
        if (distSqr >= radius * radius)
        {
			m->contactCount = 0;
			return;
		}

        real dist = std::sqrt(distSqr); // perform actual sqrt
        m->contactCount = 1;

        if (dist == 0.0f)
        {
			m->penetration = A->radius;
			m->normal = Vector2(1, 0);
			m->contacts[0] = rbA->position;
		}
        else
        {
			m->penetration = radius - dist;
			m->normal = normal / dist;
			m->contacts[0] = m->normal * A->radius + rbA->position;
		}
    }


    void CircleToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB)
    {
        Circle* A = (Circle*)rbA->shape;
        ShapePolygon* B = (ShapePolygon*)rbB->shape;

        m->contactCount = 0;

        // Transform circle center to Polygon model space
        Vector2 center = rbA->position;
        center = B->mat.Transpose() * (center - rbB->position);

        // Find edge with minimum penetration - this is the edge that needs to be clipped
        // The concept is using support points in polygon v polygon to find the closest edge to the circle
        real separation = -FLT_MAX;
        uint faceNormal = 0;

        for (uint i = 0; i < B->vertex_count; i++)
        {
			real dist = B->normals[i].Dot(B->normals[i], center - B->vertices[i]);

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
        uint i2 = faceNormal + 1 < B->vertex_count ? faceNormal + 1 : 0;
        Vector2 v2 = B->vertices[i2];

        // Check to see if the center is within the polygon
        if (separation < EPSILON)
        { 
            m->contactCount = 1; 
            m->normal = -(B->mat * B->normals[faceNormal]); 
            m->contacts[0] = m->normal * A->radius + rbA->position; 
            m->penetration = A->radius; 
            return;
        }

        // Determine which vertex of the edge is closest to the circle
        real dot1 = (center - v1).Dot(center - v1 ,v2 - v1);
        real dot2 = (center - v2).Dot(center - v2 ,v1 - v2);
        m->penetration = A->radius - separation; 

        // Closest to v1
        if (dot1 <= 0.0f)
        {
			if (center.DistanceSqrt(v1) > A->radius * A->radius)
				return;

			m->contactCount = 1; 
			Vector2 n = v1 - center; 
			n = B->mat * n; 
			n.Normalise();
			m->normal = n; 
			v1 = B->mat * v1 + rbB->position; 
			m->contacts[0] = v1; 
		}
		else if (dot2 <= 0.0f) // Closest to v2
		{ 
            if (center.DistanceSqrt(v2) > A->radius * A->radius)
                return;

            m->contactCount = 1;
            Vector2 n = v2 - center;
            v2 = B->mat * v2 + rbB->position;
            m->contacts[0] = v2;
            n = B->mat * n; 
            n.Normalise(); 
            m->normal = n;
        }
        else // Closest to face
        {
			Vector2 n = B->normals[faceNormal];
			if ((center - v1).Dot((center - v1), n) > A->radius)
				return;

			n = B->mat * n;
			m->normal = -n;
			m->contacts[0] = m->normal * A->radius + rbA->position;
			m->contactCount = 1;
		}
    }


    void PolygonToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB)
    {
        CircleToPolygon(m, rbB, rbA);
        m->normal = -m->normal;
    }


    void PolygonToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB)
    {
        ShapePolygon* A = (ShapePolygon*)rbA->shape;
        ShapePolygon* B = (ShapePolygon*)rbB->shape;

        m->contactCount = 0;
        uint faceA;
        real penetrationA = FindAxisLeastPenetration(&faceA, A, B);
        if (penetrationA >= 0.0f)
			return;

        uint faceB; 
		real penetrationB = FindAxisLeastPenetration(&faceB, B, A); 
        if (penetrationB >= 0.0f)
            return;

        uint referenceIndex; 
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

        v1 = RefPoly->mat * v1 + rbA->position; 
        v2 = RefPoly->mat * v2 + rbA->position; 

        Vector2 sidePlaneNormal = (v2 - v1); 
        sidePlaneNormal.Normalise(); 

        Vector2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

        real refC = refFaceNormal.Dot(refFaceNormal, v1);
        real negSide = -sidePlaneNormal.Dot(sidePlaneNormal, v1);
        real posSide = sidePlaneNormal.Dot(sidePlaneNormal, v2);

        if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
			return;

        if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
            return;

        m->normal = flip ? -refFaceNormal : refFaceNormal; 

        uint cp = 0;
        real separation = refFaceNormal.Dot(refFaceNormal, incidentFace[0]) - refC;
        if (separation <= 0.0f)
        {
            m->contacts[cp] = incidentFace[0]; 
            m->penetration = -separation; 
            cp++;
        }
        else
        {
            m->penetration = 0; 
        }
        separation = refFaceNormal.Dot(refFaceNormal, incidentFace[1]) - refC; 
        if (separation <= 0.0f)
        {
			m->contacts[cp] = incidentFace[1];  
			m->penetration += -separation;  
			cp++;
			m->penetration /= (real)cp;  
		}
        m->contactCount = cp;  

    }

    void FindIncidentFace(Vector2* v, ShapePolygon* RefPoly, ShapePolygon* IncPoly, uint referenceIndex) 
    {
		Vector2 referenceNormal = RefPoly->normals[referenceIndex];  

		referenceNormal = RefPoly->mat * referenceNormal;  
		referenceNormal = IncPoly->mat.Transpose() * referenceNormal;  

		int incidentFace = 0;  
		real minDot = FLT_MAX;  
        for (uint i = 0; i < IncPoly->vertex_count; i++) 
        {
            real dot = referenceNormal.Dot(referenceNormal, IncPoly->normals[i]); 
            if (dot < minDot) 
            {
				minDot = dot;  
				incidentFace = i;  
			}
        }
    }

    real FindAxisLeastPenetration(uint* faceIndex, ShapePolygon* A, ShapePolygon* B)
    {
		real bestDistance = -FLT_MAX;
		uint bestIndex;

        for (uint i = 0; i < A->vertex_count; i++)
        {
			Vector2 n = A->normals[i];
			Vector2 nw = A->mat * n;

			Mat2 buT = B->mat.Transpose();
            n = buT * nw;

            Vector2 s = B->GetBest(-n); 

            Vector2 v = A->vertices[i]; 
            v = A->mat * v + A->rb->position;
            v -= B->rb->position;
            v = buT * v;

            real d = n.Dot(n, s - v);

            if (d > bestDistance)
            {
                bestDistance = d;
                bestIndex = i;
            }
        }
        *faceIndex = bestIndex;
        return bestDistance;
    }


    int Clip(Vector2 n, real c, Vector2* face)
    {
        uint sp = 0;
        Vector2 out[2] = {face[0], face[1]};
        real d1 = n.Dot(n, face[0]) - c;
        real d2 = n.Dot(n, face[1]) - c;

        if (d1 <= 0.0f) out[sp++] = face[0];
        if (d2 <= 0.0f) out[sp++] = face[1];

        if (d1 * d2 < 0.0f)
        {
            real alpha = d1 / (d1 - d2);
            out[sp] = face[0] + (face[1] - face[0]) * alpha;
            sp++;
        }

        face[0] = out[0];
        face[1] = out[1];

        assert(sp != 3);
        return sp;
    }


}
