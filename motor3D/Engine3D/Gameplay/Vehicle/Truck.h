// Clase que representa el Protagonista del videojuego (Mustang custom)

#ifndef TRUCK_H
#define TRUCK_H

#include "Vehicle.h"
#include "..\..\Physics\cPhysicObject.h"

class Truck{

public:
	void Init(cObject* truckExterior, cObject* truckWeapon, cObject* truckTires);
	void Deinit();
	void MoveForward(float lfTimestep);
	void Break(float lfTimestep);
	void SteeringLeft(float lfTimestep);
	void SteeringRight(float lfTimestep);
	void Update(float lfYaw = 0.0f, float lfPitch = 0.0f);
	Vehicle* GetVehicleBullet() { return &mVehicle; }
	void Render();

private: 
	// Modelo físico del vehiculo 
	Vehicle mVehicle;
	// Modelos 3d de las distintas partes del vehiculo
	cObject* mTruckExt;
	cObject* mTruckInt;
	cObject* mTruckWea;
	cMatrix mScaleMatrix, lScaleMatrixChasis;
	std::vector<cObject> maTires;
	float mvPreviousYaw;
};

#endif