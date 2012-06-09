#include "Mustang.h"
#include "..\..\Physics\cPhysics.h"
#include "..\..\Physics\cPhysicObject.h"
#include "..\..\Graphics\GraphicManager.h" 
#include "..\..\Game\Game.h"
#include "..\..\Graphics\Camera.h"
#include "..\..\Physics\cPhysics.h"

void Mustang::Init(cObject* mustangExterior, cObject* mustangInterior, cObject* mustangWeapon, cObject* mustangTires){

	// Inicializacion de físicas del vehiculo
	mVehicle.initPhysics("Mustang");
	// Inicializacion de modelos 3d	
	mMusExt = mustangExterior;
	mMusInt = mustangInterior;
	mMusWea = mustangWeapon;

	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires.push_back(*mustangTires);
	}

	// Redimensionamiento de los modelos 3d
	cMatrix lScaleMatrix;
	lScaleMatrix.LoadScale(.03f);
	//lScaleMatrixChasis.LoadScale(.0305f);
	lScaleMatrixChasis.LoadScale(.0308f);
	//lScaleMatrix.LoadScale(.027f);
	mScaleMatrix = lScaleMatrix;
	mfPreviousYaw = 0.0f;
}

void Mustang::MoveForward(float lfTimestep){

	mVehicle.MoveForward(lfTimestep);
}

void Mustang::Break(float lfTimestep){
	
	mVehicle.Break(lfTimestep);
}

void Mustang::SteeringLeft(float lfTimestep){
	
	mVehicle.SteeringLeft(lfTimestep);
}

void Mustang::SteeringRight(float lfTimestep){
	
	mVehicle.SteeringRight(lfTimestep);
}

void Mustang::Update(float lfYaw, float lfPitch, bool lbIsCameraAux, bool lbFireMainWeapon){
	mVehicle.Update();

	cMatrix lTransMatrix, lRotMatrix, lOffsetMatrix;
	lTransMatrix.LoadIdentity();
	lRotMatrix.LoadIdentity();
	lOffsetMatrix.LoadIdentity();

	lOffsetMatrix.LoadTranslation( cVec3(4.5f, 1.400f, -0.040f ) );
	// Actualizacion de chasis, interior y metralleta
	mMusExt->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));
	mMusInt->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));
	// La metralleta rota junto al chasis pero tambien siguiendo el raton
	cMatrix lmPostTranslation;
	lmPostTranslation.LoadIdentity();

	GLdouble lmView[16];
	GLdouble lmProj[16];
	GLint laiViewport[4];
	GLdouble resX, resY, resZ;

	// Se cargan las matrices modelo/vista y proyeccion y el viewport
	glGetDoublev( GL_MODELVIEW_MATRIX, lmView );
	glGetDoublev( GL_PROJECTION_MATRIX, lmProj );
	glGetIntegerv(GL_VIEWPORT, laiViewport);

	// La coordenada Y se obtiene mediante la resta del viewport y la coordenada Y del raton
	float winY = (float)laiViewport[3] - lfPitch;  

	// Se proyecta el punto 2D a coordenadas 3D
	gluUnProject(lfYaw, winY, 0,
			    lmView, lmProj, laiViewport,
				&resX, &resY, &resZ);

	char buff1[256];
	sprintf(buff1, "--------------------------------------- \n");
	//OutputDebugStr(buff1);
	
	// -- Calculo de la posicion del arma

	cVec3 lvPosition = cVec3(resX, resY, resZ);
	
	//sprintf(buff1, "Weapon position %f, %f, %f \n", mMusWea->GetWorldMatrix().GetPosition().x, mMusWea->GetWorldMatrix().GetPosition().y, mMusWea->GetWorldMatrix().GetPosition().z);
	//OutputDebugStr(buff1);

	// Paso la posicion del protagonista a coordenadas del protagonista
	cMatrix lmMustangPosition = cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()) * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).Invert();

	cVec3 lvDirection = lvPosition - lmMustangPosition.GetPosition();
	float yaw = atan2f(lvDirection.x, lvDirection.z);

	//sprintf(buff1, "Direction %f, %f, %f \n", lvDirection.x, lvDirection.y, lvDirection.z);
	//OutputDebugStr(buff1);

	// Inercia
	float lfyaw = yaw * 0.1f + mfPreviousYaw * (1  - 0.1f);
	mfPreviousYaw = lfyaw;

	// Start and End are vectors
	cVec3 lvEnd = cVec3(lvDirection.x, mMusWea->GetWorldMatrix().GetPosition().y, lvDirection.z);
	btCollisionWorld::ClosestRayResultCallback RayCallback(cPhysics::Get().Local2Bullet(mMusWea->GetWorldMatrix().GetPosition()), cPhysics::Get().Local2Bullet(lvEnd));

	// Perform raycast
	cPhysics::Get().GetBulletWorld()->rayTest(cPhysics::Get().Local2Bullet(mMusWea->GetWorldMatrix().GetPosition()), cPhysics::Get().Local2Bullet(lvEnd), RayCallback);

	if(RayCallback.hasHit()) {
		btVector3 End = RayCallback.m_hitPointWorld;
		btVector3 Normal = RayCallback.m_hitNormalWorld;
		btCollisionObject* lbCollisionObject = RayCallback.m_collisionObject;

		btRigidBody* body = btRigidBody::upcast(RayCallback.m_collisionObject);
		
		if (body) {

			Truck* lpBodyPtr = (Truck*)body->getUserPointer();

			if (lpBodyPtr != NULL) {
				sprintf(buff1, "Hit! Hit! \n");
				OutputDebugStr(buff1);
			}

		}
	}

	// La camara es auxiliar las rotaciones son inversas (se ve de delante hacia atras)
	if (lbIsCameraAux)
		yaw = -yaw;

	lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), -lfyaw);

	lOffsetMatrix.LoadTranslation( cVec3(4.5f, 1.4f, 0.5f ) );
	lmPostTranslation.LoadTranslation(cVec3(0.f ,0.f, -0.5f));

	// Se posiciona el arma
	mMusWea->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * lRotMatrix * lmPostTranslation *  cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));

	lRotMatrix.LoadIdentity();

	// Actualizacion ruedas
	for (int i=0; i < mVehicle.m_vehicle->getNumWheels() ; i++){

		//synchronize the wheels with the (interpolated) chassis worldtransform
		mVehicle.m_vehicle->updateWheelTransform(i,true);
	
		//glPushMatrix();
		
		// Calcula  translacion rueda
		cVec3 lTranslation = cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getWheelInfo(i).m_worldTransform.getOrigin());


		/*if (lTranslation.DistanceTo(cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).GetPosition()) > 2.0f) {
			char buff2[256];
			sprintf(buff2, "Distance: %f \n", lTranslation.DistanceTo(cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).GetPosition()) );
			OutputDebugStr(buff2);
		}
		else {
			char buff2[256];
			sprintf(buff2, "Wrong Distance: %f \n", lTranslation.DistanceTo(cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).GetPosition()) );
			OutputDebugStr(buff2);
		}*/
		
		lTransMatrix.LoadTranslation(lTranslation);
		
		// Calcula rotacion rueda
		btQuaternion btq = mVehicle.m_vehicle->getWheelInfo(i).m_worldTransform.getRotation();
		cVec3 lqAxis = cPhysics::Get().Bullet2Local(btq.getAxis());
		lRotMatrix.LoadRotation(lqAxis, btq.getAngle());	

		lOffsetMatrix.LoadTranslation( cVec3(3.5f, 0.255f, -1.593f ) );
		if ((i == 0) || (i == 3)){
			cMatrix rotate;
			// Las ruedas de la parte izquierda estan rotadas 180º por defecto para mostrarse correctamente
			rotate.LoadRotation(cVec3(0.f,1.f,0.f), PI);
			maTires[i].SetWorldMatrix(mScaleMatrix * lOffsetMatrix * rotate * lRotMatrix * lTransMatrix);
		}else{
			maTires[i].SetWorldMatrix(mScaleMatrix * lOffsetMatrix * lRotMatrix * lTransMatrix);
		}

		//glPopMatrix();
	}	
}

void Mustang::Render(){

	// Renderiza rueda
	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i].Render();
	}
	// Renderiza interior
	mMusInt->Render();

	mMusWea->Render();


	// Renderiza chasis
	mMusExt->Render();

}

void Mustang::Deinit(){
	GetVehicleBullet()->~Vehicle();

	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i].Deinit();
	}

	maTires.empty();
}