#include "CharacterPos.h"

void CharacterPos::Init(cVec3 lPosIni, float lYawIni, int lVel){
	mPosition = lPosIni;
	mYaw = lYawIni;
	mVelocity = lVel;
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