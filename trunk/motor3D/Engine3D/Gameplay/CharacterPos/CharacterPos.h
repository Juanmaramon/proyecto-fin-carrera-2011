#ifndef CHARACTERPOS_H
#define CHARACTERPOS_H

#include "..\..\Utility\Singleton.h"
#include "..\..\MathLib\MathLib.h"

class CharacterPos : public cSingleton<CharacterPos>{
private:
	// Character position
	cVec3 mPosition;
	// Character angle rotation
	float mYaw;
	// Position or direction changed?
	bool mPosDirChange;
	// Velocity of character
	int mVelocity;
public:
	void Init(cVec3 lPosIni, float lYawIni, int lVel);
	void DeInit(void);
	cVec3 GetCharacterPosition(void){ return mPosition; }
	float GetYaw(void){ return mYaw; }
	int GetVelocity(void){ return mVelocity; }
	bool IsChanged(void){ return mPosDirChange; }
	void SetChanged(void){ mPosDirChange = false; }
	void SetCharacterPosition(cVec3 lPos);
	void SetCharacterYaw(float lYaw);
	void SetVelocity(int lvel);
};

#endif