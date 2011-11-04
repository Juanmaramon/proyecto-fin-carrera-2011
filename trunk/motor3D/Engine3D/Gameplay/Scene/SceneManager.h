/*Esta Clase implementa la interfaz cResourceManager. Es un gestor de escenas.
Esta clase es HIJA de cResourceManager (PADRE) y por tanto heredar� de ella.
De los 2 m�todos virtuales, implementa el "LoadResourceInternal" que carga el
 recurso desde un fichero.
*/


#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
 
#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"
 

//Esta clase es un Singleton. 
class cSceneManager : public cResourceManager, public cSingleton<cSceneManager>
{
   public:
      friend class cSingleton<cSceneManager>;
   protected:
      cSceneManager() { ; } // Protected constructor
 
   private:
	   //Carga la escena espec�fica desde un fichero.
       virtual  cResource * LoadResourceInternal( std::string lacNameID, const std::string &lacFile );
};
 
#endif