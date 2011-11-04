/*
El manejador o handle de recursos (ResourceHandle) es una clase que se encarga de permitirnos el 
acceso al recurso en s�, porque durante el juego se recargar�n los recursos en medio de la 
ejecuci�n sin necesidad de tener que interrumpir el juego.
Para implementar esta propiedad es muy importante que nuestro juego 
almacene �nicamente handles a recursos y no los recursos en s�. Cuando se 
accede a un recurso a trav�s de un handle, debemos hacer uso de �l y NUNCA 
guardarnos el puntero al recurso para m�s tarde.

NOTA: 
El manager de recursos (ResourceManager) tendr� todos los recursos en un array y el handle (ResourceHandle)
contendr� el �ndice del recurso en el array.
Pero el manager no contendr� el recurso en si, sino una estructura
que llamaremos cInternalResource que contendr� el recurso 
que est� almacenado en esa posici�n y una clave (key). Cada vez que el manager 
almacene un nuevo recurso, le asignar� un nuevo valor a la clave, por lo que el handle 
podr� comprobar si el recurso al que accede es el correcto o no.
*/


#ifndef RESOURCE_HANDLE_H
#define RESOURCE_HANDLE_H
 
#include <stdlib.h>
 
class cResourceManager;
class cResource;


static const unsigned kuiInvalidKey = 0;
 
class cResourceHandle
{
   public:
      cResourceHandle() { mpManager = NULL; muiKey = kuiInvalidKey; }

	  //Accede a un recurso. 
	  //Devolver� NULL si el recurso no est� disponible o un puntero al recurso asociado al 
	  // handle. Este puntero debe usarse en el fragmento de c�digo en el que estemos y nunca almacenarlo para m�s tarde.
      cResource * GetResource();
 
	  //Comprueba si un handle es v�lido (esta funci�n devuelve si el handle es v�lido, no si el 
	  // recurso est� disponible)
      inline bool IsValidHandle() { return muiKey != kuiInvalidKey; }
      
	  //Libera el handle.
	  inline void Invalidate()    { muiKey = kuiInvalidKey; }
	  
      inline unsigned GetID() { return muiID; }
      inline unsigned GetKey() { return muiKey; }

   protected:
 
      friend class cResourceManager;

	  //Inicializa el handle.
	  //La funci�n Init es protegida, pero la clase cResourceManager es una clase amiga. Esto se traduce
	  // en que s�lo los gestores de recursos pueden inicializar un handle.
      void Init( cResourceManager * lpManager, unsigned luiID, unsigned luiKey );
 
	  //Puntero al gestor de recurso para poder acceder al recurso.
      cResourceManager * mpManager;
      
	  unsigned int muiID;
      unsigned int muiKey;
};
 
#endif


