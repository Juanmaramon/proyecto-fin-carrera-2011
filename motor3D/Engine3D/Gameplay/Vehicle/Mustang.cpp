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
		maTires.push_back(mustangTires);
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
		//liNormalized_x = 2 * (lfPitch / muiHorizontalRes) - 1;
		// El eje Y esta invertido debido a que el origen del raton esta en la esquina izquierda superior
		//liNormalized_y = 1 - 2 * (lfYaw / muiVerticalRes);

		// Multiplicacion del matrices proyeccion y modelo/vista
		//lmView =  cGraphicManager::Get().GetWorldMatrix() * cGame::Get().Get3DCamera().GetView();
		glGetDoublev( GL_MODELVIEW_MATRIX, lmView );
		glGetDoublev( GL_PROJECTION_MATRIX, lmProj );
		glGetIntegerv(GL_VIEWPORT, laiViewport);
		float winY = (float)laiViewport[3] - lfPitch;  

		//cMatrix lsNearPoint = lmProjectionViewInverted * cVec4(liNormalized_x, liNormalized_y, 0, 1);

		// Obtiene la posicion Z (no en coordinadas del mundo pero si en el rango 0 - 1)
		GLfloat z_cursor;
		//glReadPixels(lfYaw, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);

		gluUnProject(lfYaw, winY, 0,
				    lmView, lmProj, laiViewport,
					&resX, &resY, &resZ);

		// Tiramos el rayo desde la posicion de la camara hasta el punto calculado anteriormente
		cVec3 lvRay_dir = cVec3(resX, resY, resZ) - cGame::Get().Get3DCamera().GetPosition();
		// Para tirar el rayo se utilizará la librería bullet
		btCollisionWorld::ClosestRayResultCallback rayCallback(cPhysics::Get().Local2Bullet(cGame::Get().Get3DCamera().GetPosition()), cPhysics::Get().Local2Bullet(lvRay_dir));
		
		// Realiza el test de rayo
		cPhysics::Get().GetBulletWorld()->rayTest(cPhysics::Get().Local2Bullet(cGame::Get().Get3DCamera().GetPosition()), cPhysics::Get().Local2Bullet(lvRay_dir), rayCallback);
		if (rayCallback.hasHit()) {
			cVec3 lvHitPoint = cPhysics::Get().Bullet2Local(rayCallback.m_hitPointWorld);
			
			char buff1[255];
			sprintf(buff1, "Hit position: %f, %f, %f \n", lvHitPoint.x, lvHitPoint.y, lvHitPoint.z);
			OutputDebugStr(buff1);

			// Calcula la direccion del arma
			cMatrix lmWeaponPosition = mMusWea->GetWorldMatrix();
			cVec3 lvDirection = lvHitPoint - lmWeaponPosition.GetPosition();

			// Calcula el angulo de rotacion
			float lfCosAngle = Dot( lmWeaponPosition.GetFront(), lvDirection.Normalize() );


			if ( lfCosAngle > 1.0f )
			{
				lfCosAngle = 1.0f;
			}
			else if (lfCosAngle < -1.0f )
			{
				lfCosAngle = -1.0f;
			}

			float lfAngle = acosf(lfCosAngle);

			sprintf(buff1, "angle: %f \n", lfAngle);
			OutputDebugStr(buff1);


			// Realiza el test del plano para saber si la rotacion sera para la derecha o izq.
			cPlane lPlane;
			lPlane.Init( lmWeaponPosition.GetRight(), lmWeaponPosition.GetPosition()); 
			if ( lPlane.PointTest(lvHitPoint) < 0.0f )
			{
				mfWeaponYaw += lfAngle;
			}
			else
			{
				mfWeaponYaw -= lfAngle;
			}
			
			// Rota el arma
			lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), mfWeaponYaw);
		}


		//lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), lfYaw);
		char buff[255];
		sprintf(buff, "Mouse Yaw: %f, Pitch: %f \n", lfYaw, lfPitch);
		OutputDebugStr(buff);
		sprintf(buff, "Mouse X: %f, Y: %f, Z: %f \n", resX, resY, resZ);
		OutputDebugStr(buff);
		sprintf(buff, "Ray direction: %f, %f, %f\n", lvRay_dir.x, lvRay_dir.y, lvRay_dir.z);	
		OutputDebugStr(buff);
	}
	lOffsetMatrix.LoadTranslation( cVec3(4.5f, 1.4f, 0.5f ) );
	lmPostTranslation.LoadTranslation(cVec3(0.f ,0.f, -0.5f));
	//lOffsetMatrix.LoadTranslation( cVec3(3.5f, 1.4f, 1.708f ) );
	mMusWea->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * lRotMatrix * lmPostTranslation *  cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));

	// Actualizacion ruedas
	for (int i=0; i < mVehicle.m_vehicle->getNumWheels() ; i++){

		//synchronize the wheels with the (interpolated) chassis worldtransform
		mVehicle.m_vehicle->updateWheelTransform(i,true);
	
		glPushMatrix();
		
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
			maTires[i]->SetWorldMatrix(mScaleMatrix * lOffsetMatrix * rotate * lRotMatrix * lTransMatrix);
		}else{
			maTires[i]->SetWorldMatrix(mScaleMatrix * lOffsetMatrix * lRotMatrix * lTransMatrix);
		}

		glPopMatrix();
	}	
}

void Mustang::Render(){

	// Renderiza rueda
	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i]->Render();
	}
	// Renderiza interior
	mMusInt->Render();

	mMusWea->Render();


	// Renderiza chasis
	mMusExt->Render();

}

void Mustang::Deinit(){
	GetVehicleBullet()->~Vehicle();
}