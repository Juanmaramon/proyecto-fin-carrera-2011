/*Esta Clase implementa la interfaz cResource.
Esta clase es HIJA de cResource (PADRE) y por tanto heredar� de ella e 
implementa las funciones virtuales correspondientes. 

Para la librer�a SOIL (que permite cargar texturas en formato DDS, entre otros) y para OpenGL
una textura se identifica por un �ndice. Por lo tanto, los atributos de esta 
clase ser�n el �ndice de la textura y la ruta del fichero.
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "../../Utility/Resource.h"

class cTexture : public cResource
{
   public:
      cTexture()                   { muiTextureHandle = 0; }
	  
	  //Inicializa una textura desde un fichero indicando su ruta.
      virtual bool Init( const std::string &lacNameID, const std::string &lacFile );

	  //Libera la textura.
      virtual void Deinit();
      
	  //Comprueba si la textura est� cargada.
	  virtual bool IsLoaded()      { return (muiTextureHandle != 0); }
      
	  //Obtiene el �ndice que identifica la textura.
      inline unsigned int GetTextureHandle(){return muiTextureHandle;}
   private:
      //Ruta del fichero que contiene la textura.
      std::string macFile;

	  //�ndice que identifica la textura.
	  //El valor 0 representa un �ndice inv�lido para el handle de la textura.
      unsigned int muiTextureHandle;
};

#endif