#ifndef BODY_SHAPE_H
#define BODY_SHAPE_H
#include "Libs.h"
class BodyShape
{
private:
	RigidBody* rb;
	Mat2 m2;
public:
	BodyShape();
	~BodyShape();

	virtual void InitBody(int shape);
	virtual void CalculateBody_Mass(real density);

	int getBodyShape();
};

#endif // !BODY_SHAPE_H



