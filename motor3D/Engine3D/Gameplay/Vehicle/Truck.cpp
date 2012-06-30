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
	mvPreviousYaw = 0.0f;

	// Callback pointer to get this object from bullet collisionShape
	mVehicle.m_carChassis->setUserPointer(this);
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

	float dist = lDistanceVec.Length();
	char buff1[256];
	sprintf(buff1, "Distancia al objetivo %2.2f\n", dist);
	//OutputDebugStr(buff1);
	//Si la distancia al objetivo es prácticamente cero, lo ha alcanzado.
	//Debemos poner esta condición, ya que debido a errores de precisión,
	//es posible que nunca se alcance una distancia 0.0f al objetivo y el
	//perseguidor permanezca dando vueltas alrededor de él.
	if (lDistanceVec.Length() < 0.1f)
	{
		OutputDebugStr("Se ha llegado al objetivo\n");
	}
	//else
	//	MoveForward(lfTimestep);

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
	if ( lfCosAngle > 1.0f )
	{
		lfCosAngle = 1.0f;
	}
	else if (lfCosAngle < -1.0f )
	{
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
	if ( lPlane.PointTest(mTarget) <= 0.4f && lPlane.PointTest(mTarget) >= -0.4f )
		mVehicle.gVehicleSteering = 0.0f;
	else {
		if ( lPlane.PointTest(mTarget) < 0.0f ) {
			//OutputDebugStr("Se debe desplazar a la izquierda\n");
			SteeringLeft(lfTimestep);
		}
		else {
			//OutputDebugStr("Se debe desplazar a la derecha\n");
			SteeringRight(lfTimestep);
		}
	}

	//Ahora el perseguidor debe desplazarse en la dirección de su nuevo
	//Front(ya que la orientación ha cambiado).

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
	float yaw = atan2f(lvDirection.x, lvDirection.z);

	float lyaw = yaw * 0.1 + mvPreviousYaw * (1 - 0.1);
	mvPreviousYaw = lyaw;

	lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), lyaw);

	lOffsetMatrix.LoadTranslation( cVec3(0.0f, 0.3f, 1.0f ) );
	lmPostTranslation.LoadTranslation(cVec3(0.f ,0.f, -1.05f));

	// Se posiciona el arma
	mTruckWea->SetWorldMatrix(lScaleMatrixChasis * lOffsetMatrix * lRotMatrix * lmPostTranslation * cPhysics::Get().Bullet2Local(mVehicle.m_vehicle->getChassisWorldTransform()));

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