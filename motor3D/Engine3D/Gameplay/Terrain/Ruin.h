#ifndef RUIN_H
#define RUIN_H

#include "../../Mathlib/Mathlib.h"
#include "..\..\Graphics\GraphicManager.h"
#include "..\..\Physics\cPhysicObject.h"

class Ruin{

public:
	void Init(cObject* ruin, cMatrix position);
	void Update(float lfTimestep);
	void Render();

private:
	cMatrix mRuinPosition;
	cObject* mRuinObj;
	cMatrix mScaleMatrix;
	cPhysicModel mBoundingBox;
	cPhysicObject mBBoxObject;
};

#endif