#include "Character.h"
#include "..\Graphics\GraphicManager.h"
#include "Behaviour\BehaviourManager.h"
#include "..\LuaManager\cLuaManager.h"
#include <typeinfo.h>

//M�todo que inicializa a unos valores por defecto los atributos de la clase.
void cCharacter::Init(int id)
{
	mfYaw = 0.0f;
	mPosition = cVec3(0.0f,0.0f,0.0f);
	mfSpeed = 1.5f;
	mfAngSpeed = 1.0f;
	//Por defecto un personaje no tiene comportamiento
	mpActiveBehaviour = NULL;
	//Identificador del personaje
	miId = id;
}

void cCharacter::Deinit()
{ 
	//Desactiva el comportamiento asociado al personaje
	mpActiveBehaviour->Deinit();
	cBehaviourManager::Get().DestroyBehaviour(mpActiveBehaviour);
}

void cCharacter::Update( float lfTimestep )
{ 
	mpActiveBehaviour->Update(lfTimestep);
	//mPosition.x += .005f;
	//mPosition.z += .002f;
}

//M�todo que renderiza un punto que representa la posici�n del personaje y una 
// peque�a l�nea que parte de dicho punto que representa la orientaci�n 
// del personaje.
void cCharacter::Render()
{
    /*La representaci�n del personaje debe estar siempre en espacio local, es decir, que no 
	se debe tener en cuenta su orientaci�n o posici�n. La posici�n y orientaci�n del 
	personaje, se aplicar�n a trav�s de la matriz de World. De esta forma, cuando se use 
	una malla para representar al personaje, la IA seguir� funcionando correctamente. */

	//Componer la matriz de World
	cMatrix lWorld;
	lWorld.LoadIdentity();
	lWorld.LoadRotation(cVec3(0.0f, 1.0f, 0.0f), mfYaw);
	lWorld.SetPosition( mPosition );
	
	//Establecer la matriz de World
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	//Dibujar el personaje (un punto para indicar su posici�n y una 
	// l�nea para indicar su orientaci�n) en espacio local. La  
	// orientaci�n del personaje por defecto ser� mirando al eje Z 
	// positivo.
	cGraphicManager::Get().DrawPoint( cVec3(0.0f, 0.0f, 0.0f), cVec3(1.0f, 0.0f, 0.0f) );
	cGraphicManager::Get().DrawLine( cVec3(0.0f, 0.0f, 0.0f), cVec3(0.0f, 0.0f, 1.0f), cVec3(1.0f, 0.0f, 0.0f) );

	//Si el comportamiento del personaje es patrullero, pintamos su circuito.
	std::string lsTipo = typeid(*mpActiveBehaviour).name();	
	if (strcmp(lsTipo.c_str(), "class cPatrol") == 0)
	{
		//Llamamos a la funci�n "CallLua" de "cLuaManager" para invocar a "DrawPath" de Lua.
		//Esta funci�n renderiza el circuito (Path) que debe seguir el patrullero cuyo id
		// se pasa como par�metro.	
		//Esta funci�n no retorna ning�n valor, por eso se pone NULL en el �ltimo par�metro de "CallLua".		
		bool lbOk = cLuaManager::Get().CallLua( "DrawPath", miId, (int *) NULL);
		assert( lbOk );//Se comprueba si la llamada a la funci�n se ha producido satisfactoriamente						
	}
	lWorld.LoadIdentity();
	//Reestablecer la matriz de World
	cGraphicManager::Get().SetWorldMatrix(lWorld);
}

//M�todo que establece el atributo de la posici�n del personaje
void cCharacter::SetPosition( const cVec3 &lPosition )
{
	mPosition = lPosition;
}

//M�todo que establece el atributo de la orientaci�n del personaje (yaw).
void cCharacter::SetYaw( float lfYaw )
{
	mfYaw = lfYaw;      
}

//M�todo que establece el atributo de la velocidad m�xima del personaje.
void cCharacter::SetSpeed( float lfSpeed )
{
	mfSpeed = lfSpeed;    
}

//M�todo que establece el atributo de la velocidad angular del personaje.
void cCharacter::SetAngSpeed( float lfAngSpeed )
{
	mfAngSpeed = lfAngSpeed;
}

//M�todo que establece el comportamiento del personaje.
void cCharacter::SetActiveBehaviour( cBehaviourBase * lpBehaviour ){
	mpActiveBehaviour = lpBehaviour;
	mpActiveBehaviour->Init(this);
}