/*
El manager de recursos (ResourceManager) tendr� todos los recursos en un array y el handle (ResourceHandle)
contendr� el �ndice del recurso en el array.
Pero el manager no contendr� el recurso en si, sino una estructura que llamaremos
cInternalResource que contendr� el recurso que est� almacenado en esa posici�n y una clave (key).
El manager o gestor de recursos es una interfaz.
*/


#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include <list>

//#include "ResourceHandle.h"
//#include "Resource.h"


class cResource;
class cResourceHandle;

//Esta estructura es lo que se guardar� en el vector que contendr� los recursos. Est� 
// compuesta por una clave y el puntero al recurso.
struct cInternalResource
{
   unsigned int muiKey;
   cResource * mpResource;
};


class cResourceManager
{
   public:

      //Inicializa el gestor de recursos.
      virtual void Init( unsigned luiMaxSize );
      
	  //Libera el gestor de recursos.
	  void Deinit();

	  //Obtiene un handle a partir del nombre de un recurso.
	  cResourceHandle FindResource( std::string lacNameID );

	  //Libera el recurso apuntado por un handle.
	  void UnloadResource( cResourceHandle * lpHandle );

	  //A�ade un recurso desde un FICHERO haciendo uso de las funciones protegidas LoadResourceInternal y AddResourceToPool.
	  cResourceHandle LoadResource( std::string lacNameID,  const std::string &lacFile );

	  //A�ade un recurso desde MEMORIA haciendo uso de la funciones protegidas LoadResourceInternal y AddResourceToPool.
	  cResourceHandle LoadResource( std::string lacNameID, void * lpMemoryData, int luiTypeID );
 
   protected:

	  //A�ade el recurso al vector de recursos.
	  cResourceHandle AddResourceToPool( cResource * lpResource );
      
	  //Esta funci�n es virtual y cada manager (clases derivadas) la implementar� para cargar el recurso espec�fico desde un FICHERO.
	  virtual cResource * LoadResourceInternal( std::string lacNameID, const std::string &lacFile ) { return NULL; };

	  //Esta funci�n es virtual y cada manager (clases derivadas) la implementar� para cargar el recurso espec�fico desde MEMORIA.
	  virtual cResource * LoadResourceInternal( std::string lacNameID, void * lpMemoryData, int luiTypeID ) {return NULL;}
	   
      //Accede a un recurso a trav�s de un handle.
	  cResource * GetResource( cResourceHandle *lpHandle );

	  //La funci�n anterior es protegida. En esta clase se establece como clase amiga al 
	  // handle. Esto quiere decir que s�lo un handle puede llamar a esta funci�n para acceder al 
      // recurso. Esto se hace en cResourceHandle::GetResource().
	  friend class cResourceHandle;

	  //Vector de recursos.
      std::vector<cInternalResource> maResources;
      
	  //Lista que guarda todos los �ndices de casillas que est�n libres en el vector.
	  //Es necesaria para poder almacenar nuevos recursos r�pidamente.
	  std::list<unsigned> mFreeResourceSlot;

	  //NOTA:
		/*Como estamos guardando los recursos en un vector, necesitamos una forma 	 
		r�pida de saber que casillas del vector est�n libres para poder almacenar nuevos 
		recursos. Es por ello que tenemos una lista adicional que guarda todos los �ndices de 
		casillas que est�n libres. Por lo tanto, cada vez que a�adamos un nuevo elemento, 
		sacaremos un �ndice de la lista de �ndices libre y lo almacenaremos en esa nueva 
		posici�n con el valor de Key correspondiente a muiNextKey. Justo despu�s de la 
		inserci�n incrementaremos en 1 el valor de muiNextKey. 
		Si la lista de �ndices libres est� vac�a significar� que no quedan casillas libres y 
		deberemos mostrar un mensaje de error. 
		Cuando liberemos un recurso, deberemos a�adir el �ndice en el que estaba el recurso 
		en la lista de �ndices libres. De esta forma, la lista siempre nos permitir� acceder a 
		casillas libres de forma muy r�pida.
        */  
 
	  //Clave que se asignar� al siguiente recurso que se almacene.
      int muiNextKey;
	  //N�mero m�ximo de recursos que podemos almacenar en la estructura.
      unsigned muiMaxSize;
};


#endif 

