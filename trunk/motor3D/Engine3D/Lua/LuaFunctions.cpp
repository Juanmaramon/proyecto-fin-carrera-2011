#include "LuaFunctions.h"
#include "../LuaManager/cLuaManager.h"
#include "../Character/Character.h"
#include "../Character/CharacterManager.h"
#include "../Character/Behaviour/BehaviourManager.h"
#include "../Graphics/GraphicManager.h"

//Esta funci�n ser� llamada desde Lua. Se encargar� de crear un personaje y asignarle un comportamiento.
//Adem�s, inicializar� el objetivo de este personaje, establecer� su velocidad m�xima, velocidad angular.
//Por �ltimo, devolver� el identificador del personaje creado.
int CreatePatrol(lua_State* lpLuaContext){

	//Comprobamos que el contexto de Lua no es NULL
	assert( lpLuaContext );

	//Cogemos el numero de argumentos de la pila de Lua
	int liArgCount = lua_gettop( lpLuaContext );

	//Comprobamos que recibimos cinco argumentos
	assert( liArgCount == 5 );

	//Cogemos los argumentos de la pila
	float liPosX = luaL_checknumber( lpLuaContext,1 );		//1� argumento
	float liPosY = luaL_checknumber( lpLuaContext,2 );		//2� argumento
	float liPosZ = luaL_checknumber( lpLuaContext,3 );		//3� argumento
	float liSpeed = luaL_checknumber( lpLuaContext,4 );		//4� argumento
	float liAngSpeed = luaL_checknumber( lpLuaContext,5 );	//5� argumento
	
	//Se crea un personaje.
	cCharacter *lpCharacterPrueba = cCharacterManager::Get().CreateCharacter();	

	//Se establece su posici�n.
	lpCharacterPrueba->SetPosition( cVec3(-4.0f, 0.0f, -2.0f) );
	//Se establece su velocidad m�xima y angular.
	lpCharacterPrueba->SetSpeed(liSpeed);
	lpCharacterPrueba->SetAngSpeed(liAngSpeed);

	//Se crea el comportamiento de perseguidor.
	cBehaviourBase *lpBehaviour = cBehaviourManager::Get().CreateBehaviour(cBehaviourManager::ePATROL);
	//Se incializa el comportamiento.
	//lpBehaviour->Init(lpCharacterPrueba);

   //Se asigna el comportamiento al personaje.
   lpCharacterPrueba->SetActiveBehaviour(lpBehaviour);

	int liId = lpCharacterPrueba->GetId();
	//Introducimos en la pila el identificador del personaje
	lua_pushinteger( lpLuaContext, liId );
	
	//Devolvemos el numero de valores de retorno
	//que hemos introducido en la pila
	return 1;
}

//Esta funci�n ser� llamada desde Lua. Se encargar� de cambiar el objetivo al siguiente punto
//para el personaje cuyo id sea igual a CharacterId. 
int SetPatrolTarget(lua_State* lpLuaContext ){
	//Comprobamos que el contexto de Lua no es NULL
	assert( lpLuaContext );

	//Cogemos el numero de argumentos de la pila de Lua
	int liArgCount = lua_gettop( lpLuaContext );

	//Comprobamos que recibimos cinco argumentos
	assert( liArgCount == 4 );

	//Cogemos los argumentos de la pila
	int liCharacterId = luaL_checknumber( lpLuaContext,1 );		//1� argumento
	float lfPosX = luaL_checknumber( lpLuaContext,2 );				//2� argumento
	float lfPosY = luaL_checknumber( lpLuaContext,3 );				//3� argumento
	float lfPosZ = luaL_checknumber( lpLuaContext,4 );				//4� argumento
	
	//Se busca el personaje con Id igual a liCharacterId.
	cCharacter *lpCharacterPrueba = cCharacterManager::Get().GetById(liCharacterId);	

	assert(lpCharacterPrueba);

	//Se establece el siguiente punto a perseguir (mTargetWayPoint).
	//Se convierte el padre (cBehaviourBase) al hijo (cPatrol).
	//El m�todo SetTargetWaypoint es propio de cPatrol.
	( (cPatrol *)lpCharacterPrueba->GetActiveBehaviour() )->SetTargetWaypoint(cVec3(lfPosX, lfPosY, lfPosZ));

	int liId = lpCharacterPrueba->GetId();
	//Introducimos en la pila el identificador del personaje
	lua_pushinteger( lpLuaContext, liId );
	
	//Devolvemos el numero de valores de retorno
	//que hemos introducido en la pila
	return 1;
}


//Funci�n que dibuja de un determinado color la l�nea 
// que une dos puntos (a,b) del circuito de un patrullero.
//Ser� llamada desde Lua de la siguiente manera:
//  DrawLine(lfPosXa, lfPosYa, lfPosZa, lfPosXb, lfPosYb, lfPosZb)
//Lua har� tantas llamadas a esta funci�n como sea neceario para renderiazar todo el circuito.
int DrawLine( lua_State* lpLuaContext )
{
	//Comprobamos que el contexto de Lua no es NULL
	assert( lpLuaContext );

	//Cogemos el numero de argumentos de la pila de Lua
	int liArgCount = lua_gettop( lpLuaContext ); 

	//Comprobamos que recibimos 6 argumentos (lfPosXa, lfPosYa, lfPosZa, lfPosXb, lfPosYb, lfPosZb)
	assert( liArgCount == 6 );

	//Cogemos los argumentos de la pila   
	float lfPosXa = (float)luaL_checknumber( lpLuaContext,1 ); //1� argumento
	float lfPosYa = (float)luaL_checknumber( lpLuaContext,2 ); //2� argumento
	float lfPosZa = (float)luaL_checknumber( lpLuaContext,3 ); //3� argumento 
	float lfPosXb = (float)luaL_checknumber( lpLuaContext,4 ); //4� argumento
	float lfPosYb = (float)luaL_checknumber( lpLuaContext,5 ); //5� argumento
	float lfPosZb = (float)luaL_checknumber( lpLuaContext,6 ); //6� argumento             

	// Set the world matrix
	cMatrix lWorld;
	lWorld.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	//Dibujamos los dos puntos
    cGraphicManager::Get().DrawPoint( cVec3(lfPosXa, lfPosYa, lfPosZa), cVec3(-1.0f, 0.0f, 1.0f) );
	cGraphicManager::Get().DrawPoint( cVec3(lfPosXb, lfPosYb, lfPosZb), cVec3(-1.0f, 0.0f, 1.0f) );
	//Dibujamos la l�nea que une los dos puntos
	cGraphicManager::Get().DrawLine( cVec3(lfPosXa, lfPosYa, lfPosZa), cVec3(lfPosXb, lfPosYb, lfPosZb), cVec3(1.0f, 0.0f, 1.0f) );

	//Devolvemos el n�mero de valores de retorno
	//que hemos introducido en la pila
	return 0;
}

//Funci�n que registrar� en Lua las funciones C++
void RegisterLuaFunctions(){
	cLuaManager::Get().Register("CreatePatrol", CreatePatrol);
	cLuaManager::Get().Register("SetPatrolTarget", SetPatrolTarget);
	cLuaManager::Get().Register("DrawLine", DrawLine);
}