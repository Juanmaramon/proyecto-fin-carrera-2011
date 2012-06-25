#include "Vehicle.h"
#include "..\..\Physics\cPhysics.h"
#include "..\..\Graphics\GLHeaders.h"
#include "..\..\Graphics\GraphicManager.h"

Vehicle::Vehicle():
	m_carChassis(0)
	{
		rightIndex = 0;
		upIndex = 1;
		forwardIndex = 2;

		gEngineForce = 0.f;
		gBreakingForce = 0.f;
		maxEngineForce = 100000.f; //1500.f; //this should be engine/velocity dependent
		maxBreakingForce = 8000.f;  // 100.f; 
		gVehicleSteering = 0.f;
		steeringIncrement =  1.0f; //0.04f;
		steeringClamp = 0.3f;
		wheelRadius =  0.35f * PHYSCAR_SCALE;	//** 0.5f;
		wheelWidth = 0.2f * PHYSCAR_SCALE;		//* 0.4f;
		wheelFriction = 10;						// 1000;//BT_LARGE_FLOAT;
		suspensionStiffness = 50.f;				// 20.f;
		suspensionDamping =	 0.3f * 2.0f * btSqrt(suspensionStiffness);	// 2.3f;
		suspensionCompression = 0.2f * 2.0f * btSqrt(suspensionStiffness);	// 4.4f;
		rollInfluence = 0.5f;
		wheelDirectionCS0 = btVector3(0,-1,0);
		wheelAxleCS = btVector3(-1,0,0);
		suspensionRestLength = 0.15f * PHYSCAR_SCALE;  //** (0.6);

		m_vehicle = 0;
		m_wheelShape = 0;
		m_vehicleRayCaster = NULL;
		m_vehicle = NULL;
		m_compound = NULL;
}

Vehicle::~Vehicle(void){
	//if (m_vehicleRayCaster != NULL) delete m_vehicleRayCaster;
	//if (m_vehicle != NULL) delete m_vehicle;
	//if (m_compound != NULL) delete m_compound;
	//if (m_wheelShape != NULL) delete m_wheelShape;
	//if (m_chassisShape != NULL) delete m_chassisShape;
}

void Vehicle::initPhysics(string lsType){

	float mass, connectionHeight;

	if (lsType.compare("Mustang") == 0) {
		m_chassisShape = new btBoxShape(btVector3(1.f, 0.5f, 2.f));
		mass = 800.f;
		connectionHeight = 1.2f;
	}
	else if (lsType.compare("Truck") == 0) {
		m_chassisShape = new btBoxShape(btVector3(1.5f, 1.2f, 2.7f));
		mass = 1200.f;
		connectionHeight = .85f;
		wheelRadius =  0.54f;	//** 0.5f;
		wheelWidth = 0.4f;		//* 0.4f;
		steeringClamp = 0.1f;
		maxEngineForce = 98000.f;
	}

	// stash this shape away
	cPhysics::Get().getCollisionShapes().push_back(m_chassisShape);

	m_compound = new btCompoundShape();
	cPhysics::Get().getCollisionShapes().push_back(m_compound);
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0,1.4,0));		//** localTrans.setOrigin(btVector3(0,1,0));

	m_compound->addChildShape(localTrans,m_chassisShape);
	
	m_carChassis = cPhysics::Get().GetNewBody(m_compound, mass, cVec3(0.f, 0.f, 0.f), 329.9f); //chassisShape

	//m_carChassis->setDamping(0.2,0.2);
	
	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));
	
	ResetVehicleParams();

	/// create vehicle
	{
		
		m_vehicleRayCaster = new btDefaultVehicleRaycaster(cPhysics::Get().GetBulletWorld());
		m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);
		
		///never deactivate the vehicle
		m_carChassis->setActivationState(DISABLE_DEACTIVATION);

		cPhysics::Get().GetBulletWorld()->addVehicle(m_vehicle);		//** ->addVehicle(m_vehicle);

		bool isFrontWheel=true;

		//choose coordinate system
		m_vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

		// front left
		btVector3 connectionPointCS0;
		if (lsType.compare("Truck") == 0)
			connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth) + 0.25f,connectionHeight,(2*CUBE_HALF_EXTENTS-wheelRadius) + 0.2f);
		else 
			connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);	
			
		// front right
		if (lsType.compare("Truck") == 0)
			connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth) - 0.25f,connectionHeight,(2*CUBE_HALF_EXTENTS-wheelRadius) + 0.2f);
		else			
			connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

		// rear left
		if (lsType.compare("Truck") == 0)
			connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth) - 0.25f,connectionHeight,(-2*CUBE_HALF_EXTENTS+wheelRadius) + 0.18f);
		else
			connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);


		isFrontWheel = false;
		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
		
		// rear right
		if (lsType.compare("Truck") == 0)
			connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth) + 0.25f,connectionHeight,(-2*CUBE_HALF_EXTENTS+wheelRadius) + 0.18f);
		else
			connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);

		btWheelInfo& wheel_RR = m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
		
		for (int i=0;i<m_vehicle->getNumWheels();i++){
			btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
			wheel.m_suspensionStiffness = suspensionStiffness;
			wheel.m_wheelsDampingRelaxation = suspensionDamping;
			wheel.m_wheelsDampingCompression = suspensionCompression;
			wheel.m_frictionSlip = wheelFriction;
			wheel.m_rollInfluence = rollInfluence;
		}
	}
}

void Vehicle::ResetVehicleParams(){
	gVehicleSteering = 0.f;
	m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	m_carChassis->setLinearVelocity(btVector3(0, 0, 0));
	m_carChassis->setAngularVelocity(btVector3(0, 0, 0));

	// Posicion inicial
	cMatrix lTransMatrix, lRotMatrix, lTransform;
	lTransMatrix.LoadTranslation(cVec3(0.f, 0.f, 0.f));
	lRotMatrix.LoadIdentity();

	lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), PI);
	lTransform = lRotMatrix * lTransMatrix;
	m_carChassis->setWorldTransform(cPhysics::Get().Local2Bullet(lTransform));

	cPhysics::Get().GetBulletWorld()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(), cPhysics::Get().GetBulletWorld()->getDispatcher());
	if (m_vehicle)
	{
		m_vehicle->resetSuspension();
		for (int i=0;i<m_vehicle->getNumWheels();i++)
		{
			//synchronize the wheels with the (interpolated) chassis worldtransform
			m_vehicle->updateWheelTransform(i,true);
		}
	}
}

void Vehicle::renderme(){

	btScalar m[16];
	int i;

	btVector3 wheelColor(1,0,0);

	btVector3	worldBoundsMin,worldBoundsMax;
	//cPhysics::Get().GetBulletWorld()->getBroadphase()->getBroadphaseAabb(worldBoundsMin,worldBoundsMax);

	for (i=0;i<m_vehicle->getNumWheels();i++)
	{
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);
	
		//draw wheels (cylinders)
		m_vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(m);
		glPushMatrix();
		cVec3 lTranslation = cPhysics::Get().Bullet2Local(m_vehicle->getWheelInfo(i).m_worldTransform.getOrigin());
		
		cMatrix lTransMatrix, lRotMatrix;
		lTransMatrix.LoadTranslation(lTranslation);
		btQuaternion btq = m_vehicle->getWheelInfo(i).m_worldTransform.getRotation();
		cVec3 lqAxis = cPhysics::Get().Bullet2Local(btq.getAxis());
		lRotMatrix.LoadRotation(lqAxis, btq.getAngle());

		cGraphicManager::Get().SetWorldMatrix(lRotMatrix * lTransMatrix);
		debugWheels(m,m_wheelShape,wheelColor,1, btVector3(1.0,1.0,1.0) /*worldBoundsMin*/, btVector3(1.0,1.0,1.0) /*worldBoundsMax*/);
		
		lTransMatrix.LoadIdentity();
		cGraphicManager::Get().SetWorldMatrix(lTransMatrix);

		glPopMatrix();
	}
}

void Vehicle::Update(){
	// for rear wheel drive cars
	for (int i = 0; i < m_vehicle->getNumWheels(); i++){
		if (m_vehicle->getWheelInfo(i).m_bIsFrontWheel == true){
			m_vehicle->setSteeringValue(gVehicleSteering, i);
		}
		else{
			m_vehicle->applyEngineForce(gEngineForce, i);
			m_vehicle->setBrake(gBreakingForce, i);
		}
	}
}

void Vehicle::MoveForward(float lfTimestep){
	gEngineForce = maxEngineForce * lfTimestep;
	gBreakingForce = 0.f;
}

void Vehicle::Break(float lfTimestep){
	gBreakingForce = maxBreakingForce * lfTimestep; 
	gEngineForce = 0.f;
}

void Vehicle::SteeringLeft(float lfTimestep){
	gVehicleSteering += steeringIncrement * lfTimestep;
	if (gVehicleSteering > steeringClamp)
		gVehicleSteering = steeringClamp;
}

void Vehicle::SteeringRight(float lfTimestep){
	gVehicleSteering -= steeringIncrement * lfTimestep;
	if (gVehicleSteering < -steeringClamp)
		gVehicleSteering = -steeringClamp;
}

cVec3 Vehicle::GetChasisPos(void){
	btTransform chassisWorldTrans;
	m_carChassis->getMotionState()->getWorldTransform(chassisWorldTrans);
	cMatrix lmChasisTrans = cPhysics::Get().Bullet2Local(chassisWorldTrans);

	return lmChasisTrans.GetPosition();
}

cVec3 Vehicle::GetChasisRot(){
	btTransform chassisWorldTrans;
	m_carChassis->getMotionState()->getWorldTransform(chassisWorldTrans);
	cMatrix lmChasisTrans = cPhysics::Get().Bullet2Local(chassisWorldTrans);

	return lmChasisTrans.GetFront();
}

float Vehicle::getEngineForce(void){
	return gEngineForce;
}

void Vehicle::setEngineForce(float newForce){ 
	gEngineForce = newForce; 
}


float Vehicle::getBreakingForce(void){
	return gBreakingForce;
}

void Vehicle::setBreakingForce(float newForce){
	gBreakingForce = newForce;
}

Vehicle::ShapeCache*		Vehicle::cache(btConvexShape* shape)
{
	ShapeCache*		sc=(ShapeCache*)shape->getUserPointer();
	if(!sc)
	{
		sc=new(btAlignedAlloc(sizeof(ShapeCache),16)) ShapeCache(shape);
		sc->m_shapehull.buildHull(shape->getMargin());
		m_shapecaches.push_back(sc);
		shape->setUserPointer(sc);
		/* Build edges	*/ 
		const int			ni=sc->m_shapehull.numIndices();
		const int			nv=sc->m_shapehull.numVertices();
		const unsigned int*	pi=sc->m_shapehull.getIndexPointer();
		const btVector3*	pv=sc->m_shapehull.getVertexPointer();
		btAlignedObjectArray<ShapeCache::Edge*>	edges;
		sc->m_edges.reserve(ni);
		edges.resize(nv*nv,0);
		for(int i=0;i<ni;i+=3)
		{
			const unsigned int* ti=pi+i;
			const btVector3		nrm=btCross(pv[ti[1]]-pv[ti[0]],pv[ti[2]]-pv[ti[0]]).normalized();
			for(int j=2,k=0;k<3;j=k++)
			{
				const unsigned int	a=ti[j];
				const unsigned int	b=ti[k];
				ShapeCache::Edge*&	e=edges[btMin(a,b)*nv+btMax(a,b)];
				if(!e)
				{
					sc->m_edges.push_back(ShapeCache::Edge());
					e=&sc->m_edges[sc->m_edges.size()-1];
					e->n[0]=nrm;e->n[1]=-nrm;
					e->v[0]=a;e->v[1]=b;
				}
				else
				{
					e->n[1]=nrm;
				}
			}
		}
	}
	return(sc);
}

void Vehicle::debugWheels(btScalar* m, const btCollisionShape* shape, const btVector3& color,int	debugMode,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax){
	ShapeCache*	sc=cache((btConvexShape*)shape);

	btShapeHull* hull = &sc->m_shapehull; //(btShapeHull*)shape->getUserPointer();

	if (hull->numTriangles () > 0)
	{
		int index = 0;
		const unsigned int* idx = hull->getIndexPointer();
		const btVector3* vtx = hull->getVertexPointer();
		glColor3f(1.0f, 0.0f, .0f);
		glBegin (GL_TRIANGLES);

		for (int i = 0; i < hull->numTriangles (); i++)
		{
			int i1 = index++;
			int i2 = index++;
			int i3 = index++;
			btAssert(i1 < hull->numIndices () &&
				i2 < hull->numIndices () &&
				i3 < hull->numIndices ());

			int index1 = idx[i1];
			int index2 = idx[i2];
			int index3 = idx[i3];
			btAssert(index1 < hull->numVertices () &&
				index2 < hull->numVertices () &&
				index3 < hull->numVertices ());

			btVector3 v1 = vtx[index1];
			btVector3 v2 = vtx[index2];
			btVector3 v3 = vtx[index3];
			btVector3 normal = (v3-v1).cross(v2-v1);
			normal.normalize ();
			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f (v1.x(), v1.y(), v1.z());
			glVertex3f (v2.x(), v2.y(), v2.z());
			glVertex3f (v3.x(), v3.y(), v3.z());

		}
		glEnd ();
	}
	glNormal3f(0,1,0);
}
