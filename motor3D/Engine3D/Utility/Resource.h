/*
La interfaz Resource representa un recurso que cargaremos en nuestro 
juego, y de la cual todos los recursos heredar�n.
En general un recurso se podr� inicializar, deinicializar y recargar. Adem�s 
vendr� identificado por un nombre que deber�a ser �nico con respecto a los recursos 
del mismo tipo.
*/

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class cResource
{
   public:  
  
	  //Inicializa un recurso desde un fichero indicando su ruta.  
      virtual bool Init( const std::string &lacNameID, const std::string &lacFile ) { return false; }
      
	  //Inicializa un recurso desde una zona de memoria (habr� recursos que estar�n empaquetados en un �nico fichero).
	  virtual bool Init( const std::string &lacNameID, void * lacMemoryData ) { return false; }

	  //Libera el recurso.
      virtual void Deinit()                { ; }
      
	  //Comprueba si el recurso est� cargado.
	  virtual bool IsLoaded()              { return false; }

	  //Recarga el recurso.
      virtual void Reload()                { ; }
 
      //Comprueba si el identificador del recurso es el que se indica por par�metro.
	  bool IsThisResource( const std::string &lacNameID ) { return macNameID == lacNameID; }

	  //Establece el nombre del recurso.
      inline void SetNameID( const std::string &lacNameID ) { macNameID = lacNameID; }
	  
	  //Obtiene el nombre del recurso.
	  inline std::string GetNameID( ) { return macNameID; }

   private:
      //Nombre para identificar el recurso. Es �nico con respecto a los recursos del mismo tipo.
      std::string macNameID;
};


#endif