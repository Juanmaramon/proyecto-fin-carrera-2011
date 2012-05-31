/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#ifndef VEHICLE_H
#define VEHICLE_H

#include "../../Libraries/Bullet/include/BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "../../Mathlib/Mathlib.h"
#include "../../Libraries/Bullet/include/BulletCollision/CollisionShapes/btShapeHull.h"
#include <string>
using namespace std;

#define PHYSCAR_SCALE 1
#define CUBE_HALF_EXTENTS 1

///Vehicle use the built-in raycast vehicle
class Vehicle{

public:

	btRigidBody* m_carChassis;
	btRaycastVehicle*	m_vehicle;
	btCollisionShape*	m_wheelShape;
	btVehicleRaycaster*	m_vehicleRayCaster;
	btRaycastVehicle::btVehicleTuning	m_tuning;
	class btCompoundShape* m_compound;
	btCollisionShape* m_chassisShape;

	Vehicle();
	~Vehicle(void);
	void initPhysics(string lsType);
	void ResetVehicleParams();
	void renderme();
	void Update();
	void MoveForward(float lfTimestep);
	void Break(float lfTimestep);
	void SteeringLeft(float lfTimestep);
	void SteeringRight(float lfTimestep);

	cVec3 GetChasisPos(void);
	cVec3 GetChasisRot(void);
	float getEngineForce(void);
	void setEngineForce(float newForce);
	float getBreakingForce(void);
	void setBreakingForce(float newForce);

	/* añadido para depuracion de ruedas */
	struct ShapeCache{
		struct Edge { btVector3 n[2];int v[2]; };
		ShapeCache(btConvexShape* s) : m_shapehull(s) {}
		btShapeHull					m_shapehull;
		btAlignedObjectArray<Edge>	m_edges;
	};
	btAlignedObjectArray<ShapeCache*>	m_shapecaches;
	void debugWheels(btScalar* m, const btCollisionShape* shape, const btVector3& color,int	debugMode,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax);
	ShapeCache*	cache(btConvexShape*);


	int rightIndex;
	int upIndex;
	int forwardIndex;

	static const int maxProxies = 32766;
	static const int maxOverlap = 65535;

	///btRaycastVehicle is the interface for the constraint that implements the raycast vehicle
	///notice that for higher-quality slow-moving vehicles, another approach might be better
	///implementing explicit hinged-wheel constraints with cylinder collision, rather then raycasts
	float	gEngineForce;
	float	gBreakingForce;

	float	maxEngineForce; //1500.f; //this should be engine/velocity dependent
	float	maxBreakingForce;  // 100.f; 

	float	gVehicleSteering;
	float	steeringIncrement; //0.04f;
	float	steeringClamp;

	float	wheelRadius;	//** 0.5f;
	float	wheelWidth;		//* 0.4f;

	// The coefficient of friction between the tyre and the ground. 
	// Should be about 0.8 for realistic cars, but can increased for better handling.
	// Set large (10000.0) for kart racers
	float	wheelFriction;						// 1000;//BT_LARGE_FLOAT;

	// The stiffness constant for the suspension. 
	// 10.0 - Offroad buggy, 50.0 - Sports car, 200.0 - F1 Car
	float	suspensionStiffness;				// 20.f;

	// The damping coefficient for when the suspension is expanding. 
	// See the comments for wheelsDampingCompression for how to set k.
	// wheelsDampingRelaxation should be slightly larger than wheelsDampingCompression, eg 0.2 to 0.5
	float	suspensionDamping;	// 2.3f;

	// The damping coefficient for when the suspension is compressed. 
	// Set to k * 2.0 * btSqrt(suspensionStiffness) so k is proportional to critical damping.
	// k = 0.0 undamped & bouncy, k = 1.0 critical damping
	// 0.1 to 0.3 are good values
	float	suspensionCompression;	// 4.4f;

	// Reduces the rolling torque applied from the wheels that cause the vehicle to roll over.
	// This is a bit of a hack, but it's quite effective. 0.0 = no roll, 1.0 = physical behaviour.
	// If m_frictionSlip is too high, you'll need to reduce this to stop the vehicle rolling over.
	// You should also try lowering the vehicle's centre of mass
	float	rollInfluence;							// 0.1f;  //1.0f;	*/

	/*float	gEngineForce = 0.f;
	float	gBreakingForce = 0.f;

	float	maxEngineForce = 1000.f;//this should be engine/velocity dependent
	float	maxBreakingForce = 100.f;

	float	gVehicleSteering = 0.f;
	float	steeringIncrement = 0.04f;
	float	steeringClamp = 0.3f;
	float	wheelRadius = 0.5f;
	float	wheelWidth = 0.4f;
	float	wheelFriction = 1000;//BT_LARGE_FLOAT;
	float	suspensionStiffness = 20.f;
	float	suspensionDamping = 2.3f;
	float	suspensionCompression = 4.4f;
	float	rollInfluence = 0.1f;//1.0f;


	btScalar suspensionRestLength(0.6);*/

	btVector3 wheelDirectionCS0;
	btVector3 wheelAxleCS;

	// The maximum length of the suspension (metres)
	btScalar suspensionRestLength;  //** (0.6);

};

#endif