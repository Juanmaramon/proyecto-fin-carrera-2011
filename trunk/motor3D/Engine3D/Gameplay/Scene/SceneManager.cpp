#include "SceneManager.h"
#include "Scene.h"
 
//M�todo que carga la escena espec�fica desde un fichero.
cResource * cSceneManager::LoadResourceInternal(  std::string lacNameID, const std::string &lacFile )
{
   //La escena vendr� definida por un fichero, crearemos la 
   // escena con ese fichero y si todo ha ido bien, devolveremos la escena reci�n cargada. De 
   // lo contrario, liberamos la memoria y devolvemos NULL.
   cScene * lpScene = new cScene();
   if (!lpScene->Init( lacNameID, lacFile ))
   {
      delete lpScene;
      return NULL;
   }
   return lpScene;
}