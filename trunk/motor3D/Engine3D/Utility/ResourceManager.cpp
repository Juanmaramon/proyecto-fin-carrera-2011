#include "ResourceManager.h"

#include "ResourceHandle.h"
#include "Resource.h"

#include <assert.h>

//M�todo que inicializa el gestor de recursos.
void cResourceManager::Init( unsigned luiMaxSize )
{	
	muiMaxSize = luiMaxSize;
	// Initialize the structures.
	maResources.resize( muiMaxSize );
	mFreeResourceSlot.clear();
 
	for ( unsigned luiIndex = 0; luiIndex < muiMaxSize; ++luiIndex )
	{
	  // Initialize the resource slot.
      maResources[luiIndex].muiKey = kuiInvalidKey;
      maResources[luiIndex].mpResource = NULL;
 
      // Add the free index to the list.
      mFreeResourceSlot.push_back( luiIndex );
	}
 
	// Prepare the first key.
	muiNextKey = kuiInvalidKey + 1;
}

//M�todo que libera el gestor de recursos.
void cResourceManager::Deinit()
{
   for ( unsigned luiIndex = 0; luiIndex < muiMaxSize; ++luiIndex )
   {
      // Is a valid resource?
      if ( maResources[luiIndex].muiKey != kuiInvalidKey ) 
      {
         // Check that all is right
         assert( maResources[luiIndex].mpResource );
         assert( maResources[luiIndex].mpResource->IsLoaded() );
 
         // Deinit the resource
         maResources[luiIndex].mpResource->Deinit();
         delete maResources[luiIndex].mpResource;
 
         // Clear the resource slot
         maResources[luiIndex].muiKey = kuiInvalidKey;
         maResources[luiIndex].mpResource = NULL;
      }
   }
}

//M�todo que accede a un recurso a trav�s de un handle.
cResource * cResourceManager::GetResource( cResourceHandle *lpHandle )
{
   assert( lpHandle );
   assert( lpHandle->IsValidHandle() );
 
   unsigned luiIndex = lpHandle->GetID();
   assert( luiIndex < muiMaxSize );
   //Se comprueba si el �ndice del handle est� dentro del rango del array y si 
   // la casilla correspondiente tiene la misma clave. De ser as�, se comprueba si el recurso 
   // est� cargado. Si todo est� correcto se devuelve el recurso, de lo contrario se devuelve NULL.
   if (  maResources[luiIndex].muiKey == lpHandle->GetKey() 
      && maResources[luiIndex].mpResource->IsLoaded() )
   {
      return maResources[luiIndex].mpResource;
   }
   return NULL;
   //NOTA: Mientras un recurso se est� cargando, devolver� NULL hasta que est� preparado.
}
 
//M�todo, que a partir del nombre de un recurso, obtiene un handle.
cResourceHandle cResourceManager::FindResource( std::string lacNameID )
{   
   //Esta funci�n recorre el vector de recursos buscando un recurso que tenga el mismo 
   // nombre que el que se pas� como par�metro a la funci�n. Si lo encuentra, construye un 
   // handle y lo devuelve. De lo contrario devuelve un handle inv�lido.	
   cResourceHandle lHandle;
   for ( unsigned luiIndex = 0; luiIndex < muiMaxSize; ++luiIndex )
   {
	    // Is a valid resource?
      if ( maResources[luiIndex].muiKey != kuiInvalidKey ) 
      {
         // Check that all is right
         assert( maResources[luiIndex].mpResource );
         assert( maResources[luiIndex].mpResource->IsLoaded() );
 
         // Is the right resource?
         if ( maResources[luiIndex].mpResource->IsThisResource( lacNameID ) )
         {
            lHandle.Init(this, luiIndex, maResources[luiIndex].muiKey);
            break;
         }
      }
   }
   return lHandle;
}

//M�todo que libera el recurso apuntado por un handle.
void cResourceManager::UnloadResource( cResourceHandle * lpHandle )
{
   assert( lpHandle );
   assert( lpHandle->IsValidHandle() );
 
   unsigned luiIndex = lpHandle->GetID();
   assert( luiIndex < muiMaxSize );
   //Si se encuentra el recurso indicado por el handle, se libera, se limpia la casilla del 
   // vector y se a�ade a la lista de �ndices disponibles el �ndice de la casilla.  
   if (  maResources[luiIndex].muiKey == lpHandle->GetKey() 
      && maResources[luiIndex].mpResource->IsLoaded() )
   {
         // Deinit the resource
         maResources[luiIndex].mpResource->Deinit();
         delete maResources[luiIndex].mpResource;
 
         // Clear the resource slot
         maResources[luiIndex].muiKey = kuiInvalidKey;
         maResources[luiIndex].mpResource = NULL;
		 // Add the slot to the free list
         mFreeResourceSlot.push_front(luiIndex);
   }
}

//M�todo que a�ade un recurso desde un FICHERO haciendo uso de las funciones protegidas LoadResourceInternal y AddResourceToPool.
cResourceHandle cResourceManager::LoadResource( std::string lacNameID, const std::string &lacFile )
{
   //Se comprueba si el recurso ya se encuentra en el 
   // vector cargado (imaginemos que dos mallas comparten la misma textura) y de no ser 
   // as�, hacer las llamadas necesarias para cargarlo.
   cResourceHandle lHandle = FindResource( lacNameID );
   if ( !lHandle.IsValidHandle() )
   {
      // Load the Resource
      //Se carga el recurso desde un fichero. 
      cResource * lpResource = LoadResourceInternal( lacNameID, lacFile );
      if (lpResource)
	  {
         // Set the ID
         lpResource->SetNameID( lacNameID );
         // Save it into the pool
		 //Si el recurso se carga correctamente, se a�ade al vector y se devuelve un handle v�lido. 
         lHandle = AddResourceToPool( lpResource );
      }
      else
      {
         lHandle.Invalidate();
      }
   }
   return lHandle;
}

//M�todo que a�ade el recurso al vector de recursos.
cResourceHandle cResourceManager::AddResourceToPool( cResource * lpResource )
{
   //Se verifica que haya casillas libres.
   assert( mFreeResourceSlot.size() > 0 );
 
   //Se extrae el siguiente �ndice v�lido, se accede a la posici�n del array y se  
   // inicializan sus campos. Despu�s se crea un handle v�lido que apunte a ese recurso:

   unsigned luiNext = *mFreeResourceSlot.begin();
   mFreeResourceSlot.pop_front();
 
  
   assert( maResources[luiNext].muiKey == kuiInvalidKey );
   assert(muiNextKey != kuiInvalidKey );
 
   maResources[luiNext].muiKey = muiNextKey++;
   maResources[luiNext].mpResource = lpResource;
 
   cResourceHandle lHandle;
   lHandle.Init(this, luiNext, maResources[luiNext].muiKey);
 
   return lHandle;
}

//M�todo que a�ade un recurso desde MEMORIA haciendo uso de la funciones protegidas LoadResourceInternal y AddResourceToPool. 
cResourceHandle cResourceManager::LoadResource( std::string lacNameID, void * lpMemoryData, int luiTypeID )
{
   //Se comprueba si el recurso ya se encuentra en el 
   // vector cargado (imaginemos que dos mallas comparten la misma textura) y de no ser 
   // as�, hacer las llamadas necesarias para cargarlo.
   cResourceHandle lHandle = FindResource( lacNameID );
   if ( !lHandle.IsValidHandle() )
   {
      // Load the Resource
      //Se carga el recurso desde memoria.
      cResource * lpResource = LoadResourceInternal( lacNameID, lpMemoryData, luiTypeID );
      if (lpResource)
      {
         // Set the ID
         lpResource->SetNameID( lacNameID );
         // Save it into the pool
		 //Si el recurso se carga correctamente, se a�ade al vector y se devuelve un handle v�lido. 
         lHandle = AddResourceToPool( lpResource );
      }
      else
      {
         lHandle.Invalidate();
      }
   }
   return lHandle;
}