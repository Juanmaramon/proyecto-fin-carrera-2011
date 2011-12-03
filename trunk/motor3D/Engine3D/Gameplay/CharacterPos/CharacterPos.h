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
	// Angular velocity of character
	float mAngVel;
public:
	void Init(cVec3 lPosIni, float lYawIni, int lVel, float lAngVel);
	void DeInit(void);
	cVec3 GetCharacterPosition(void){ return mPosition; }
	float GetYaw(void){ return mYaw; }
	int GetVelocity(void){ return mVelocity; }
	float GetAngVelocity(void){ return mAngVel; }
	bool IsChanged(void){ return mPosDirChange; }
	void ResetChanged(void){ mPosDirChange = false; }
	void SetCharacterPosition(cVec3 lPos);
	void SetCharacterYaw(float lYaw);
	void SetVelocity(int lvel);
	void MoveFront(void);
	void MoveBack(void);
	void StrafeRight(void);
	void StrafeLeft(void);
	//Devuelve el vector Front del personaje.
	inline cVec3 GetFront() const { return  cVec3(   sinf(mYaw),  0.0f, cosf(mYaw) ); }
};

#endif