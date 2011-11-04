#include "TextureManager.h"
#include "Texture.h"

//M�todo que carga la textura espec�fica desde un fichero.
cResource * cTextureManager::LoadResourceInternal( std::string lacNameID, const std::string &lacFile )
{
   //Se crea una instancia de cTexture y llama a su funci�n Init con los par�metros asociados. 
   //Si la funci�n devuelve error, libera la memoria y devuelve NULL. De lo contrario, construye la 
   // textura y devuelve el recurso (textura en este caso).
   cTexture * lpTexture = new cTexture();
   if (!lpTexture->Init( lacNameID, lacFile ))
   {
      delete lpTexture;
      return NULL;
   }
 
   return lpTexture;
}