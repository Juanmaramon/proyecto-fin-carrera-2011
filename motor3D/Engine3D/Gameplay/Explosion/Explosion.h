#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "../../Utility/ResourceHandle.h"
#include "../../Mathlib/Mathlib.h"

using namespace std;

#define EXPLOSION_MAX_STEPS 1

//Represents a single particle.
struct Particle {
	cVec3 pos;
	cVec3 velocity;
	cVec3 color;
	float timeAlive; //The amount of time that this particle has been alive.
	float lifespan;  //The total amount of time that this particle is to live.
};

//#define PI  3.1415926535f
const float GRAVITY = 2.0f;//3.0f;
const int NUM_PARTICLES = 1500;//1500;
//The interval of time, in seconds, by which the particle engine periodically
//steps.
const float STEP_TIME = 0.01f;
//The length of the sides of the quadrilateral drawn for each particle.
const float PARTICLE_SIZE = 0.18f; //0.2f;//0.05f;
const int TIMER_MS = 25;

class Explosion{

public:

	void Init(cResourceHandle* explosion_sprite, cResourceHandle* explosion_sprite1, cResourceHandle* flash, cResourceHandle* particleTexture);
	void Render();
	void Deinit();
	void SetPosition (cMatrix position);
	void SetAlive (bool  alive_state) { mAlive = alive_state; }
	bool IsAlive () { return mAlive; }

	// Sist. particulas
	void RenderParticles();
	void UpdateParticles(float lfTimestep);
	void InitParticles(cResourceHandle* textureParticle);
	void StepParticles();
	void CreateParticle(Particle* p);
	cVec3 CurVelocity();
	void DeinitParticles();
	void SetParticlesPosition(cMatrix position) { 		cMatrix lmPosCorrection; lmPosCorrection.LoadTranslation(cVec3(0.f, 0.3f, -0.8f)); mvParticleSysPos = position * lmPosCorrection; };
	void RenderSmoke();

private:
	
	cResourceHandle* mExplosion_sprite, *mExplosion_sprite1, *mExplosion_flash, *mTextureParticle;
	int miExplosionSeq;
	int miExplosionStep;
	float xo,yo,xf,yf, deviation_x, deviation_z;
	cMatrix mTranslate;
	// Si la explosion tiene vigencia
	bool mAlive;
	float mfAngle, lfColorTime, lfTimeUntilNextStep;

	// Sistema de particulas
	Particle particles[NUM_PARTICLES];
	cMatrix mvParticleSysPos;

};

#endif

