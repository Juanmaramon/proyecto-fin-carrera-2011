#include "ResourceHandle.h"
#include "ResourceManager.h"
 
//M�todo que inicializa el handle.
void cResourceHandle::Init( cResourceManager *  lpManager, unsigned luiID, unsigned luiKey )
{
   mpManager = lpManager;
   muiID = luiID;
   muiKey = luiKey;
}
 
//M�todo que accede a un recurso. 
//Devolver� NULL si el recurso no est� disponible o un puntero al recurso asociado al 
// handle. Este puntero debe usarse en el fragmento de c�digo en el que estemos y nunca almacenarlo para m�s tarde.
cResource * cResourceHandle::GetResource() 
{
   //Si la clave no es v�lida se devuelve directamente NULL. Si la clave por el 
   // contrario es v�lida, llamamos a la funci�n GetResource del manager correspondiente 
   // pasando el handle en s� mismo. Esto se hace s�, ya que el handle contiene toda la 
   // informaci�n necesaria para acceder al recurso. Si las claves no coinciden, ser� el propio 
   // manager el que devuelva NULL.
   if (muiKey != kuiInvalidKey)
   {
      return mpManager->GetResource(this); 
   }
   else
   {
      return NULL;
   }
}