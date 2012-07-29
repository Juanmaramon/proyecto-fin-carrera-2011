// Clase que representa el Protagonista del videojuego (Mustang custom)

#ifndef MUSTANG_H
#define MUSTANG_H

#include "Vehicle.h"
#include "..\..\Physics\cPhysicObject.h"
#include "..\Bullet\Bullet.h" 
#include "..\Explosion\Explosion.h"
#include "..\StateConstants.h"

#define WEAPON_RANGE 100000.f
#define FRAME_DELAY 4

class Mustang{

public:
	void Init(cObject* mustangExterior, cObject* mustangInterior, cObject* mustangWeapon, cObject* mustangTires, 
			  cResourceHandle* weapon_muzzle1, cResourceHandle* weapon_muzzle2, cResourceHandle* weapon_muzzle3, 
			  cResourceHandle* arrow_enemy, cResourceHandle* mPositiveAmmunition, cObject* mustangExteriorDestroyed,
				   cObject* mustangInteriorDestroyed, cResourceHandle* explosion_sprite,
				 cResourceHandle* explosion_sprite1, cResourceHandle* particle_texture,
				 cResourceHandle* crosshair);
	void Deinit();
	void MoveForward(float lfTimestep);
	void Break(float lfTimestep);
	void SteeringLeft(float lfTimestep);
	void SteeringRight(float lfTimestep);
	void Update(float lfTimestep, float lfYaw = 0.0f, float lfPitch = 0.0f, bool lbIsCameraAux = false, bool lbFireMainWeapon = false);
	Vehicle* GetVehicleBullet() { return &mVehicle; }
	void Render();
	void RenderArrowEnemy ();
	void RenderMuzzle();
	void RenderHUD ();
	bool IsAlive();
	void Damage();
	int GetCurrentLives() { return miCurrentLives; };
	void Crosshair();
private: 
	void RenderRayGun ();

	// Modelo físico del vehiculo 
	Vehicle mVehicle;
	// Modelos 3d de las distintas partes del vehiculo
	cObject* mMusExt;
	cObject* mMusInt;
	cObject* mMusWea;
	cMatrix mScaleMatrix, lScaleMatrixChasis;
	std::vector<cObject> maTires;
	// Para implementar la inercia de la torreta
	float mfPreviousYaw;
	// Esta disparando el protagonista?
	bool mbIsFiring, mbHit, mbLaserHit, mbIsCamAux;
	// Punto en el que choca el rayo del arma 
	cVec3 mvRayHitPosition, mvRayLaserHitPosition;
	Bullet mBullets;
	// Punto final del rayo que simula la trayectoria de la bala
	cVec3 mvEnd;
	cResourceHandle* mWeaponMuzzle1;
	cResourceHandle* mWeaponMuzzle2;
	cResourceHandle* mWeaponMuzzle3;
	cResourceHandle* mArrowEnemy;
	cResourceHandle* mPosAmmu;
	// Secuencia de animacion de fogonazo
	int miFlashSeq;
	float mfEnemyArrow;
	// Contador de vidas
	int miCurrentLives;

	cObject* mMusExtDes, *mMusIntDes; 

	// Explosion
	Explosion mExplosion;

	// Estado del protagonista
	eStates mState;

	// Mirilla
	cResourceHandle* mCrosshair;
	float mYaw, mPitch, mCHRotationCount;

};

#endif