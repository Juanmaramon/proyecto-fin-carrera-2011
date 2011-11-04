/*Esta Clase (malla) implementa la interfaz cResource.
Esta clase es HIJA de cResource (PADRE) y por tanto heredar� de ella e 
implementa las funciones virtuales correspondientes. 

Se encarga de cargar la malla desde la estructura (aiMesh) que nos proporciona 
la librer�a Assimp para posteriormente almacenar la informaci�n de mallas en la 
tarjeta gr�fica (mirar m�todo cMesh::Init).
*/


#ifndef MESH_H
#define MESH_H
 
#include <string>
#include <vector>
#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"

// This constants allow us to differents  between an static mash and a skeletal one
static int kuiStaticMesh = 0;
static int kuiSkeletalMesh = 1;

//En la funci�n "cMesh::Init" se hace referencia a la estructura aiMesh, por eso es necesario
// a�adir una declaraci�n forward de dicha clase:
struct aiMesh;
 
class cMesh : public cResource
{
   public:
      cMesh() { mbLoaded = false; }
 
      //Inicializa una malla desde Memoria.
	  virtual bool Init( const std::string &lacNameID, void * lpMemoryData, int luiTypeID );

	  //Reloads the mesh
	  void Reload( void * lpMemoryData );
      
	  //Libera los bufferes que se crean en el m�todo Init.
	  virtual void Deinit();

      //Comprueba si la malla est� cargada.
	  virtual bool IsLoaded()  { return mbLoaded; }
      
      //Renderiza la malla.	  
	  virtual void RenderMesh();

	  //This function will be update the mesh data
	  virtual void Update(float lfTimestep) {}
   
	  // Virtual function, in cSkeletalMesh will set shader animation matrix
	  virtual void PrepareRender(cResourceHandle lMaterial) { ; }

	protected:
	
	  // Mesh name
	  std::string macFile;

      //Almacena el n�mero de �ndices que contiene la malla y que se necesita
	  // para poder renderizar.
	  unsigned muiIndexCount;

	  //Array with multiple texture coordinates 
	  std::vector<unsigned> maVboTexture;

	  //Booleano que indica si la malla est� cargada o no.
      bool mbLoaded;

	  //Buffer de v�rtices (posiciones).
	  unsigned int mVboVertices;

	  //Buffer de normales.
	  unsigned int mVboNormals;

	  //Buffer de �ndices.
	  unsigned int mVboIndex;

	  //Booleano que indique si la malla cuenta o no con normales.
	  bool mbHasNormals;

};
#endif