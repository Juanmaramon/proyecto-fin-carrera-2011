#include "Truck.h"
#include "..\..\Physics\cPhysics.h"
#include "..\..\Physics\cPhysicObject.h"
#include "..\..\Graphics\GraphicManager.h" 
#include "..\..\Game\Game.h"
#include "..\..\Graphics\Camera.h"
#include "..\..\Physics\cPhysics.h"
#include "Mustang.h"
#include "../../Graphics/Textures/Texture.h"

void Truck::Init(cVec3 ini_pos, cObject* truckExterior, cObject* truckWeapon, cObject* truckTires,
				 cResourceHandle* weapon_muzzle1, cResourceHandle* weapon_muzzle2, cResourceHandle* weapon_muzzle3,
				 cObject* truckExteriorDestroyed, cObject* truckWeaponDestroyed, cResourceHandle* explosion_sprite,
				 cResourceHandle* explosion_sprite1, cResourceHandle* particle_texture) {

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
	lTransform.LoadTranslation(ini_pos);
	mVehicle.m_carChassis->setWorldTransform(cPhysics::Get().Local2Bullet(lTransform));
	mvPreviousYaw = 0.0f;

	// Callback pointer to get this object from bullet collisionShape
	mVehicle.m_carChassis->setUserPointer(this);

	// Contador de vidas
	miCurrentLives = miMaxLives = 250;

	// Texturas de fogonazo arma
	mWeaponMuzzle1 = weapon_muzzle1;
	mWeaponMuzzle2 = weapon_muzzle2;
	mWeaponMuzzle3 = weapon_muzzle3;

	// Modelo destruido
	mTruckExtDes = truckExteriorDestroyed;
	mTruckWeaDes = truckWeaponDestroyed;

	// Explosion
	mExplosion.Init(explosion_sprite, explosion_sprite1, weapon_muzzle1, particle_texture);

	mState = eREADY;

	// Inicialmente no recibe daño
	mbDamaged = false;
	// Contador para actualizar estado dañado
	miStepsDamaged = 0;
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

void Truck::Update(float lfTimestep){
	bool res = IsAlive();	

	char buff1[256];
	sprintf(buff1, "Contador de vidas: %d\n", miCurrentLives);
	//OutputDebugStr(buff1);

	float dist;
	// Si no esta con vida, no se actualiza
	if (!IsAlive()) {

		// Activa animacion de explosion (solo recien destruido)
		if (mState == eDESTROYED)
			mExplosion.SetAlive(true);
	
		if (mState == eDESTROYED)
			mState = eDEATH;
	
		if (mState == eDEATH) {
			mExplosion.SetPosition(mTruckExtDes->GetWorldMatrix());
			mExplosion.SetParticlesPosition(mTruckExtDes->GetWorldMatrix());
		}

		Break(lfTimestep);
		mTruckExtDes->SetWorldMatrix(mTruckExt->GetWorldMatrix());
		// Correccion sobre la posicion de la torreta (un poco bajada)
		cMatrix lmWeaFix;
		lmWeaFix.LoadTranslation(cVec3(0.f, -0.65f, 0.f));
		mTruckWeaDes->SetWorldMatrix(mTruckWea->GetWorldMatrix() * lmWeaFix);
	
	} else {

		//Calcular el vector distancia (diferencia entre la posición del
		//perseguidor y la posición del objetivo a perseguir)
		cVec3 mTarget = cGame::Get().GetMustang().GetVehicleBullet()->GetChasisPos();
		cVec3 lCharacterPosition = cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).GetPosition();
		cVec3 lDistanceVec;

		lDistanceVec.x = mTarget.x - lCharacterPosition.x;
		lDistanceVec.y = mTarget.y - lCharacterPosition.y;
		lDistanceVec.z = mTarget.z - lCharacterPosition.z;

		//Calcular la distancia que se moverá el perseguidor teniendo en
		//cuenta su velocidad máxima
		//float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;

		dist = lDistanceVec.Length();

		sprintf(buff1, "Distancia al objetivo %2.2f\n", dist);
		//OutputDebugStr(buff1);
		//Si la distancia al objetivo es prácticamente cero, lo ha alcanzado.
		//Debemos poner esta condición, ya que debido a errores de precisión,
		//es posible que nunca se alcance una distancia 0.0f al objetivo y el
		//perseguidor permanezca dando vueltas alrededor de él.
		if (lDistanceVec.Length() < 0.1f) {
			OutputDebugStr("Se ha llegado al objetivo\n");
		}
		else
			MoveForward(lfTimestep);

		//Para mover la orientación del perseguidor es necesario calcular el
		//ángulo que hay entre el Front del perseguidor y el vector Distance
		//(el cual marca la dirección en la que está el objetivo). Usaremos
		//para ello el Dot Product que nos devuelve el coseno del ángulo que
		//buscamos.
		float lfCosAngle = Dot( cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).GetFront().Normalize(), lDistanceVec.Normalize() );
		//El Dot Product (producto escalar) devuelve el coseno del ángulo
		//entre dos vectores si éstos están normalizados. Debe devolver un
		//valor entre [-1,1]. En ocasiones, por problemas internos de redondeo
		//da valores algo mayores (como 1.0000001f) que pueden dar problemas.
		//Por ello añadimos el siguiente código:
		if ( lfCosAngle > 1.0f ){
			lfCosAngle = 1.0f;
		} else if (lfCosAngle < -1.0f ) {
			lfCosAngle = -1.0f;
		}
		//Ahora podemos calcular cuál es ese ángulo, es decir, cuántos
		//radianes se debe girar el perseguidor para alinearse con el punto
		//objetivo teniendo en cuenta la orientación actual(sabemos que el
		//valor de lfCosAngle va a estar entre [-1,1] con lo que no tendremos
		//problemas al llamar a acosf)
		float lfAngle = acosf(lfCosAngle);
		sprintf(buff1, "Angulo de giro: %2.2f\n", lfAngle);
		//OutputDebugStr(buff1);

		if ( lfAngle <= 0.6f && lfAngle >= -0.6f /*&& lDistanceVec.Length() >= 50.f*/ )
			mVehicle.gVehicleSteering = 0.0f;	
		
		//Calcular cuántos radianes se puede mover el perseguidor en este
		//frame teniendo en cuenta su velocidad angular
		//float lfAngleDisplacement = mpCharacter->GetAngSpeed() * lfTimestep;
		//Si la cantidad de radianes que el personaje se puede mover este
		//frame (acorde a su velocidad angular) es menor que la cantidad de
		//radianes que debería girar para alinearse al objetivo, restringimos
		//la rotación que vamos a realizar
		/*if (lfAngle > lfAngleDisplacement )
		{
			lfAngle = lfAngleDisplacement;
		}*/
		//Ahora se sabe cuántos radianes se debe mover el perseguidor, pero
		//¿hacia qué lado?¿A su derecha o hacia su izquierda? Para ello se
		//debe hacer un Test del plano. Pasos:
		//Necesitamos el vector Right del perseguidor que será la normal del
		//plano que tenemos que usar. Este plano contiene al vector Front del
		//personaje y nos servirá para determinar si el punto objetivo está a
		//la izquierda o derecha del perseguidor.
		//Hacemos el Test de plano. Si la posición del perseguidor esta
		//contenido en el plano (=0), a la derecha del Front (<0) o a la
		//izquierda del Front (>0)

		cPlane lPlane;
		lPlane.Init( cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).GetRight(), cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).GetPosition()); 
		sprintf(buff1, "Desplazamiento: %2.2f\n", lPlane.PointTest(mTarget));
		//OutputDebugStr(buff1);
		/*if ( lPlane.PointTest(mTarget) <= 0.4f && lPlane.PointTest(mTarget) >= -0.4f && lDistanceVec.Length() < 50.f )
			mVehicle.gVehicleSteering = 0.0f;
		else {*/
			if ( lPlane.PointTest(mTarget) < 0.0f ) {
				//OutputDebugStr("Se debe desplazar a la izquierda\n");
				SteeringLeft(lfTimestep);
			}
			else {
				//OutputDebugStr("Se debe desplazar a la derecha\n");
				SteeringRight(lfTimestep);
			}
		//}
	}

	mVehicle.Update();

	cMatrix lTransMatrix, lRotMatrix, lOffsetMatrix;
	lTransMatrix.LoadIdentity();
	lRotMatrix.LoadIdentity();
	lOffsetMatrix.LoadIdentity();

	lOffsetMatrix.LoadTranslation( cVec3(0.0f, 0.300f, -0.040f ) );
	// Acutlizacion de chasis, interior y metralleta
	mTruckExt->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));

	// La metralleta rota junto al chasis pero tambien siguiendo al jugador
	cMatrix lmPostTranslation;
	lmPostTranslation.LoadIdentity();
	
	// Calculo de la posicion del arma (se pasa la posicion del protagonista al espacio del vehiculo truck)
	cMatrix lvPosition = cPhysics::Get().Bullet2Local(cGame::Get().GetMustang().GetVehicleBullet()->m_vehicle->getChassisWorldTransform()) * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()).Invert(); //->GetChasisPos();

	// La direccion del arma será la posicion del jugador menos la posicion del arma (que al estar en la posicion 0,0,0 se desprecia)
	cVec3 lvDirection = lvPosition.GetPosition(); //- mTruckWea->GetWorldMatrix().GetPosition();

	float lyaw;
	// Si esta vivo actualiza la rotacíon de la torreta
	if (IsAlive()) {
 
		float yaw = atan2f(lvDirection.x, lvDirection.z);

		lyaw = yaw * 0.1 + mvPreviousYaw * (1 - 0.1);

		mvPreviousYaw = lyaw;

	// Si no esta vivo no actualiza rotacion
	} else
		lyaw = mvPreviousYaw;

	lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), lyaw);

	lOffsetMatrix.LoadTranslation( cVec3(0.0f, 0.3f, 1.0f ) );
	lmPostTranslation.LoadTranslation(cVec3(0.f ,0.f, -1.05f));

	// Se posiciona el arma
	mTruckWea->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * lRotMatrix * lmPostTranslation * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));


	// Si se esta disparando, raytest para simular el rayo del laser de la ametralladora
	if (dist <= 300.0f && IsAlive()) {
		mbIsFiring = true;
		mbLaserHit = false;
		
		// Start and End are vectors
		cVec3 lvEnd;
		cVec3 lvStart;

		TransformPoint( lvStart, cVec3(-0.0f, 18.0f, 3.0f ), mTruckWea->GetWorldMatrix() );
		TransformPoint( lvEnd, cVec3(-0.0f, 18.0f, WEAPON_RANGE), mTruckWea->GetWorldMatrix() );

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

				Mustang* lpBodyPtr = (Mustang*)body->getUserPointer();

				if (lpBodyPtr != NULL) {
					sprintf(buff1, "Hit! Hit!\n");
					OutputDebugStr(buff1);
					//lpBodyPtr->Damage();
				}
				else {
					sprintf(buff1, "\n\n\n");
					//OutputDebugStr(buff1);
				}
			}
		}

		cVec3 lvHit;
		if (mbLaserHit)
			TransformPoint( lvHit, mvRayHitPosition, mTruckWea->GetWorldMatrix().Invert() );
		else
			lvHit = lvEnd;

		// Ahora se calcula la trayectoria de la bala real
		// Antes se calculo el rayo del laser de la torreta
		float a = -3000.0f;
		float b = 3000.0f;
		float deviation_x = ((b - a) * ((float)rand() / RAND_MAX)) + a;
		float deviation_y = ((b - a) * ((float)rand() / RAND_MAX)) + a;
		float deviation_z = ((b - a) * ((float)rand() / RAND_MAX)) + a;

		// Este es el punto final con las direccion aleatorio
		cVec3 lvEndPoint = cVec3(-0.f + deviation_x, 11.f + deviation_y, WEAPON_RANGE +  deviation_z);

		TransformPoint( lvStart, cVec3(-0.0f, 11.0f, 3.0f ), mTruckWea->GetWorldMatrix() );
		TransformPoint( lvEnd, lvEndPoint, mTruckWea->GetWorldMatrix() );

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

				Mustang* lpBodyPtr = (Mustang*)body->getUserPointer();

				if (lpBodyPtr != NULL) {
					sprintf(buff1, "Hit! Hit!\n");
					//OutputDebugStr(buff1);
					lpBodyPtr->Damage();
					lpBodyPtr->GettingDamage(true);
				}
				else {
					sprintf(buff1, "\n\n\n");
					//OutputDebugStr(buff1);
				}
			}
		}

		//mBullets.Create(lvStart, lvHit, lfyaw);
	}

	lRotMatrix.LoadIdentity();

	// Actualizacion ruedas
	for (int i=0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {

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
			maTires[i].SetWorldMatrix(mScaleMatrix * lOffsetMatrix * rotate *  lRotMatrix * lTransMatrix);
		}else{
			lmPostTranslation.LoadTranslation(cVec3(-0.25f ,0.f, 0.0f));
			maTires[i].SetWorldMatrix(mScaleMatrix * lOffsetMatrix * lRotMatrix * lTransMatrix);
		}

	}	
}

void Truck::Render(){

	// Renderiza rueda
	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i].Render();
	}

	if (IsAlive()) {
	
		// Renderiza arma
		mTruckWea->Render();

		// Renderiza chasis
		mTruckExt->Render();
	
		//mVehicle.renderme();
		
		// Renderiza fogonazo del arma y laser
		glDisable (GL_FOG);

		//RenderRayGun ();

		// Si su objetivo esta vivo
		if (cGame::Get().GetMustang().IsAlive())
			RenderMuzzle();

		RenderHUD();

		glEnable (GL_FOG);

	} else {
		
		// Si esta destruido reemplaza el modelo por el coche destrozado
		
		// Renderiza arma destruida
		mTruckWeaDes->Render();

		if (mState == eDEATH)
			mExplosion.RenderSmoke();

		// Renderiza chasis destruido
		mTruckExtDes->Render();

		mExplosion.Render();

	}
}

void Truck::Deinit(){
	GetVehicleBullet()->~Vehicle();

	for (int i = 0; i < mVehicle.m_vehicle->getNumWheels() ; i++) {
		maTires[i].Deinit();
	}

	maTires.empty();
}

bool Truck::IsAlive() {

	return ((miCurrentLives > 0)? true : false);

}

void Truck::Damage() {

	if (IsAlive()) {
		// Se marca al enemigo como recien destruido
		((miCurrentLives == 1)? mState = eDESTROYED : eREADY);
		miCurrentLives--;
	}

}

void Truck::RenderRayGun () {

	// Si se esta disparando el arma
	if (mbIsFiring && IsAlive()) {
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
		cGraphicManager::Get().SetWorldMatrix(mTruckWea->GetWorldMatrix());

		cVec3 lvHitPoint;

		// Se renderiza el rayo del laser de la torreta
		// Si el rayo choca con un objeto entonces se para en ese objeto, si no hasta el alcance del arma 
		if (mbLaserHit)
			TransformPoint( lvHitPoint, mvRayLaserHitPosition, mTruckWea->GetWorldMatrix().Invert() );
		else
			lvHitPoint = cVec3(-0.0f, 18.0f,  WEAPON_RANGE);

			cGraphicManager::Get().DrawLine(cVec3(-0.0f, 18.0f, 3.0f ), lvHitPoint, cVec3(1.f, 0.f, 0.f), 0.5f);
			cGraphicManager::Get().DrawSphere(lvHitPoint, cVec3(1.f, 0.f, 0.f), 0.5f);

		// Ahora se renderiza la bala real con el desplazamiento calculado
		if (mbHit)
			TransformPoint( lvHitPoint, mvRayHitPosition, mTruckWea->GetWorldMatrix().Invert() );
		else
			lvHitPoint = mvRayHitPosition;

		char buff1[256];
		sprintf(buff1, "Punto de impacto: (%2.2f , %2.2f , %2.2f) \n", lvHitPoint.x, lvHitPoint.y, lvHitPoint.z);
		//OutputDebugStr(buff1);

			cGraphicManager::Get().DrawLine(cVec3(-0.0f, 18.0f, 3.0f ), cVec3(lvHitPoint.x, lvHitPoint.y,lvHitPoint.z), cVec3(0.f, 0.f, 1.f), 0.2f);
			cGraphicManager::Get().DrawSphere(lvHitPoint, cVec3(0.f, 0.f, 1.f), 0.2f);

		cGraphicManager::Get().SetWorldMatrix(lmPosMatrix.LoadIdentity());

		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
		glColor4f(1,1,1,1.f);
	}

}

void Truck::RenderMuzzle() {

	if (mbIsFiring && IsAlive()) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		cVec3 lvMuzzlePos;
		TransformPoint( lvMuzzlePos, cVec3(-0.0f, 18.0f, 3.0f ), mTruckWea->GetWorldMatrix() );

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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
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

void Truck::RenderHUD() {

	if (IsAlive()) {

		if (!mbDamaged) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ONE);
		}

		cVec3 lvHUDPos, lvHUDPosBright;
		TransformPoint( lvHUDPos, cVec3(-15.0f, 26.0f, 0.0f ), mTruckWea->GetWorldMatrix() );
		TransformPoint( lvHUDPosBright, cVec3(-15.0f, 22.0f, 0.0f ), mTruckWea->GetWorldMatrix() );

		cMatrix lmBillboardMatrix;
		lmBillboardMatrix.LoadTranslation(lvHUDPos);
		cGraphicManager::Get().SetWorldMatrix(lmBillboardMatrix);

		// Se aplica el billboard para ignorar la rotacion y orientar el quad a la camara
		cGraphicManager::Get().BillboardCheatSphericalBegin();

		glColor4f (1.f, 1.f, 1.f, 1.f);
		
		// Health Bar

		float lfPercentLives = (float)miCurrentLives / (float)miMaxLives;

		float lfRedComponent = (float)1.f - lfPercentLives;

		glDisable(GL_TEXTURE_2D);
			cGraphicManager::Get().DrawRect(0.f, 0.f, lfPercentLives * 2.5f, 0.15f, cVec3(lfRedComponent, lfPercentLives, 0.f));

		glEnable(GL_TEXTURE_2D);

		cGraphicManager::Get().SetWorldMatrix(lmBillboardMatrix.LoadIdentity());

		cGraphicManager::Get().BillboardEnd();

		if (!mbDamaged)
			glDisable(GL_BLEND);

		glColor4f (1.f, 1.f, 1.f, 1.f);

	}

}

void Truck::GettingDamage(bool lbDamage) {
	// Si esta en estado dañado esperar 200 frames para actualizar
	if (mbDamaged) {
		if (miStepsDamaged == 200) {
			miStepsDamaged = 0;
			mbDamaged = lbDamage;
		} else
			miStepsDamaged++;
	} else
		mbDamaged = lbDamage;
}