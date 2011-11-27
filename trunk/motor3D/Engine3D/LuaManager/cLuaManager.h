//Clase que encapsula todos lo m�todos necesarios para trabajar con la librer�a LUA

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <string>//Para usar std::string
#include "..\Utility\Singleton.h"

// extern C le dice al compilador que la libreria debe ser compilada en C y no en C++ 
extern "C"
{
   #include "../Lua/src/lua.h"
   #include "../Lua/src/lauxlib.h"
   #include "../Lua/src/lualib.h"
}

//Esta clase usar� el patr�n Singleton definido en "..Utility/Singleton.h"
class cLuaManager : public cSingleton<cLuaManager>
{
	public:
		void Init();
		bool DoString( const char *lacStatement );	
		bool DoFile( const char* lacFile );
		void Register( const char* lacFuncName, lua_CFunction lpFunc );
		template <class Z> bool CallLua( const char* lacFuncName, Z* lpRet );
		template <class T, class Z> bool CallLua( const char* lacFuncName, T lArg, Z* lpRet);
		template <class T, class U, class Z> bool CallLua( const char* lacFuncName, T lArg1, U lArg2, Z* lpRet );
		template <class T, class U, class V, class Z> bool CallLua( const char* lacFuncName, T lArg1, U lArg2, V lArg3, Z* lpRet);
		void Deinit();
		inline lua_State* GetContext() { return mpLuaContext; }
		friend class cSingleton<cLuaManager>;
	
	protected:
		cLuaManager(); // Protected constructor
	
	private:
		/*Una variable de tipo "lua_state" mantiene una traza de qu� funciones han sido llamadas, 
		qu� variables han sido definidas, sus valores y otro tipo de informaci�n que se necesita 
		durante la ejecuci�n. A este estado tambi�n se le suele llamar contexto de ejecuci�n.*/
		lua_State* mpLuaContext;

		bool CheckError( int liError );
		static int FuncPanic(lua_State *lpContext);

		void PushArg( int liArg );
		void PushArg( long liArg );
		void PushArg( float lfArg );
		void PushArg( const char* lacArg );
		void PushArg( std::string &lacArg );
		void PushArg( bool lbArg );

		void GetRetValue( int* liRet );
		void GetRetValue( long* liRet );
		void GetRetValue( float* lfRet );
		void GetRetValue( const char* lacRet );
		void GetRetValue( std::string* lacRet );
		void GetRetValue( bool* lbRet );

		void Pop( unsigned int luiNValues );
};

/*Los m�todos de tipo template se definen aqu� en el .h por fuera de la clase*/

//M�todo  que se encargar� de realizar todos los pasos necesarios para llamar a
// una funci�n definida en Lua que no tenga argumentos.
//Par�metros: "lacFuncName", nombre de la funci�n definida en Lua; 
// "lpRet", puntero donde se almacena el valor de retorno si lo hay. 
// Si "lpRet" es NULL significar� que la funci�n que queremos llamar no devuelve ning�n valor. En 
// caso contrario, la funci�n devuelve una valor que se almacenar� en lpRet.
// "CallLua" se define como un template porque los argumentos pueden ser de cualquier tipo.
template <class Z> bool cLuaManager::CallLua( const char* lacFuncName, Z* lpRet )
{
	assert( mpLuaContext );

	//1.Introducir en la pila el nombre de la funcion definida en Lua que
	//  queremos llamar.
	lua_getglobal( mpLuaContext, lacFuncName );

    //Antes de poder usar una funci�n de Lua es necesario que se cargue el fichero
	// (script .lua) que contiene su definici�n. Esto se hace normalmente en el "Main.cpp->main()".

	//Chequeamos que la funci�n esta definida en Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );
	//Si no est� definida en Lua
	if ( !lbExists )
	{
		//Sacamos el nombre de la funci�n de la pila de Lua
		Pop( 1 );
		printf( "Error: la funcion %s no existe en Lua\n", lacFuncName );
		return false;
	}

	//Si la funcion est� definida en Lua...
	
	//2.Obtener el valor de retorno (si lo hay)
	//Si la funcion devuelve un valor, el puntero "lpRet" no ser� nulo.
	if ( lpRet )
	{
		//Llamamos a la funci�n Lua que se insert� en el punto 1. para el contexto dado,
		//  indicando que tiene 0 par�metros y 1 valor de retorno.
		lua_call( mpLuaContext, 0, 1 );

		//Obtenemos el resultado (valor de retorno) del tope de la pila (la funci�n "GetRetValue" est� sobrecargada: dependiendo del 
		//  tipo del argumento, se llamar� autom�ticamente a la funci�n correspondiente).
		GetRetValue( lpRet );
		
		//3.Eliminar el valor de retorno de la pila.
		Pop( 1 );
	}
	//La funci�n no devuelve ning�n valor
	else
	{
		//Llamamos a la funci�n Lua que se insert� en el punto 1. para el contexto dado,
		//  indicando que tiene 0 par�metros y 0 valores de retorno.
		lua_call( mpLuaContext, 0, 0 );
	}
	return true;
}

//M�todo  que se encargar� de realizar todos los pasos necesarios para llamar a
// una funci�n definida en Lua que tenga 1 argumento.
//Par�metros: "lacFuncName", nombre de la funci�n definida en Lua; "lArg", �nico argumento de la funci�n,
// "lpRet", puntero donde se almacena el valor de retorno si lo hay. 
// Si "lpRet" es NULL significar� que la funci�n que queremos llamar no devuelve ning�n valor. En 
// caso contrario, la funci�n devuelve una valor que se almacenar� en lpRet.
// "CallLua" se define como un template porque los argumentos pueden ser de cualquier tipo.
template <class T, class Z> bool cLuaManager::CallLua( const char* lacFuncName, T lArg, Z* lpRet)
{
	assert( mpLuaContext );

	//1.Introducir en la pila el nombre de la funcion definida en Lua que
	//  queremos llamar.
	lua_getglobal( mpLuaContext, lacFuncName );

    //Antes de poder usar una funci�n de Lua es necesario que se cargue el fichero
	// (script .lua) que contiene su definici�n. Esto se hace normalmente en el "Main.cpp->main()".

	//Chequeamos que la funci�n esta definida en Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );
	//Si no est� definida en Lua
	if ( !lbExists )
	{
		//Sacamos el nombre de la funci�n de la pila de Lua
		Pop( 1 );
		printf( "Error: la funcion %s no existe en Lua\n", lacFuncName );
		return false;
	}

	//Si la funcion est� definida en Lua...

	//2.Introducir el argumento en la pila (la funci�n "PushArg" est� sobrecargada: dependiendo del 
	//  tipo del argumento, se llamar� autom�ticamente a la funci�n correspondiente).	
	PushArg(lArg);
	
	//3.Obtener el valor de retorno (si lo hay)
	//Si la funcion devuelve un valor, el puntero "lpRet" no ser� nulo.
	if ( lpRet )
	{
		//Llamamos a la funci�n Lua que se insert� en el punto 1. para el contexto dado,
		//  indicando que tiene 1 par�metro y 1 valor de retorno.
		lua_call( mpLuaContext, 1, 1 );

		//Obtenemos el resultado (valor de retorno) del tope de la pila (la funci�n "GetRetValue" est� sobrecargada: dependiendo del 
		//  tipo del argumento, se llamar� autom�ticamente a la funci�n correspondiente).
		GetRetValue( lpRet );
		
		//4.Eliminar el valor de retorno de la pila.
		Pop( 1 );
	}
	//La funci�n no devuelve ning�n valor
	else
	{
		//Llamamos a la funci�n Lua que se insert� en el punto 1. para el contexto dado,
		//  indicando que tiene 1 par�metro y 0 valores de retorno.
		lua_call( mpLuaContext, 1, 0 );
	}
	return true;
}

//M�todo  que se encargar� de realizar todos los pasos necesarios para llamar a
// una funci�n definida en Lua que tenga 3 argumentos.
//Par�metros: "lacFuncName", nombre de la funci�n definida en Lua; "lArg1", primer argumento de la funci�n;
// "lArg2", segundo argumento de la funci�n; "lpRet", puntero donde se almacena el valor de retorno si lo hay. 
// Si "lpRet" es NULL significar� que la funci�n que queremos llamar no devuelve ning�n valor. En 
// caso contrario, la funci�n devuelve una valor que se almacenar� en lpRet.
// "CallLua" se define como un template porque los argumentos pueden ser de cualquier tipo.
template <class T, class U, class Z> bool cLuaManager::CallLua( const char* lacFuncName, T lArg1, U lArg2, Z* lpRet)
{
	assert( mpLuaContext );

	//1.Introducir en la pila el nombre de la funcion definida en Lua que
	//  queremos llamar.
	lua_getglobal( mpLuaContext, lacFuncName );

    //Antes de poder usar una funci�n de Lua es necesario que se cargue el fichero
	// (script .lua) que contiene su definici�n. Esto se hace normalmente en el "Main.cpp->main()".

	//Chequeamos que la funci�n esta definida en Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );
	//Si no est� definida en Lua
	if ( !lbExists )
	{
		//Sacamos el nombre de la funci�n de la pila de Lua
		Pop( 1 );
		printf( "Error: la funcion %s no existe en Lua\n", lacFuncName );
		return false;
	}

	//Si la funcion est� definida en Lua...

	//2.Introducir los argumentos en la pila (la funci�n "PushArg" est� sobrecargada: dependiendo del 
	//  tipo del argumento, se llamar� autom�ticamente a la funci�n correspondiente).
	//Introducimos en la pila el primer argumento
	PushArg(lArg1);
	//Introducimos en la pila el segundo argumento
	PushArg(lArg2);

	//3.Obtener el valor de retorno (si lo hay)
	//Si la funcion devuelve un valor, el puntero "lpRet" no ser� nulo.
	if ( lpRet )
	{
		//Llamamos a la funci�n Lua que se insert� en el punto 1. para el contexto dado,
		//  indicando que tiene 2 par�metros y 1 valor de retorno.
		lua_call( mpLuaContext, 2, 1 );

		//Obtenemos el resultado (valor de retorno) del tope de la pila (la funci�n "GetRetValue" est� sobrecargada: dependiendo del 
		//  tipo del argumento, se llamar� autom�ticamente a la funci�n correspondiente).
		GetRetValue( lpRet );
		
		//4.Eliminar el valor de retorno de la pila.
		Pop( 1 );
	}
	//La funci�n no devuelve ning�n valor
	else
	{
		//Llamamos a la funci�n Lua que se insert� en el punto 1. para el contexto dado,
		//  indicando que tiene 2 par�metros y 0 valores de retorno.
		lua_call( mpLuaContext, 2, 0 );
	}
	return true;
}

//M�todo  que se encargar� de realizar todos los pasos necesarios para llamar a
// una funci�n definida en Lua que tenga 3 argumentos.
//Par�metros: "lacFuncName", nombre de la funci�n definida en Lua; "lArg1", primer argumento de la funci�n;
// "lArg2", segundo argumento de la funci�n; "lArg3", tercer argumento;
// "lpRet", puntero donde se almacena el valor de retorno si lo hay. 
// Si "lpRet" es NULL significar� que la funci�n que queremos llamar no devuelve ning�n valor. En 
// caso contrario, la funci�n devuelve una valor que se almacenar� en lpRet.
// "CallLua" se define como un template porque los argumentos pueden ser de cualquier tipo.
template <class T, class U, class V, class Z> bool cLuaManager::CallLua( const char* lacFuncName, T lArg1, U lArg2, V lArg3, Z* lpRet)
{
	assert( mpLuaContext );

	//1.Introducir en la pila el nombre de la funcion definida en Lua que
	//  queremos llamar.
	lua_getglobal( mpLuaContext, lacFuncName );

    //Antes de poder usar una funci�n de Lua es necesario que se cargue el fichero
	// (script .lua) que contiene su definici�n. Esto se hace normalmente en el "Main.cpp->main()".

	//Chequeamos que la funci�n esta definida en Lua
	int lbExists = lua_isfunction( mpLuaContext, -1 );
	//Si no est� definida en Lua
	if ( !lbExists )
	{
		//Sacamos el nombre de la funci�n de la pila de Lua
		Pop( 1 );
		printf( "Error: la funcion %s no existe en Lua\n", lacFuncName );
		return false;
	}

	//Si la funcion est� definida en Lua...

	//2.Introducir los argumentos en la pila (la funci�n "PushArg" est� sobrecargada: dependiendo del 
	//  tipo del argumento, se llamar� autom�ticamente a la funci�n correspondiente).
	//Introducimos en la pila el primer argumento
	PushArg(lArg1);
	//Introducimos en la pila el segundo argumento
	PushArg(lArg2);
	//Introducimos en la pila el tercer argumento
	PushArg(lArg3);

	//3.Obtener el valor de retorno (si lo hay)
	//Si la funcion devuelve un valor, el puntero "lpRet" no ser� nulo.
	if ( lpRet )
	{
		//Llamamos a la funci�n Lua que se insert� en el punto 1. para el contexto dado,
		//  indicando que tiene 3 par�metros y 1 valor de retorno.
		lua_call( mpLuaContext, 3, 1 );

		//Obtenemos el resultado (valor de retorno) del tope de la pila (la funci�n "GetRetValue" est� sobrecargada: dependiendo del 
		//  tipo del argumento, se llamar� autom�ticamente a la funci�n correspondiente).
		GetRetValue( lpRet );
		
		//4.Eliminar el valor de retorno de la pila.
		Pop( 1 );
	}
	//La funci�n no devuelve ning�n valor
	else
	{
		//Llamamos a la funci�n Lua que se insert� en el punto 1. para el contexto dado,
		//  indicando que tiene 3 par�metros y 0 valores de retorno.
		lua_call( mpLuaContext, 3, 0 );
	}
	return true;
}

#endif