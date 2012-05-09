// Clase que representa el Protagonista del videojuego (Mustang custom)

#ifndef MUSTANG_H
#define MUSTANG_H

#include "Vehicle.h"
#include "..\..\Physics\cPhysicObject.h"

class Mustang{

public:
	void Init(cObject* mustangExterior, cObject* mustangInterior, cObject* mustangWeapon, cObject* mustangTires);
	void Deinit();
	void MoveForward(float lfTimestep);
	void Break(float lfTimestep);
	void SteeringLeft(float lfTimestep);
	void SteeringRight(float lfTimestep);
	void Update(float lfYaw = 0.0f, float lfPitch = 0.0f);
	Vehicle* GetVehicleBullet() { return &mVehicle; }
	void Mustang::Render();

private: 
	// Modelo físico del vehiculo 
	Vehicle mVehicle;
	// Modelos 3d de las distintas partes del vehiculo
	cObject* mMusExt;
	cObject* mMusInt;
	cObject* mMusWea;
	cMatrix mScaleMatrix, lScaleMatrixChasis;
	std::vector<cObject*> maTires;
	// Resolucion vertical y horizontal
	unsigned muiVerticalRes, muiHorizontalRes;
	// Rotacion del arma
	float mfWeaponYaw;
};

#endif