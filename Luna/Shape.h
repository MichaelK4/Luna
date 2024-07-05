#ifndef SHAPE_H
#define SHAPE_H
#include "RigidBody.h"
#define MaxPolyCount 128

namespace Luna
{
	struct Shape
	{
		enum ShapeType // Enum for the shape type
		{
			eCircle,
			ePolygon,
			eCount
		};

		RigidBody* rb; 
		 
		Shape() { } // Default constructor

		virtual Shape* Clone(RigidBody* rb) const = 0 { rb = rb; } // Clone function to create a new shape
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
		ShapeCircle(real r)
		{
			radius = r;
		}

		Shape* Clone(RigidBody* rb) const override
		{
			return new ShapeCircle(radius);
		}

		void Init(RigidBody* rb)
		{
			ComputeMass(1.0f, rb);
		}

		void ComputeMass(real density, RigidBody* rb)
		{
			rb->Mass = PI * radius * radius * density;
			rb->InverseMass = (rb->Mass) ? 1.0f / rb->Mass : 0.0f;
			rb->Inertia = rb->Mass * radius * radius;
			rb->InverseInertia = (rb->Inertia) ? 1.0f / rb->Inertia : 0.0f;
		}

		void SetOrient(real radians)
		{
			// do nothing - a circle is symmetrical
		}

		void Draw(RigidBody* rb) override
		{
			const size_t k_segments = 20; 

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

		ShapeType GetType() const override
		{
			return eCircle;
		}
	};

	struct ShapePolygon : public Shape
	{
		size_t vertex_count;
		Vector2 vertices[MaxPolyCount];
		Vector2 normals[MaxPolyCount];

		Shape* Clone(RigidBody* rb) const override
		{
			ShapePolygon* poly = new ShapePolygon();
			poly->mat = mat;
			for (size_t i = 0; i < vertex_count; i++)
			{
				poly->vertices[i] = vertices[i];
				poly->normals[i] = normals[i];
			}
			poly->vertex_count = vertex_count;
			return poly;
		}

		void Init(RigidBody* rb) override
		{
			ComputeMass(1.0f, rb);
		}

		void ComputeMass(real density, RigidBody* rb) override
		{
			Vector2 centroid(0.0f, 0.0f);
			real area = 0.0f;
			real I = 0.0f;
			const real k_inv3 = 1.0f / 3.0f;

			for (size_t i = 0; i < vertex_count; i++)
			{
				Vector2 p(vertices[i]);
				size_t next = i + 1 < vertex_count ? i + 1 : 0;
				Vector2 q(vertices[next]);

				real D = Cross(p, q);
				real triangleArea = 0.5f * D;

				area += triangleArea;

				centroid += (p + q) * triangleArea * k_inv3;

				real intx = p.x * p.x + q.x * p.x + q.x * q.x;
				real inty = p.y * p.y + q.y * p.y + q.y * q.y;
				I += (0.25f * k_inv3 * D) * (intx + inty);
			}

			centroid *= 1.0f / area;

			for (size_t i = 0; i < vertex_count; i++)
				vertices[i] -= centroid;

			rb->Mass = density * area;
			rb->InverseMass = (rb->Mass) ? 1.0f / rb->Mass : 0.0f;
			rb->Inertia = I * density;
			rb->InverseInertia = rb->Inertia ? 1.0f / rb->Inertia : 0.0f;
		}

		void SetOrient(real radians) override
		{
			mat.Set(radians);
		}

		void Draw(RigidBody* rb) override
		{

			glColor4f(rb->r, rb->g, rb->b, 0.5f); 
			glBegin(GL_POLYGON);
				for (size_t i = 0; i < vertex_count; i++)
				{
					Vector2 v = rb->position + mat * vertices[i];
					glVertex2f(v.x, v.y);
				}
			glEnd();

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

		ShapeType GetType() const
		{
			return ePolygon;
		}

		void SetBox(real hw, real hh)
		{
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
			assert(count > 2 && count < MaxPolyCount);
			count = std::min((size_t)MaxPolyCount, count);

			// Find the right most point on the hull
			int rightMost = 0;
			real highestXCoord = vec[0].x;
			for (size_t i = 1; i < count; i++)
			{
				real x = vec[i].x;
				if (x > highestXCoord)
				{
					highestXCoord = x;
					rightMost = i;
				}
				else if (x == highestXCoord)
				{
					if (vec[i].y < vec[rightMost].y)
						rightMost = i;
				}
			}

			int hull[MaxPolyCount]; // hull is a list of point indexes
			int outCount = 0;
			int indexHull = rightMost;

			for (;;)
			{
				hull[outCount] = indexHull;
				int nextHullIndex = 0;
				for (int i = 1; i < (int)count; i++)
				{
					if (nextHullIndex == indexHull)
					{
						nextHullIndex = i;
						continue;
					}

					Vector2 e1 = vec[nextHullIndex] - vec[hull[outCount]];
					Vector2 e2 = vec[i] - vec[hull[outCount]];
					real c = Cross(e1, e2);
					if (c < 0.0f)
						nextHullIndex = i;

					if (c == 0.0f && e2.LengthSqr() > e1.LengthSqr())
						nextHullIndex = i;
				}

				outCount++;
				indexHull = nextHullIndex;

				if (nextHullIndex == rightMost)
				{
					vertex_count = outCount;
					break;
				}
			}

			for (size_t i = 0; i < vertex_count; i++)
				vertices[i] = vec[hull[i]];

			for (size_t i = 0; i < vertex_count; i++)
			{
				size_t i2 = i + 1 < vertex_count ? i + 1 : 0;
				Vector2 face = vertices[i2] - vertices[i];

				assert(face.LengthSqr() > EPSILON * EPSILON);

				normals[i] = Vector2(face.y, -face.x);
				normals[i].Normalise();
			}
		}

		Vector2 GetBest(const Vector2& dir)
		{
			real bestProjection = -FLT_MAX;
			Vector2 bestVertex;

			for (size_t i = 0; i < vertex_count; i++)
			{
				Vector2 v = vertices[i];
				real projection = Dot(v, dir);

				if (projection > bestProjection)
				{
					bestVertex = v;
					bestProjection = projection;
				}
			}
			return bestVertex;
		}
	};
}

#endif // !SHAPE_H
