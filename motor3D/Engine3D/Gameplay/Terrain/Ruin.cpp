#include "Ruin.h"


void Ruin::Init(cObject* ruin, cMatrix position){
	mRuinObj = ruin;
	mRuinPosition = position;
	// Redimensionamiento de los modelos 3d
	mScaleMatrix.LoadScale(.3f);

	mBoundingBox.InitBox( 0.0f, cVec3(5.f, 2.8f, 1.5f) );
	mBBoxObject.SetWorldMatrix( position );
	mBBoxObject.SetScaleMatrix( mScaleMatrix );
	mBBoxObject.CreatePhysics( &mBoundingBox );
}

void Ruin::Update(float lfTimestep) {

	mBBoxObject.Update(lfTimestep);

}

void Ruin::Render(){
//	mBBoxObject.Render();

	mRuinObj->SetWorldMatrix(mScaleMatrix * mRuinPosition);

	mRuinObj->Render();

}