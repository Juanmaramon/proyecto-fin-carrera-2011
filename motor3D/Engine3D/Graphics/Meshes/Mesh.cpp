#include "Mesh.h"
#include "../GLHeaders.h"
#include <assimp.hpp>      // C++ importer interface
#include <aiMesh.h>        // Output data structure
#include <aiPostProcess.h> // Post processing flags
#include <cassert>

/*NOTA:
Las mallas que vamos a cargar se encuentran indexadas, es decir que tenemos un array con 
todos los posibles v�rtices que puede contener la malla pero sin seguir un orden concreto (es 
decir, los tres primeros v�rtices no definen un tri�ngulo y en principio no tienen relaci�n entre 
ellos). Por otro lado se tiene un array de �ndices que indica que v�rtices del array anterior 
forman tri�ngulos.
Esta representaci�n es muy com�n en videojuegos ya que suele requerir menos memoria, 
produce menos fallos de cach� y adem�s las tarjetas gr�ficas la soportan de forma nativa. 
Debido a esta representaci�n, hemos a�adido en la clase el atributo "muiIndexCount" para que almacene el 
n�mero de �ndices que contiene la malla y que necesitaremos para poder renderizar.
In this version multiple textures can be associated to a mesh, so shaders can used them and improve the model's quality
*/

bool cMesh::Init( const std::string &lacNameID, void * lpMemoryData, int liDataType )
{
   //macFile = "";
   aiMesh* lpAiMesh = (aiMesh*)lpMemoryData;

   // These lines have been added
   //-----------------------------------------------------------------
   // Get the number of texture coordinates
   unsigned luiTextureCoordinateCount = lpAiMesh->GetNumUVChannels();
   assert(luiTextureCoordinateCount <= 4);
   maVboTexture.resize(luiTextureCoordinateCount);
   //-----------------------------------------------------------------
 
   // En OpenGL los Vertex Buffer Objects se parecen mucho a los Vertex Arrays. En 
   // primer lugar debemos tener cada componente del v�rtice por separado, esto es, 
   // deberemos tener un buffer para las posiciones, otro para las coordenadas de textura, 
   // para las normales, �, y finalmente uno para los �ndices. 
   
   // En primer lugar crearemos el 
   // BUFFER DE V�RTICES (posiciones) (Vertex Buffer):
   glGenBuffers(1, &mVboVertices);
   assert(glGetError() == GL_NO_ERROR);

   // Una vez que hemos creado el buffer, 
   // debemos indicarle a OpenGL que las siguientes llamadas se refieren al buffer de 
   // v�rtices, por lo que ejecutamos:

   // These lines have been added
   //-----------------------------------------------------------------
   // Creating all the texture coordinate buffers
   for(unsigned luiIndex = 0; luiIndex < luiTextureCoordinateCount; ++luiIndex) {
		glGenBuffers(1, &maVboTexture[luiIndex]);
		assert(glGetError() == GL_NO_ERROR);
   }
   //-----------------------------------------------------------------

   //El BUFFER DE NORMALES es bastante similar:
   //Al igual que en el caso de las posiciones, 
   // las normales est�n convenientemente almacenadas en la escena, por lo que la 
   // inicializaci�n del buffer es bastante simple.
   glGenBuffers(1, &mVboNormals);
   assert(glGetError() == GL_NO_ERROR);

   //...y se crea el buffer de �ndices:
   glGenBuffers(1, &mVboIndex);
   assert(glGetError() == GL_NO_ERROR);

   // Binds position buffer
   glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
   assert(glGetError() == GL_NO_ERROR);


   //Ahor inicializamos el buffer con la siguiente llamada:
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 *lpAiMesh->mNumVertices, lpAiMesh->mVertices, GL_STATIC_DRAW);
   //El primer par�metro de la llamada le indica a OpenGL que es un array buffer normal y 
   // no uno de �ndices. A continuaci�n se le indica cuanta memoria tiene que copiar en el 
   // buffer, por lo que se multiplica el n�mero de v�rtices por el n�mero de flotantes que 
   // tiene cada v�rtice (X, Y, Z) por el tama�o que tiene un flotante, todo esto junto da la 
   // cantidad de bytes que se deben copiar. El tercer par�metro es el comienzo de la 
   // memoria donde se encuentran los v�rtices dentro de la estructura de la escena (en este 
   // caso la estructura encaja con lo que necesitamos, ya que es una array de posiciones X, 
   // Y, Z). El �ltimo par�metro le indica a la tarjeta gr�fica que el buffer se debe almacenar 
   // en memoria de video y que la aplicaci�n no acceder� directamente al buffer para 
   // modificar los datos que hay en �l contenidos. Con esto ya tenemos listo el buffer de 
   // posiciones.
   assert(glGetError() == GL_NO_ERROR);

   // Check that the mesh has normals
   if ( lpAiMesh->HasNormals() ) 
   {	    
		glBindBuffer(GL_ARRAY_BUFFER, mVboNormals);
		assert(glGetError() == GL_NO_ERROR);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * lpAiMesh->mNumVertices, lpAiMesh->mNormals, GL_STATIC_DRAW);
		assert(glGetError() == GL_NO_ERROR);
		mbHasNormals = true;
   }
   else
   {	   
	   mbHasNormals = false;
   }

   // Tex Coords
   // These lines have been added
   //-----------------------------------------------------------------
   // Reading all the texture coordinate

   //Al contrario que las posiciones y las normales, las COORDENADAS DE TEXTURA no est�n convenientemente
   // almacenadas en la escena, por lo que tenemos que componer primero el buffer en memoria:
   //Se crea el buffer en memoria con los datos en la distribuci�n que 
   // necesitamos. Se aplica una conversi�n a las coordenadas Y de 
   // la textura. Esto es porque OpenGL y DirectX tienen invertidos entre s� este par�metro. 
   //Por lo que es necesario invertirlo para que se muestre correctamente.
   assert(lpAiMesh->HasTextureCoords(0));
   unsigned luiTexCoordNum = lpAiMesh->mNumVertices;
   unsigned luiTexCoordFloats = 2 * luiTexCoordNum;
   float* lpTexCoordinates = new float[ luiTexCoordFloats ];

   for(unsigned luiTexCoordChannel = 0; luiTexCoordChannel < luiTextureCoordinateCount; ++luiTexCoordChannel) {
		unsigned luiInc = 0;
		for ( unsigned luiTexIndex = 0; luiTexIndex < luiTexCoordNum; ++luiTexIndex ) {
			lpTexCoordinates[luiInc++] = lpAiMesh->mTextureCoords[luiTexCoordChannel][luiTexIndex].x;
			lpTexCoordinates[luiInc++] = (1.0f - lpAiMesh->mTextureCoords[luiTexCoordChannel][luiTexIndex].y);
			// OpenGL Correction
		}
		//Una vez que tenemos el buffer, se aplica un c�digo similar al ya 
        // visto con las posiciones y las normales:
		glBindBuffer(GL_ARRAY_BUFFER, maVboTexture[luiTexCoordChannel]);
		assert(glGetError() == GL_NO_ERROR);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * luiTexCoordFloats, lpTexCoordinates, GL_STATIC_DRAW);
		assert(glGetError() == GL_NO_ERROR);
	}
	delete [] lpTexCoordinates;
	//----------------------------------------------------------------

   //A continuaci�n se realiza la carga del BUFFER DE �NDICES, que tambi�n nos fuerza a componer primero el 
   //buffer en memoria:
   // Index
	muiIndexCount = lpAiMesh->mNumFaces * 3;
	unsigned* lpIndex = new unsigned[ muiIndexCount ];
	unsigned luiIndex = 0;
	for ( unsigned luiFaceIndex = 0; luiFaceIndex < lpAiMesh->mNumFaces; ++luiFaceIndex ) {
		assert(lpAiMesh->mFaces[luiFaceIndex].mNumIndices == 3 );
		lpIndex[luiIndex++]= lpAiMesh->mFaces[luiFaceIndex].mIndices[0];
		lpIndex[luiIndex++]= lpAiMesh->mFaces[luiFaceIndex].mIndices[1];
		lpIndex[luiIndex++]= lpAiMesh->mFaces[luiFaceIndex].mIndices[2];
	}
   //El primer par�metro de las llamadas a "glBindBuffer" y a "glBufferData" es 
   // distinto al de los casos anteriores, esto es porque OpenGL necesita que se le
   // indique que el buffer es un buffer de �ndices.
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndex);
   assert(glGetError() == GL_NO_ERROR);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, muiIndexCount* sizeof(unsigned), lpIndex, GL_STATIC_DRAW);
   assert(glGetError() == GL_NO_ERROR);
   delete [] lpIndex;

   //En este punto se acaba la CARGA DE LA MALLA EN MEMORIA.
	
   // End of the function
   mbLoaded = true;
   return true;
}

//M�todo que libera los bufferes que se crean en el m�todo Init.
void cMesh::Deinit()
{
   glDeleteBuffers(1, &mVboVertices);

   // These lines have been added
   //-----------------------------------------------------------------
   for(unsigned luiTexCoordChannel = 0; luiTexCoordChannel < maVboTexture.size(); ++luiTexCoordChannel) {
		glDeleteBuffers(1, &maVboTexture[luiTexCoordChannel]);
   }
   //-----------------------------------------------------------------

   if (mbHasNormals) //este atributo se inicializa en el Init.
      glDeleteBuffers(1, &mVboNormals);
   glDeleteBuffers(1, &mVboIndex);
}

//M�todo que renderiza la malla.
void cMesh::RenderMesh()
{
   //Para renderizar se usar� un c�digo muy similar al que se utilizar�a con Array Buffers.
   glColor3f (1.0f, 1.0f, 1.0f); //Color blanco.
 
   //Antes de renderizar la malla le indicamos a OpenGL cuales 
   // con los b�feres que debe utilizar:
   
   // Position
   glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
   assert(glGetError() == GL_NO_ERROR);
   glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, 0);
   assert(glGetError() == GL_NO_ERROR);
 
   // Normals
   if (mbHasNormals) //este atributo se inicializa en el Init.  
   {
		glBindBuffer(GL_ARRAY_BUFFER, mVboNormals);
		assert(glGetError() == GL_NO_ERROR);
		glNormalPointer(GL_FLOAT, sizeof(float) * 3, 0);
		assert(glGetError() == GL_NO_ERROR);
   }

   // This line has been added
   //-----------------------------------------------------------------
   // Set all the UV channels to the render
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   static GLenum meTextureChannelEnum[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };
   
   for(unsigned luiTexCoordChannel = 0; luiTexCoordChannel < maVboTexture.size(); ++luiTexCoordChannel) {
	   // Texture coordinates
	   glClientActiveTexture(meTextureChannelEnum[luiTexCoordChannel]);
       glBindBuffer(GL_ARRAY_BUFFER, maVboTexture[luiTexCoordChannel]);
       assert(glGetError() == GL_NO_ERROR);
       glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, 0);
       assert(glGetError() == GL_NO_ERROR);
   }
   
   // Index
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIndex);
   assert(glGetError() == GL_NO_ERROR);
 
   //Las llamadas a glEnableClientState sirven para indicarle a OpenGL que la memoria de los b�feres se 
   // encuentra en la tarjeta gr�fica y no en la memoria del programa
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glDrawRangeElements(GL_TRIANGLES, 0, muiIndexCount, muiIndexCount, GL_UNSIGNED_INT, NULL);
   assert(glGetError() == GL_NO_ERROR);
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
} 