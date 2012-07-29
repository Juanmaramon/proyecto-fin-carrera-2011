#ifndef BUSH_H
#define BUSH_H

#include "../../Mathlib/Mathlib.h"
#include "..\..\Graphics\GraphicManager.h"
#include "..\..\Physics\cPhysicObject.h"

class Bush{

public:
	void Init(cResourceHandle* bush, cResourceHandle* bushMask, cMatrix position);
	void Update(float lfTimestep);
	void Render();

private:
	cMatrix mBushPosition;
	cResourceHandle* mBushTexture, *mBushTextureMask;

};

#endif