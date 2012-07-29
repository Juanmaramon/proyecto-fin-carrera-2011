#include "Mustang.h"
#include "..\..\Physics\cPhysics.h"
#include "..\..\Physics\cPhysicObject.h"
#include "..\..\Graphics\GraphicManager.h" 
#include "..\..\Game\Game.h"
#include "..\..\Graphics\Camera.h"
#include "..\..\Physics\cPhysics.h"
#include "..\..\Graphics\GraphicManager.h"
#include "../../Graphics/Textures/Texture.h"

void Mustang::Init(cObject* mustangExterior, cObject* mustangInterior, cObject* mustangWeapon, cObject* mustangTires, 
				   cResourceHandle* weapon_muzzle1, cResourceHandle* weapon_muzzle2, cResourceHandle* weapon_muzzle3, 
				   cResourceHandle* arrow_enemy, cResourceHandle* mPositiveAmmunition, cObject* mustangExteriorDestroyed,
				   cObject* mustangInteriorDestroyed, cResourceHandle* explosion_sprite,
				 cResourceHandle* explosion_sprite1, cResourceHandle* particle_texture,
				 cResourceHandle* crosshair) {

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
	mBullets.Init(WEAPON_RANGE);
	// Texturas de fogonazo arma
	mWeaponMuzzle1 = weapon_muzzle1;
	mWeaponMuzzle2 = weapon_muzzle2;
	mWeaponMuzzle3 = weapon_muzzle3;
	// Textura de flecha que persigue enemigo
	mArrowEnemy  = arrow_enemy;
	// Texturas de HUD
 	mPosAmmu = mPositiveAmmunition;
	// Secuencia de animacion fogonazo arma
	miFlashSeq = 0;
	// Direccion de la flecha que indica el enemigo
	mfEnemyArrow = 0.0f;

	// Callback pointer to get this object from bullet collisionShape
	mVehicle.m_carChassis->setUserPointer(this);

	// Contador de vidas
	miCurrentLives = 300;

	// Modelo destruido
	mMusExtDes = mustangExteriorDestroyed;
	mMusIntDes = mustangInteriorDestroyed;

	// Explosion
	mExplosion.Init(explosion_sprite, explosion_sprite1, weapon_muzzle1, particle_texture);

	mState = eREADY;

	mCrosshair = crosshair;
	mCHRotationCount = 0.0f;
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

void Mustang::Update(float lfTimestep, float lfYaw, float lfPitch, bool lbIsCameraAux, bool lbFireMainWeapon){
	char buff1[256];
	sprintf(buff1, " Current lives: %d \n", miCurrentLives);
	//OutputDebugStr(buff1);

	mbIsFiring = false;
	mbIsCamAux = lbIsCameraAux;
	mYaw = lfYaw;
	mPitch = lfPitch;
	mCHRotationCount += 0.001f;

	mVehicle.Update();

	// Si no esta con vida, no se actualiza
	if (!IsAlive()) {

		// Activa animacion de explosion (solo recien destruido)
		if (mState == eDESTROYED)
			mExplosion.SetAlive(true);
	
		if (mState == eDESTROYED)
			mState = eDEATH;

		Break(lfTimestep);
		
		mMusExtDes->SetWorldMatrix(mMusExt->GetWorldMatrix());
		mMusIntDes->SetWorldMatrix(mMusInt->GetWorldMatrix());

		if (mState == eDEATH) {
			cMatrix lmPosFix;
			lmPosFix.LoadTranslation(cVec3(3.7f, 0.0f, 0.f));
			mExplosion.SetPosition(mMusExtDes->GetWorldMatrix() * lmPosFix);
			// Correccion sobre la posicion del sist. particulas
			lmPosFix.LoadTranslation(cVec3(4.5f, -1.15f, -0.9f));
			mExplosion.SetParticlesPosition(mMusExtDes->GetWorldMatrix() * lmPosFix);
		}

	}

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
	
	// -- Calculo de la posicion del arma

	cVec3 lvPosition = cVec3(resX, resY, resZ);

	// Paso la posicion del protagonista a coordenadas del protagonista
	cMatrix lmMustangPosition = cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()) * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).Invert();

	cVec3 lvDirection = lvPosition - lmMustangPosition.GetPosition();
	
	float yaw;
	// Si esta destruido no actualiza posicion arma
	if (IsAlive())
		yaw = atan2f(lvDirection.x, lvDirection.z);
	else
		yaw = mfPreviousYaw;

	// Inercia
	float lfyaw = yaw * 0.1f + mfPreviousYaw * (1  - 0.1f);
	mfPreviousYaw = lfyaw;

	// La camara es auxiliar las rotaciones son inversas (se ve de delante hacia atras)
	if (lbIsCameraAux)
		lfyaw = -lfyaw;

	lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), -lfyaw);

	lOffsetMatrix.LoadTranslation( cVec3(4.5f, 1.4f, 0.5f ) );
	lmPostTranslation.LoadTranslation(cVec3(0.f ,0.f, -0.5f));

	// Se posiciona el arma
	mMusWea->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * lRotMatrix * lmPostTranslation *  cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));

	// Si se esta disparando, raytest para simular el rayo del laser de la ametralladora
	if (lbFireMainWeapon) {
		mbIsFiring = true;
		mbLaserHit = false;
		
		// Start and End are vectors
		cVec3 lvEnd;
		cVec3 lvStart;

		TransformPoint( lvStart, cVec3(-146.0f, 41.0f, 10.0f ), mMusWea->GetWorldMatrix() );
		TransformPoint( lvEnd, cVec3(-146.f, 41.f, WEAPON_RANGE), mMusWea->GetWorldMatrix() );

		btCollisionWorld::ClosestRayResultCallback RayCallbackLaser(cPhysics::Get().Local2Bullet(lvStart), cPhysics::Get().Local2Bullet(lvEnd));

		// Perform raycast
		cPhysics::Get().GetBulletWorld()->rayTest(cPhysics::Get().Local2Bullet(lvStart), cPhysics::Get().Local2Bullet(lvEnd), RayCallbackLaser);

		if (RayCallbackLaser.hasHit()) {
			mbLaserHit = true;
			btVector3 LaserEnd = RayCallbackLaser.m_hitPointWorld;
			mvRayLaserHitPosition = cPhysics::Get().Bullet2Local(LaserEnd);
			btVector3 Normal = RayCallbackLaser.m_hitNormalWorld;
			btCollisionObject* lbCollisionObject = RayCallbackLaser.m_collisionObject;

			btRigidBody* body = btRigidBody::upcast(RayCallbackLaser.m_collisionObject);
			
			if (body) {

				Truck* lpBodyPtr = (Truck*)body->getUserPointer();

				if (lpBodyPtr != NULL) {
					sprintf(buff1, "Hit! Hit!\n");
					//OutputDebugStr(buff1);
				}
				else {
					sprintf(buff1, "\n\n\n");
					//OutputDebugStr(buff1);
				}
			}
		}


		cVec3 lvHit;
		if (mbLaserHit)
			TransformPoint( lvHit, mvRayHitPosition, mMusWea->GetWorldMatrix().Invert() );
		else
			lvHit = lvEnd;

		// Ahora se calcula la trayectoria de la bala real
		// Antes se calculo el rayo del laser de la torreta
		float a = -4000.0f;
		float b = 4000.0f;
		float deviation_x = ((b - a) * ((float)rand() / RAND_MAX)) + a;
		float deviation_y = ((b - a) * ((float)rand() / RAND_MAX)) + a;
		float deviation_z = ((b - a) * ((float)rand() / RAND_MAX)) + a;

		// Este es el punto final con las direccion aleatorio
		cVec3 lvEndPoint = cVec3(-146.f + deviation_x, 41.f + deviation_y, WEAPON_RANGE +  deviation_z);

		TransformPoint( lvStart, cVec3(-146.0f, 41.0f, 10.0f ), mMusWea->GetWorldMatrix() );
		TransformPoint( lvEnd, lvEndPoint, mMusWea->GetWorldMatrix() );

		sprintf(buff1, " ini_x: %2.2f, ini_y: %2.2f, ini_z: %2.2f \n", -146.f, 41.f , 10.0f);
		//OutputDebugStr(buff1);

		sprintf(buff1, " final_x: %2.2f, final_y: %2.2f, final_z: %2.2f \n", lvEnd.x, lvEnd.y, lvEnd.z);
		//OutputDebugStr(buff1);

		btCollisionWorld::ClosestRayResultCallback RayCallback(cPhysics::Get().Local2Bullet(lvStart), cPhysics::Get().Local2Bullet(lvEnd));

		// Perform raycast
		cPhysics::Get().GetBulletWorld()->rayTest(cPhysics::Get().Local2Bullet(lvStart), cPhysics::Get().Local2Bullet(lvEnd), RayCallback);

		// Se guarda la posicion final
		mvRayHitPosition = lvEnd;

		if (RayCallback.hasHit()) {
			mbHit = true;
			btVector3 End = RayCallback.m_hitPointWorld;
			mvRayHitPosition = cPhysics::Get().Bullet2Local(End);
			btVector3 Normal = RayCallback.m_hitNormalWorld;
			btCollisionObject* lbCollisionObject = RayCallback.m_collisionObject;

			btRigidBody* body = btRigidBody::upcast(RayCallback.m_collisionObject);
			
			if (body) {

				Truck* lpBodyPtr = (Truck*)body->getUserPointer();

				if (lpBodyPtr != NULL) {
					sprintf(buff1, "Hit! Hit!\n");
					//OutputDebugStr(buff1);
					lpBodyPtr->Damage();
				}
				else {
					sprintf(buff1, "\n\n\n");
					//OutputDebugStr(buff1);
				}
			}
		}


		//mBullets.Create(lvStart, lvHit, lfyaw);
	}

	// Ahora se calcula la orientacion de la flecha de aviso de enemigos
	// Calcular el vector distancia (diferencia entre la posición del
	// perseguidor y la posición del objetivo a perseguir)
	// Se pasa a coordenadas del jugador
	cMatrix mTarget = cPhysics::Get().Bullet2Local(cGame::Get().GetTruck().GetVehicleBullet()->m_vehicle->getChassisWorldTransform()) * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).Invert();

	cVec3 lDistanceVec;
	// Dependiendo de la pos de la camara el vector direccion se orientara de una forma u otra
	if (lbIsCameraAux)
		lDistanceVec = mTarget.GetPosition().Normalize();
	else
		lDistanceVec = -mTarget.GetPosition().Normalize();

	mfEnemyArrow = atan2f(lDistanceVec.x, lDistanceVec.z);

	lRotMatrix.LoadIdentity();

	// Actualizacion ruedas
	for (int i=0; i < mVehicle.m_vehicle->getNumWheels() ; i++){

		//synchronize the wheels with the (interpolated) chassis worldtransform
		mVehicle.m_vehicle->updateWheelTransform(i,true);
		
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
	}

	//mBullets.Update();
}

void Mustang::Render(){

	// Renderiza rueda
	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i].Render();
	}
	
	// Si es la camara aux renderizar el muzzle lo ultimo
	if (mbIsCamAux) {
		if (IsAlive()) {

			// Renderiza interior
			mMusInt->Render();

			// Renderiza el arma
			mMusWea->Render();

			// Renderiza chasis
			mMusExt->Render();

			// Renderiza fogonazo del arma y laser
			glDisable (GL_FOG);

			RenderRayGun ();
			RenderMuzzle();

			glEnable (GL_FOG);

		} else {

			// Si esta destruido reemplaza el modelo por el coche destrozado
			
			// Renderiza interior
			mMusIntDes->Render();

			if (mState == eDEATH)
				mExplosion.RenderSmoke();

			// Renderiza chasis destruido
			mMusExtDes->Render();

			mExplosion.Render();

		}

	} else {

		if (IsAlive()) {

			// Renderiza fogonazo del arma y laser
			glDisable (GL_FOG);

			RenderRayGun ();
			RenderMuzzle();

			glEnable (GL_FOG);

			// Renderiza interior
			mMusInt->Render();

			// Renderiza el arma
			mMusWea->Render();

			// Renderiza chasis
			mMusExt->Render();

		} else {

			// Si esta destruido reemplaza el modelo por el coche destrozado
			
			// Renderiza interior
			mMusIntDes->Render();

			if (mState == eDEATH)
				mExplosion.RenderSmoke();

			// Renderiza chasis destruido
			mMusExtDes->Render();

			mExplosion.Render();

		}

	}

		//mBullets.Render();
}

void Mustang::RenderRayGun () {

	// Si se esta disparando el arma
	if (mbIsFiring) {
		// Primero pinta el laser del arma
		//glBlendFunc(GL_DST_COLOR, GL_ONE);
		//glBlendFunc(GL_DST_COLOR, GL_ZERO);
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		// He elegido esta funcion de blending porque se ve incluso sobre colores negros de fondo
		glEnable(GL_BLEND);		
		glBlendFunc(GL_ONE, GL_ONE);

		// Enable antialiasing
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

		cMatrix lmPosMatrix;
		cGraphicManager::Get().SetWorldMatrix(mMusWea->GetWorldMatrix());

		cVec3 lvHitPoint;

		// Se renderiza el rayo del laser de la torreta
		// Si el rayo choca con un objeto entonces se para en ese objeto, si no hasta el alcance del arma 
		if (mbLaserHit)
			TransformPoint( lvHitPoint, mvRayLaserHitPosition, mMusWea->GetWorldMatrix().Invert() );
		else
			lvHitPoint = cVec3(-146.f, 41.f, WEAPON_RANGE);

			cGraphicManager::Get().DrawLine(cVec3(-146.0f, 41.0f, 10.0f ), lvHitPoint, cVec3(1.f, 0.f, 0.f), 0.5f);
			cGraphicManager::Get().DrawSphere(lvHitPoint, cVec3(1.f, 0.f, 0.f), 0.5f);

		// Ahora se renderiza la bala real con el desplazamiento calculado
	/*	if (mbHit)
			TransformPoint( lvHitPoint, mvRayHitPosition, mMusWea->GetWorldMatrix().Invert() );
		else
			lvHitPoint = mvRayHitPosition;

		char buff1[256];
		sprintf(buff1, "Punto de impacto: (%2.2f , %2.2f , %2.2f) \n", lvHitPoint.x, lvHitPoint.y, lvHitPoint.z);
		//OutputDebugStr(buff1);

			cGraphicManager::Get().DrawLine(cVec3(-146.0f, 41.0f, 10.0f ), cVec3(lvHitPoint.x, lvHitPoint.y,lvHitPoint.z), cVec3(0.f, 0.f, 1.f), 0.2f);
			cGraphicManager::Get().DrawSphere(lvHitPoint, cVec3(0.f, 0.f, 1.f), 0.2f);	*/

		cGraphicManager::Get().SetWorldMatrix(lmPosMatrix.LoadIdentity());

		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
		glColor4f(1,1,1,1.f);
	}

}

void Mustang::Deinit(){
	GetVehicleBullet()->~Vehicle();

	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i].Deinit();
	}

	maTires.empty();

	mBullets.Deinit();

}

void Mustang::RenderArrowEnemy () {

	if (cGame::Get().GetTruck().IsAlive()) {

		cMatrix lmRotate, lmTranslation;
		lmRotate.LoadRotation(cVec3(0.f, 0.f, 1.f), mfEnemyArrow);
		lmTranslation.LoadTranslation(cVec3(0.f, -330.f, 0.f));
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ONE);

		cGraphicManager::Get().SetWorldMatrix(lmTranslation * lmRotate);
		
		cTexture* lpTexture = (cTexture*) mArrowEnemy->GetResource();
		glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBegin(GL_QUADS);

			 glTexCoord2f(0.0f, 0.0f); glVertex3f(-25, -25, 0);	
			 glTexCoord2f(1.0f, 0.0f); glVertex3f(25, -25, 0);
			 glTexCoord2f(1.0f, 1.0f); glVertex3f(25, 25, 0);
			 glTexCoord2f(0.0f, 1.0f); glVertex3f(-25, 25, 0); 

		glEnd();

		cGraphicManager::Get().SetWorldMatrix(lmRotate.LoadIdentity());
		glDisable(GL_BLEND);
		glColor4f(1,1,1,1.f);
	
	}

}

void Mustang::RenderMuzzle() {

	if (mbIsFiring) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		cVec3 lvMuzzlePos;
		TransformPoint( lvMuzzlePos, cVec3(-146.0f, 41.0f, 10.0f ), mMusWea->GetWorldMatrix() );

		cMatrix lmBillboardMatrix;
		lmBillboardMatrix.LoadTranslation(lvMuzzlePos);
		cGraphicManager::Get().SetWorldMatrix(lmBillboardMatrix);

		// Se aplica el billboard para ignorar la rotacion y orientar el quad a la camara
		cGraphicManager::Get().BillboardCheatSphericalBegin();

		// Ahora pinta el fogonazo del arma
		glColor4f (1.f, 1.f, 1.f, 1.f);
		cTexture* lpTexture;

		miFlashSeq = rand() % 5;

		bool void_step = false;
		switch (miFlashSeq) {
			case 0:
				   lpTexture = (cTexture*) mWeaponMuzzle1->GetResource();
				   break;
			case 1:
				   lpTexture = (cTexture*) mWeaponMuzzle2->GetResource();
				   break;
			case 2:
				   lpTexture = (cTexture*) mWeaponMuzzle3->GetResource();
				   break;
			default:
			case 4:
			case 5:
				   void_step = true;
				   break;
		}

		if (!void_step) {
			glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glBegin(GL_QUADS);

				 glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, 0);	
				 glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, 0);
				 glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, 0);
				 glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, 0); 

				glEnd();
		}

		cGraphicManager::Get().BillboardEnd();

		cGraphicManager::Get().SetWorldMatrix(lmBillboardMatrix.LoadIdentity());

		glDisable(GL_BLEND);
		glColor4f(1,1,1,1.f);

	}

}

void Mustang::RenderHUD () {

	if (IsAlive()) {

		cMatrix lmRotate, lmTranslation;
		lmRotate.LoadRotation(cVec3(0.f, 0.f, 1.f), PI);
		lmTranslation.LoadTranslation(cVec3(0.f, -250.f, 0.f));
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ONE);

		cGraphicManager::Get().SetWorldMatrix(lmRotate*lmTranslation);
		
		cTexture* lpTexture = (cTexture*) mPosAmmu->GetResource();
		glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBegin(GL_QUADS);

  			 glTexCoord2f(1.0f, 0.0f); glVertex3f(40, -40, 0);
			 glTexCoord2f(0.0f, 0.0f); glVertex3f(-40, -40, 0);
			 glTexCoord2f(0.0f, 1.0f); glVertex3f(-40, 40, 0); 
			 glTexCoord2f(1.0f, 1.0f); glVertex3f(40, 40, 0);

		glEnd();

		cGraphicManager::Get().SetWorldMatrix(lmTranslation.LoadIdentity());
		glDisable(GL_BLEND);
		glColor4f(1,1,1,1.f);
	
	}

}

// Chequea las vidas protagonista
bool Mustang::IsAlive() {

	return ((miCurrentLives > 0)? true : false);

}

void Mustang::Damage() {

	if (IsAlive()) {
		// Se marca al protagonista como recien destruido
		((miCurrentLives == 1)? mState = eDESTROYED : eREADY);
		miCurrentLives--;
	}

}

void Mustang::Crosshair() {
	// Crosshair  

	if (!IsAlive())
		return;

	cMatrix lmCrosshairTranslation, lmCrosshairRotation;

	lmCrosshairRotation.LoadRotation(cVec3(0.f, 0.f, 1.f), PI * mCHRotationCount);
	lmCrosshairTranslation.LoadTranslation(cVec3(mYaw - (cGame::Get().GetGameWidth() / 2), -mPitch + (cGame::Get().GetGameHeight() / 2), 0));
	cGraphicManager::Get().SetWorldMatrix(lmCrosshairRotation * lmCrosshairTranslation);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1,1,1,1);

	cTexture* lpTexture = (cTexture*) mCrosshair->GetResource();
	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBegin(GL_QUADS);
		glTexCoord2d(1,1); glVertex2f(30, 30);
		glTexCoord2d(1,0); glVertex2f(30, -30);
		glTexCoord2d(0,0); glVertex2f(-30, -30);
		glTexCoord2d(0,1); glVertex2f(-30, 30);
	glEnd();


	glDisable(GL_BLEND);  

	cGraphicManager::Get().SetWorldMatrix(lmCrosshairTranslation.LoadIdentity());

}