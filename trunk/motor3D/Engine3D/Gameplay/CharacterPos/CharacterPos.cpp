#include "CharacterPos.h"

void CharacterPos::Init(cVec3 lPosIni, float lYawIni, int lVel, float lAngVel){
	mPosition = lPosIni;
	mYaw = lYawIni;
	mVelocity = lVel;
	mAngVel = lAngVel;
	mPosDirChange = false;
}

void CharacterPos::DeInit(void){}

void CharacterPos::SetCharacterPosition(cVec3 lPos){
	mPosition = lPos;
	mPosDirChange = true;
}

void CharacterPos::SetCharacterYaw(float lYaw){
	mYaw = lYaw;
	mPosDirChange = true;
}

void CharacterPos::SetVelocity(int lVel){
	mVelocity = lVel;
}

void CharacterPos::MoveFront(void){
	mPosition += cVec3( 0.0f, 0.0f, -mVelocity );
	mPosDirChange = true;
}

void CharacterPos::MoveBack(void){
	mPosition += cVec3( 0.0f, 0.0f, mVelocity );
	mPosDirChange = true;
}

void CharacterPos::StrafeRight(void){
	mYaw -= mAngVel; 	
}

void CharacterPos::StrafeLeft(void){
	mYaw += mAngVel; 	
}