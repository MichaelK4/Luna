#ifndef SHAPE_H
#define SHAPE_H
#include "RigidBody.h"
#define MaxPolyCount 128

namespace Luna
{
	struct Shape
	{
		// Enum for the shape type
		enum ShapeType { eCircle, ePolygon, eCount };

		RigidBody* rb; // Pointer to the rigid body

		Shape() { } // Default constructor
		virtual ~Shape() { } // Destructor 

		virtual Shape* Clone(RigidBody* rb) const = 0 { } // Clone function to create a new shape
		virtual void Init(RigidBody* rb) = 0; // Initialize the shape
		virtual void ComputeMass(real density, RigidBody* rb) = 0; // Compute the mass of the shape
		virtual void SetOrient(real radians) = 0; // Set the orientation of the shape
		virtual void Draw(RigidBody* rb) = 0; // Draw the shape
		virtual ShapeType GetType() const = 0; // Get the type of the shape

		real radius; // Radius of the shape

		Mat2 mat; // Matrix for the shape
	};

	struct ShapeCircle : public Shape
	{
		ShapeCircle(real r) { radius = r; }
		~ShapeCircle() { }

		// create a new ShapeCircle with the same radius
		Shape* Clone(RigidBody* rb) const override { return new ShapeCircle(radius); }

		// initialize the shape by computing the mass properties
		void Init(RigidBody* rb) { ComputeMass(1.0f, rb); }

		void ComputeMass(real density, RigidBody* rb)
		{
			// compute mass properties for a circle
			rb->Mass = PI * radius * radius * density; // set the mass
			rb->InverseMass = (rb->Mass) ? 1.0f / rb->Mass : 0.0f; // set the inverse mass
			rb->Inertia = rb->Mass * radius * radius; // set the moment of inertia
			rb->InverseInertia = (rb->Inertia) ? 1.0f / rb->Inertia : 0.0f; // set the inverse moment of inertia
		}

		void SetOrient(real radians) { /* do nothing - a circle is symmetrical*/ }

		void Draw(RigidBody* rb) override
		{
			const size_t k_segments = 20; 

			// Draw the circle
			glColor4f(rb->r, rb->g, rb->b, 0.5f);
			glBegin(GL_TRIANGLE_FAN);
				real theta = rb->Orient;
				real increment = PI * 2.0f / real(k_segments);
				for (size_t i = 0; i < k_segments; i++) 
				{
					theta += increment;
					Vector2 p(cos(theta), sin(theta));
					p *= radius;
					p += rb->position;
					glVertex2f(p.x, p.y);
				}
			glEnd();

			// Draw the outline of the circle
			glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
			glLineWidth(2.0f);
			glBegin(GL_LINE_LOOP);
				theta = rb->Orient;
				increment = PI * 2.0f / real(k_segments);
				for (size_t i = 0; i < k_segments; i++)
				{
					theta += increment;
					Vector2 p(cos(theta), sin(theta));
					p *= radius;
					p += rb->position;
					glVertex2f(p.x, p.y);
				}
			glEnd();

			// Draw line to show rotation
			glLineWidth(4.0f);
			glBegin(GL_LINE_STRIP);
				glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
				Vector2 r(0, 1.0f);
				real c = cos(rb->Orient);
				real s = sin(rb->Orient);
				r.Set(r.x * c - r.y * s, r.x * s + r.y * c);
				r *= radius;
				r += rb->position;
				glVertex2f(rb->position.x, rb->position.y);
				glVertex2f(r.x, r.y);
			glEnd();
		}

		// get the type of the shape
		ShapeType GetType() const override { return eCircle; }
	};

	struct ShapePolygon : public Shape
	{
		size_t vertex_count; // number of vertices
		Vector2 vertices[MaxPolyCount]; // vertices of the polygon
		Vector2 normals[MaxPolyCount]; // normals of the polygon

		ShapePolygon() { vertex_count = 0; }
		~ShapePolygon() { }

		Shape* Clone(RigidBody* rb) const override
		{
			// create a new polygon with the same properties
			ShapePolygon* poly = new ShapePolygon();
			poly->mat = mat; // set the matrix

			// copy the vertices and normals
			for (size_t i = 0; i < vertex_count; i++)
			{
				poly->vertices[i] = vertices[i]; // copy the vertices
				poly->normals[i] = normals[i]; // copy the normals
			}
			poly->vertex_count = vertex_count; // set the number of vertices
			// return the new polygon
			return poly;
		}

		// initialize the shape by computing the mass properties
		void Init(RigidBody* rb) override { ComputeMass(1.0f, rb); }

		void ComputeMass(real density, RigidBody* rb) override
		{
			// Initialize the variables for centroid calaculation and area
			Vector2 centroid(0.0f, 0.0f); // set the centroid to the zero vector
			real area = 0.0f; // set the area to zero
			real I = 0.0f; // set the moment of inertia to zero
			const real k_inv3 = 1.0f / 3.0f; // inverse of 3

			// iterate thjrough all triangles formed by the polygon
			for (size_t i = 0; i < vertex_count; i++)
			{
				// get current vertex and next vertex (wrapping around to 0 if necessary)
				Vector2 p(vertices[i]); // get the current vertex
				size_t next = i + 1 < vertex_count ? i + 1 : 0; // get the next vertex via wrapping around
				Vector2 q(vertices[next]); // get the next vertex 

				// Calculate the cross product of p and q
				real D = Cross(p, q);
				// calculate the area of this triangle
				real triangleArea = 0.5f * D;

				// add to the total area
				area += triangleArea;

				// accumulate weighted centroid
				// the cetroid of a triangle is at (p + q) / 3
				centroid += (p + q) * triangleArea * k_inv3;

				// calculate integrals for moment of inertia
				real intx = p.x * p.x + q.x * p.x + q.x * q.x; // calculate the x-integral
				real inty = p.y * p.y + q.y * p.y + q.y * q.y; // calculate the y-integral
				I += (0.25f * k_inv3 * D) * (intx + inty); // calculate the moment of inertia 
			}

			// finalize the centroid calculation
			centroid *= 1.0f / area;

			// move vertices to be centered around centroid
			for (size_t i = 0; i < vertex_count; i++)
				vertices[i] -= centroid;

			// set mass properties
			rb->Mass = density * area; // set the mass
			rb->InverseMass = (rb->Mass) ? 1.0f / rb->Mass : 0.0f; // set the inverse mass
			rb->Inertia = I * density; // set the moment of inertia
			rb->InverseInertia = rb->Inertia ? 1.0f / rb->Inertia : 0.0f; // set the inverse moment of inertia
		}

		// set the orientation of the shape
		void SetOrient(real radians) override { mat.Set(radians); }

		void Draw(RigidBody* rb) override
		{
			// Draw the polygon
			glColor4f(rb->r, rb->g, rb->b, 0.5f); 
			glBegin(GL_POLYGON);
				for (size_t i = 0; i < vertex_count; i++)
				{
					Vector2 v = rb->position + mat * vertices[i];
					glVertex2f(v.x, v.y);
				}
			glEnd();

			// Draw the outline of the polygon
			glLineWidth(2.0f); 
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f); // White color for the outline
			glBegin(GL_LINE_LOOP); 
				for (size_t i = 0; i < vertex_count; i++)
				{
					Vector2 v = rb->position + mat * vertices[i];  
					glVertex2f(v.x, v.y); 
				} 
			glEnd(); 
		}

		// get the type of the shape
		ShapeType GetType() const { return ePolygon; }

		void SetBox(real hw, real hh)
		{
			// set vertices and normals for a box shape
			vertex_count = 4;

			vertices[0].Set(-hw, -hh); 
			vertices[1].Set(hw, -hh); 
			vertices[2].Set(hw, hh);
			vertices[3].Set(-hw, hh);

			normals[0].Set(0.0f, -1.0f);
			normals[1].Set(1.0f, 0.0f);
			normals[2].Set(0.0f, 1.0f);
			normals[3].Set(-1.0f, 0.0f);
		}

		void Set(Vector2* vec, size_t count)
		{
			// Ensure we have at least 3 points and not more than MaxPolyCount
			assert(count > 2 && count < MaxPolyCount);
			count = std::min((size_t)MaxPolyCount, count);

			// Find the right most point on the hull
			int rightMost = 0; // start from the first point
			real highestXCoord = vec[0].x; // set the highest x-coord to the first point
			for (size_t i = 1; i < count; i++)
			{
				real x = vec[i].x;
				if (x > highestXCoord) // if x-coord is higher, choose this point
				{
					highestXCoord = x; // set the new highest x-coord
					rightMost = i; // choose this point
				}
				else if (x == highestXCoord) // if x-coords are equal, choose the lower point
					if (vec[i].y < vec[rightMost].y) // if y-coord is lower, choose this point
						rightMost = i; // choose this point
			}

			int hull[MaxPolyCount]; // hull is a list of point indexes
			int outCount = 0; // outCount is the number of points in the hull
			int indexHull = rightMost; // start from the rightmost point

			// graham scan to create a convex hull
			// grahm scan is a method of finding the convex hull of a finite set of points in the plane with time complexity O(n log n)
			for (;;)
			{
				hull[outCount] = indexHull; // add point to hull
				// find next index for the hull
				int nextHullIndex = 0; // start from the first point
				for (int i = 1; i < (int)count; i++)
				{
					// skip if it's the same as current index
					if (nextHullIndex == indexHull)
					{ 
						nextHullIndex = i; // choose this point
						continue; // skip this iteration
					}

					// Create vectors from current hull point to next potential hull point
					Vector2 e1 = vec[nextHullIndex] - vec[hull[outCount]]; // vector from current hull point to next potential hull point
					Vector2 e2 = vec[i] - vec[hull[outCount]]; // vector from current hull point to next point
					real c = Cross(e1, e2); // cross product of the two vectors

					// If c < 0, it's a right turn, so update nextHullIndex
					if (c < 0.0f) 
						nextHullIndex = i; // choose this point

					// If vectors are collinear, choose the further point
					if (c == 0.0f && e2.LengthSqr() > e1.LengthSqr())
						nextHullIndex = i; // choose this point
				}

				outCount++; // increment the number of points in the hull
				indexHull = nextHullIndex; // set the next hull index

				// Conclude the algorithm upon wrap-around
				if (nextHullIndex == rightMost) // if the next hull index is the rightmost point, break the loop
				{
					vertex_count = outCount; // set the number of points in the hull
					break; 
				}
			}

			// Copy vertices from hull to polygon
			for (size_t i = 0; i < vertex_count; i++)
				vertices[i] = vec[hull[i]]; 

			// Compute face normals
			for (size_t i = 0; i < vertex_count; i++)
			{
				size_t i2 = i + 1 < vertex_count ? i + 1 : 0; // get the next point
				Vector2 face = vertices[i2] - vertices[i]; // get the face vector

				// Ensure the face has no zero-length edges
				assert(face.LengthSqr() > EPSILON * EPSILON);

				// Calculate normal and normalize it
				normals[i] = Vector2(face.y, -face.x); // set the normal
				normals[i].Normalise(); // normalize the normal
			}
		}

		// Get the best vertex in the polygon in a given direction
		Vector2 GetBest(const Vector2& dir)
		{
			// Initialize the best projection to negative infinity
			real bestProjection = -FLT_MAX; 
			Vector2 bestVertex; // set the best vertex to the zero vector

			// Iterate through all vertices of the polygon
			for (size_t i = 0; i < vertex_count; i++)
			{
				// get the current vertex
				Vector2 v = vertices[i];

				// Calculate the dot product of the vertex and the direction
				// This gives us the projection of the vertex onto the direction vector
				real projection = Dot(v, dir);

				// If this projection is greater than our current best projection
				if (projection > bestProjection)
				{
					// Set the best projection to the current projection
					bestVertex = v;
					bestProjection = projection;
				}
			}
			// Return the vertex that had the largest projection in the given direction
			return bestVertex;
		}
	};
}

#endif // !SHAPE_H