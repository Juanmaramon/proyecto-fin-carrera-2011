// Clase que representa el Protagonista del videojuego (Mustang custom)

#ifndef TRUCK_H
#define TRUCK_H

#include "Vehicle.h"
#include "..\..\Physics\cPhysicObject.h"
#include "..\Explosion\Explosion.h"
#include "..\StateConstants.h"

class Truck{

public:
	void Init(cVec3 ini_pos, cObject* truckExterior, cObject* truckWeapon, cObject* truckTires,
		 cResourceHandle* weapon_muzzle1, cResourceHandle* weapon_muzzle2, cResourceHandle* weapon_muzzle3,
		 cObject* truckExteriorDestroyed, cObject* truckWeaponDestroyed, cResourceHandle* explosion_sprite,
		 cResourceHandle* explosion_sprite1, cResourceHandle* particle_texture);
	void Deinit();
	void MoveForward(float lfTimestep);
	void Break(float lfTimestep);
	void SteeringLeft(float lfTimestep);
	void SteeringRight(float lfTimestep);
	void Update(float lfTimestep);
	Vehicle* GetVehicleBullet() { return &mVehicle; }
	void Render();
	bool IsAlive();
	void Damage();
	void RenderMuzzle();
	void RenderRayGun ();

private: 
	// Modelo físico del vehiculo 
	Vehicle mVehicle;
	// Modelos 3d de las distintas partes del vehiculo
	cObject* mTruckExt;
	cObject* mTruckInt;
	cObject* mTruckWea;
	cObject* mTruckExtDes;
	cObject* mTruckWeaDes;
	cMatrix mScaleMatrix, lScaleMatrixChasis;
	std::vector<cObject> maTires;
	float mvPreviousYaw;
	// Contandor de vidas
	int miCurrentLives;
	// Estado del enemigo
	eStates mState;

	cResourceHandle* mWeaponMuzzle1;
	cResourceHandle* mWeaponMuzzle2;
	cResourceHandle* mWeaponMuzzle3;
	bool mbIsFiring, mbHit, mbLaserHit, mbIsCamAux;
	// Punto en el que choca el rayo del arma 
	cVec3 mvRayHitPosition, mvRayLaserHitPosition;
	// Punto final del rayo que simula la trayectoria de la bala
	cVec3 mvEnd;
	// Secuencia de animacion de fogonazo
	int miFlashSeq;
	float mfEnemyArrow;

	// Explosion
	Explosion mExplosion;

};

#endif