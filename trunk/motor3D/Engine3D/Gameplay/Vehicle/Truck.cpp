#include "Truck.h"
#include "..\..\Physics\cPhysics.h"
#include "..\..\Physics\cPhysicObject.h"
#include "..\..\Graphics\GraphicManager.h" 
#include "..\..\Game\Game.h"
#include "..\..\Graphics\Camera.h"
#include "..\..\Physics\cPhysics.h"
#include "Mustang.h"

void Truck::Init(cObject* truckExterior, cObject* truckWeapon, cObject* truckTires) {

	// Inicializacion de físicas del vehiculo
	mVehicle.initPhysics("Truck");
	// Inicializacion de modelos 3d	
	mTruckExt = truckExterior;
	mTruckWea = truckWeapon;

	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires.push_back(*truckTires);
	}

	// Redimensionamiento de los modelos 3d

	//lScaleMatrix.LoadScale(.03f);
	//lScaleMatrixChasis.LoadScale(.0305f);
	lScaleMatrixChasis.LoadScale(.15f);
	//lScaleMatrix.LoadScale(.027f);
	mScaleMatrix.LoadScale(.128f);

	cMatrix lTransform;
	lTransform.LoadTranslation(cVec3(0.f, 0.0f, -40.f));
	mVehicle.m_carChassis->setWorldTransform(cPhysics::Get().Local2Bullet(lTransform));

}

void Truck::MoveForward(float lfTimestep){

	mVehicle.MoveForward(lfTimestep);
}

void Truck::Break(float lfTimestep){
	
	mVehicle.Break(lfTimestep);
}

void Truck::SteeringLeft(float lfTimestep){
	
	mVehicle.SteeringLeft(lfTimestep);
}

void Truck::SteeringRight(float lfTimestep){
	
	mVehicle.SteeringRight(lfTimestep);
}

void Truck::Update(float lfYaw, float lfPitch){
	mVehicle.Update();

	cMatrix lTransMatrix, lRotMatrix, lOffsetMatrix;
	lTransMatrix.LoadIdentity();
	lRotMatrix.LoadIdentity();
	lOffsetMatrix.LoadIdentity();

	lOffsetMatrix.LoadTranslation( cVec3(0.0f, 0.300f, -0.040f ) );
	// Acutlizacion de chasis, interior y metralleta
	mTruckExt->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));

	// La metralleta rota junto al chasis pero tambien siguiendo el raton
	cMatrix lmPostTranslation;
	lmPostTranslation.LoadIdentity();
	
	// Calculo de la posicion del arma
	cVec3 lvPosition = cGame::Get().GetMustang().GetVehicleBullet()->GetChasisPos()  - cGame::Get().GetMustang().GetVehicleBullet()->GetChasisRot();

	cVec3 lvDirection = lvPosition - mTruckWea->GetWorldMatrix().GetPosition();
	float yaw = atan2f(lvDirection.x, lvDirection.z);

	//lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), lfYaw);
	lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), yaw);

	lOffsetMatrix.LoadTranslation( cVec3(0.0f, 0.3f, 1.0f ) );
	lmPostTranslation.LoadTranslation(cVec3(0.f ,0.f, -1.05f));

	// Se posiciona el arma
	mTruckWea->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * lRotMatrix * lmPostTranslation *  cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));

	lRotMatrix.LoadIdentity();

	// Actualizacion ruedas
	for (int i=0; i < mVehicle.m_vehicle->getNumWheels() ; i++){

		//synchronize the wheels with the (interpolated) chassis worldtransform
		mVehicle.m_vehicle->updateWheelTransform(i,true);
	
		// Calcula  translacion rueda
		cVec3 lTranslation = cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getWheelInfo(i).m_worldTransform.getOrigin());
		
		lTransMatrix.LoadTranslation(lTranslation);
		
		// Calcula rotacion rueda
		btQuaternion btq = mVehicle.m_vehicle->getWheelInfo(i).m_worldTransform.getRotation();
		cVec3 lqAxis = cPhysics::Get().Bullet2Local(btq.getAxis());
		lRotMatrix.LoadRotation(lqAxis, btq.getAngle());	

		lOffsetMatrix.LoadTranslation( cVec3(-0.895f, -0.48f, -1.46f ) );
		if ((i == 0) || (i == 3)){
			lmPostTranslation.LoadTranslation(cVec3(0.75f ,0.f, 0.0f));
			//lmPostTranslation.LoadIdentity();
			cMatrix rotate;
			rotate.LoadRotation(cVec3(0.f, 1.f, 0.f), PI);
			maTires[i].SetWorldMatrix(mScaleMatrix * lOffsetMatrix * rotate *  lRotMatrix /** lmPostTranslation*/ * lTransMatrix);
		}else{
			lmPostTranslation.LoadTranslation(cVec3(-0.25f ,0.f, 0.0f));
			maTires[i].SetWorldMatrix(mScaleMatrix * lOffsetMatrix * lRotMatrix /** lmPostTranslation*/ * lTransMatrix);
		}

	}	
}

void Truck::Render(){

	// Renderiza rueda
	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i].Render();
	}

	// Renderiza arma
	mTruckWea->Render();

	// Renderiza chasis
	mTruckExt->Render();

	//mVehicle.renderme();

}

void Truck::Deinit(){
	GetVehicleBullet()->~Vehicle();

	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i].Deinit();
	}

	maTires.empty();
}