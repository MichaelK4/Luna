#include "Libs.h"

namespace Luna
{
    // 2D array of collision detection function pointers
    // This allows for efficient dispatching of collision detection based on shape types
    CollisionCallback Dispatch[Shape::eCount][Shape::eCount] =
    {
        { CircleToCircle, CircleToPolygon },
        { PolygonToCircle, PolygonToPolygon },
    };

    void CircleToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB) // Circle to Circle collision
    {
        // Cast the shapes to circle type for easier access to circle-specific properties
        ShapeCircle* A = (ShapeCircle*)rbA->shape;
        ShapeCircle* B = (ShapeCircle*)rbB->shape;

        // Calculate the vector from center of circle A to center of circle B
        // This vector represents the direction of the potential collision
        Vector2 normal = rbB->position - rbA->position;

        // Compute the squared distance between the circles' centers
        // We use squared distance to avoid unnecessary square root calculations
        real distSqr = normal.LengthSqr();
        
        // Calculate the sum of the radii
        real radius = A->radius + B->radius;

        // Check if the circles are colliding
        // If the squared distance is greater than or equal to the squared sum of radii, no collision
        if (distSqr >= radius * radius)
        {
            m->contactCount = 0; // No collision, so no contact points
            return;
        }

        // At this point, we know the circles are colliding
        // Calculate the actual distance between circle centers
        real dist = std::sqrt(distSqr); // perform actual sqrt
        m->contactCount = 1; // Circle-circle collision always has one contact point

        if (dist == 0.0f) // If the circles are exactly on top of each other
        {
            // The circles are exactly coincident (on top of each other)
            // In this case, we need to choose an arbitrary normal and set the penetration to the radius of A
            m->penetration = A->radius; // Penetration depth
            m->normal = Vector2(1, 0); // Arbitrary normal along x-axis
            m->contacts[0] = rbA->position; // Contact point is at the center of circle A
        }
        else
        {
            // Normal case: circles are overlapping but not coincident
            m->penetration = radius - dist; // Penetration depth
            m->normal = normal / dist; // Normalize the collision normal
            // Calculate the contact point
            // It's located on the surface of circle A in the direction of circle B
            m->contacts[0] = m->normal * A->radius + rbA->position;
        }
    }

    void CircleToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB) // Circle to Polygon collision
    {
        // Cast shapes to their specific types
        ShapeCircle* A = (ShapeCircle*)rbA->shape;
        ShapePolygon* B = (ShapePolygon*)rbB->shape;

        m->contactCount = 0; // Initialize contact count to 0

        // Transform circle center to Polygon model space
        // This simplifies calculations by working in the polygon's local space
        Vector2 center = rbA->position;
        center = B->mat.Transpose() * (center - rbB->position);

        // Find the edge with minimum penetration
        // This is the most likely collision edge
        real separation = -FLT_MAX; // Initialize separation to negative infinity
        size_t faceNormal = 0; // Initialize face normal index to 0

        for (size_t i = 0; i < B->vertex_count; i++)
        {
            // Calculate the distance from the circle center to each edge
            real dist = Dot(B->normals[i], center - B->vertices[i]);

            // If the circle is outside the polygon, no collision
            if (dist > A->radius)
                return;

            // Keep track of the face with the least penetration
            if (dist > separation)
            {
                separation = dist; // Update separation
                faceNormal = i; // Update face normal index
            }
        }

        // Grab face's vertices - we need to find the closest edge
        Vector2 v1 = B->vertices[faceNormal];
        size_t i2 = faceNormal + 1 < B->vertex_count ? faceNormal + 1 : 0;
        Vector2 v2 = B->vertices[i2];

        // Check if the center is within the polygon
        if (separation < EPSILON)
        {
            // The circle's center is inside the polygon
            m->contactCount = 1; // Set contact count to 1
            m->normal = (B->mat * B->normals[faceNormal]); // Set normal to face normal
            m->normal = -m->normal; // Reverse normal
            m->contacts[0] = m->normal * A->radius + rbA->position; // Set contact point
            m->penetration = A->radius; // Set penetration depth
            return;
        }

        // Determine which vertex of the edge is closest to the circle
        real dot1 = Dot(center - v1, v2 - v1);
        real dot2 = Dot(center - v2, v1 - v2);
        m->penetration = A->radius - separation; // Set penetration depth

        // Closest to v1
        if (dot1 <= 0.0f)
        {
            if (DistSqr(center, v1) > A->radius * A->radius)
                return; // No collision

            m->contactCount = 1; // Set contact count to 1
            Vector2 n = v1 - center; // Calculate normal
            n = B->mat * n; // Transform normal to world space
            n.Normalise(); // Normalize normal
            m->normal = n; // Set normal
            v1 = B->mat * v1; // Transform v1 to world space
            v1 += rbB->position; // Transform v1 to world space
            m->contacts[0] = v1; // Set contact point
        }
        else if (dot2 <= 0.0f) // Closest to v2
        {
            if (DistSqr(center, v2) > A->radius * A->radius)
                return; // No collision

            m->contactCount = 1; // Set contact count to 1
            Vector2 n = v2 - center; // Calculate normal
            v2 = B->mat * v2; // Transform v2 to world space
            v2 += rbB->position; // Transform v2 to world space
            m->contacts[0] = v2; // Set contact point
            n = B->mat * n; // Transform normal to world space
            n.Normalise(); // Normalize normal
            m->normal = n; // Set normal
        }
        else // Closest to face
        {
            Vector2 n = B->normals[faceNormal];
            if (Dot((center - v1), n) > A->radius)
                return; // No collision

            n = B->mat * n; // Transform normal to world space
            m->normal = -n; // Set normal
            m->contacts[0] = m->normal * A->radius + rbA->position; // Set contact point
            m->contactCount = 1; // Set contact count to 1
        }
    }

    void PolygonToCircle(Manifold* m, RigidBody* rbA, RigidBody* rbB) // Polygon to Circle collision
    {
        // Reuse CircleToPolygon by swapping A and B, then flipping the normal
        CircleToPolygon(m, rbB, rbA);
        m->normal = (-m->normal);
    }

    void PolygonToPolygon(Manifold* m, RigidBody* rbA, RigidBody* rbB) // Polygon to Polygon collision
    {
        // Cast shapes to their specific types
        ShapePolygon* A = (ShapePolygon*)rbA->shape;
        ShapePolygon* B = (ShapePolygon*)rbB->shape;

        m->contactCount = 0; // Initialize contact count to 0
        
        // Check for a separating axis with A's face planes
        size_t faceA; // Initialize face index for shape A
        real penetrationA = FindAxisLeastPenetration(&faceA, A, B); // Find the axis with the least penetration
        if (penetrationA >= 0.0f)
            return; // No collision

        // Check for a separating axis with B's face planes
        size_t faceB;
        real penetrationB = FindAxisLeastPenetration(&faceB, B, A); // Find the axis with the least penetration
        if (penetrationB >= 0.0f)
            return; // No collision

        size_t referenceIndex; // Determine which shape contains reference face
        bool flip; // Determines whether we need to swap normal direction

        ShapePolygon* RefPoly; // Reference polygon
        ShapePolygon* IncPoly; // Incident polygon

        // Determine which shape features should be used to generate contact points
        if (BiasGreaterThan(penetrationA, penetrationB)) 
        {
            RefPoly = A; // A is reference
            IncPoly = B; // B is incident
            referenceIndex = faceA; // Store reference face index
            flip = false; // Do not flip normal
        }
        else
        {
            RefPoly = B; // B is reference
            IncPoly = A; // A is incident
            referenceIndex = faceB; // Store reference face index
            flip = true; // Flip normal
        }

        // World space incident face
        Vector2 incidentFace[2];
        FindIncidentFace(incidentFace, RefPoly, IncPoly, referenceIndex); // Find the incident face

        // Setup reference face vertices
        Vector2 v1 = RefPoly->vertices[referenceIndex]; // Get reference vertex
        referenceIndex = referenceIndex + 1 == RefPoly->vertex_count ? 0 : referenceIndex + 1; // Get next index
        Vector2 v2 = RefPoly->vertices[referenceIndex]; // Get next vertex

        // Transform vertices to world space
        v1 = RefPoly->mat * v1; 
        v1 += RefPoly->rb->position; 
        v2 = RefPoly->mat * v2; 
        v2 += RefPoly->rb->position;

        // Calculate reference face side normal in world space
        Vector2 sidePlaneNormal = (v2 - v1);
        sidePlaneNormal.Normalise();
         
        // Calculate reference face normal in world space
        Vector2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

        // ax + by = c
        real refC = Dot(refFaceNormal, v1);
        real negSide = (-Dot(sidePlaneNormal, v1));
        real posSide = Dot(sidePlaneNormal, v2);

        // Clip incident face to reference face side planes
        if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
            return; // Due to floating point error, possible to not have required points

        if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
            return; // Due to floating point error, possible to not have required points

        // Flip normal if required
        m->normal = flip ? (-refFaceNormal) : refFaceNormal;

        // Keep points behind reference face
        size_t cp = 0; // clipped points behind reference face
        real separation = Dot(refFaceNormal, incidentFace[0]) - refC;
        if (separation <= 0.0f)
        {
            m->contacts[cp] = incidentFace[0]; // Set contact point
            m->penetration = -separation; // Set penetration depth
            cp++; // Increment contact point count
        }
        else
            m->penetration = 0; // No penetration

        separation = Dot(refFaceNormal, incidentFace[1]) - refC; // Calculate separation
       
        // Keep points behind reference face
        if (separation <= 0.0f)
        {
            m->contacts[cp] = incidentFace[1]; // Set contact point
            m->penetration += (-separation); // Add penetration depth
            cp++; // Increment contact point count
            // Average penetration depth
            m->penetration /= (real)cp; 
        }
        m->contactCount = cp; // Set contact count
    }

    void FindIncidentFace(Vector2* v, ShapePolygon* RefPoly, ShapePolygon* IncPoly, size_t referenceIndex) // Find the incident face 
    {
        // Get the normal of the reference face
        Vector2 referenceNormal = RefPoly->normals[referenceIndex];

        // Transform the reference normal to world space
        referenceNormal = RefPoly->mat * referenceNormal;

        // Transform the reference normal to incident polygon's local space
        referenceNormal = IncPoly->mat.Transpose() * referenceNormal;

        int incidentFace = 0; // Initialize incident face index
        real minDot = FLT_MAX; // Initialize minimum dot product

        // Iterate through all faces of the incident polygon
        for (size_t i = 0; i < IncPoly->vertex_count; i++)
        {
            // Calculate the dot product between the reference normal and each face normal of the incident polygon
            real dot = Dot(referenceNormal, IncPoly->normals[i]);
            
            // We're looking for the face most anti-parallel to the reference face
            // This will be the face with the smallest (most negative) dot product
            if (dot < minDot)
            {
                minDot = dot;
                incidentFace = i;
            }
        }
        // Once we've found the incident face, we need to return its vertices
        // Transform the first vertex of the incident face to world space
        v[0] = IncPoly->mat * IncPoly->vertices[incidentFace];
        v[0] += IncPoly->rb->position;

        // Get the index of the next vertex, wrapping around if necessary
        incidentFace = incidentFace + 1 >= (int)IncPoly->vertex_count ? 0 : incidentFace + 1;

        // Transform the second vertex of the incident face to world space
        v[1] = IncPoly->mat * IncPoly->vertices[incidentFace];
        v[1] += IncPoly->rb->position;
    }

    real FindAxisLeastPenetration(size_t* faceIndex, ShapePolygon* A, ShapePolygon* B) // Find the axis with the least penetration
    {
        real bestDistance = (-FLT_MAX); // Initialize best distance to negative infinity
        size_t bestIndex = 0; // Initialize best index to 0

        // Iterate through all faces of polygon A
        for (size_t i = 0; i < A->vertex_count; i++)
        {
            // Get the normal of the current face
            Vector2 n = A->normals[i];
           
            // Transform the normal to world space
            Vector2 nw = A->mat * n;

            // Get the transpose of B's transformation matrix
            Mat2 buT = B->mat.Transpose();

            // Transform the world space normal to B's local space
            n = buT * nw;

            // Find the support point of B in the opposite direction of n
            Vector2 s = B->GetBest(-n);

            // Get the vertex of the current face of A
            Vector2 v = A->vertices[i];
            
            // Transform the vertex to world space
            v = A->mat * v;
            v += A->rb->position;
            
            // Transform the vertex to B's local space
            v -= B->rb->position;
            v = buT * v;

            // Calculate the distance between the vertex and the support point
            real d = Dot(n, s - v);

            // Keep track of the face with the largest distance (least penetration)
            if (d > bestDistance)
            {
                bestDistance = d; // Update best distance
                bestIndex = i; // Update best index
            }
        }
        // Store the index of the face with the least penetration
        *faceIndex = bestIndex;
        // return the penetration depth
        return bestDistance;
    }

    int Clip(Vector2 n, real c, Vector2* face) // Clip the polygon to the line
    {
        size_t sp = 0; // Initialize the number of clipped points to 0
        Vector2 tmp = { 0,0 }; // Initialize temporary vector
        Vector2 out[2] = { face[0], face[1] }; // Initialize output array

        // Calculate the distance of each vertex from the line
        real d1 = Dot(n, face[0]) - c;
        real d2 = Dot(n, face[1]) - c;

        // If a vertex is behind the line, keeo it
        if (d1 <= 0.0f) out[sp++] = face[0];
        if (d2 <= 0.0f) out[sp++] = face[1];

        // If the vertices are on different sides of the line
        if (d1 * d2 < 0.0f)
        {
            // Calculate the intersection point
            real alpha = d1 / (d1 - d2);
            tmp = (face[1] - face[0]);

            // If we have not already reached kept both points
            if (sp < 2)
            {
                // Add the intersection point
                out[sp] = tmp * alpha + face[0];
                sp++;
            }
        }
        // Update the input face with the clipped points
        face[0] = out[0];
        face[1] = out[1];

        // Ensure we have not somehow ended up with more than 2 points
        assert(sp != 3);

        // Return the number points left after clipping
        return sp;
    }
}