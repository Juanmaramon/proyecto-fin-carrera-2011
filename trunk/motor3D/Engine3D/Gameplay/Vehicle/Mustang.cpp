#include "Mustang.h"
#include "..\..\Physics\cPhysics.h"
#include "..\..\Physics\cPhysicObject.h"
#include "..\..\Graphics\GraphicManager.h" 
#include "..\..\Game\Game.h"
#include "..\..\Graphics\Camera.h"
#include "..\..\Physics\cPhysics.h"

void Mustang::Init(cObject* mustangExterior, cObject* mustangInterior, cObject* mustangWeapon, cObject* mustangTires){

	// Inicializacion de físicas del vehiculo
	mVehicle.initPhysics();
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

	muiHorizontalRes = cGame::Get().GetGameWidth(); 
	muiVerticalRes = cGame::Get().GetGameHeight();

	mfWeaponYaw = 0.0f;
	mfDistance = 50.0f;
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

void Mustang::Update(float lfYaw, float lfPitch){
	mVehicle.Update();

	cMatrix lTransMatrix, lRotMatrix, lOffsetMatrix;
	lTransMatrix.LoadIdentity();
	lRotMatrix.LoadIdentity();
	lOffsetMatrix.LoadIdentity();

	lOffsetMatrix.LoadTranslation( cVec3(4.5f, 1.400f, -0.040f ) );
	// Acutlizacion de chasis, interior y metralleta
	mMusExt->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));
	mMusInt->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));
	// La metralleta rota junto al chasis pero tambien siguiendo el raton
	cMatrix lmPostTranslation;
	lmPostTranslation.LoadIdentity();
	//int liNormalized_x, liNormalized_y; 
	GLdouble lmView[16];
	GLdouble lmProj[16];
	GLint laiViewport[4];
	GLdouble resX, resY, resZ;
	if ((lfYaw != 0.0f) && (lfPitch != 0.0f)) {
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
		OutputDebugStr(buff1);
		sprintf(buff1, "UnProject %f, %f, %f \n", resX, resY, resZ);
		OutputDebugStr(buff1);
		
		// Calculo de la posicion del arma
		cVec3 lvPosition = (cGame::Get().Get3DCamera().GetFront() * mfDistance) + cVec3(resX, resY, resZ) + cGame::Get().Get3DCamera().GetPosition();
		cVec3 lvDirection = lvPosition - mMusWea->GetWorldMatrix().GetPosition();
	
		// Calcula el angulo de rotacion
		//float lfCosAngle = Dot( mMusWea->GetWorldMatrix().GetPosition(), lvDirection.Normalize() );

		float lfCosAngle = Dot( mMusWea->GetWorldMatrix().GetFront(), lvDirection.Normalize() );


		// El coseno estara entre [-1, 1]
		if ( lfCosAngle > 1.0f )
		{
			lfCosAngle = 1.0f;
		}
		else if (lfCosAngle < -1.0f )
		{
			lfCosAngle = -1.0f;
		}

		float lfAngle = acosf(lfCosAngle);

		// Realiza el test del plano para saber si la rotacion sera para la derecha o izq.
		cPlane lPlane;
		lPlane.Init( mMusWea->GetWorldMatrix().GetRight(), mMusWea->GetWorldMatrix().GetPosition());

		//lfAngle -= mfWeaponYaw;
		
		if ( lPlane.PointTest(lvPosition) < 0.0f )
		{
			mfWeaponYaw += lfAngle;
		}
		else
		{
			mfWeaponYaw -= lfAngle;
		}

		sprintf(buff1, "angle: %f \n", lfAngle);
		OutputDebugStr(buff1);

		sprintf(buff1, "WeaponYaw: %f \n", mfWeaponYaw);
		OutputDebugStr(buff1);

		// Rota el arma
		lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), mfWeaponYaw);

	}

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