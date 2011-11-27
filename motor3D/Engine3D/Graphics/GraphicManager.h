//Clase que encapsular� en la medida de lo posible todas las operaciones 
// relacionadas con OpenGL.

#ifndef GraphicManager_H
#define GraphicManager_H

#include "..\Utility\Singleton.h"
#include "GLHeaders.h"
#include "Camera.h"
#include "..\Window\Window.h"
#include "..\MathLib\MathLib.h"

class cWindow;
class cGraphicManager : public cSingleton<cGraphicManager>
{
   public:
	  //Funci�n que se encarga de la inicializaci�n de OpenGL.
	  //Recibe la ventana sobre la que OpenGL har� el render.
      bool Init( cWindow * lpWindow );

	  //Funci�n que se encarga de intercambiar el backbuffer con el frontbuffer.
      void SwapBuffer();

	  //Funci�n para renderizar un punto en el espacio.
	  //El primer par�metro es la posici�n donde 
	  // se renderizar� el punto y el segundo par�metro ser� el color representado por RGB 
	  // entre 0.0 y 1.0.
	  void DrawPoint( const cVec3 &lvPosition, const cVec3 &lvColor );

	  //Funci�n para rederizar una l�nea.
	  void DrawLine( const cVec3 &lvPosition1, const cVec3 &lvPosition2, const cVec3 &lvColor );

	  //Funci�n para renderizar una malla o una rejilla.
	  void DrawGrid();

	  //Funci�n para renderizar unos ejes de coordenadas y as�
	  // saber en qu� direcci�n est� cada uno (X, Y, Z).
	  void DrawAxis();

	  //Funci�n que establece la matriz de mundo.
	  void SetWorldMatrix( const cMatrix &lMatrix );

	  //Funci�n que establece la c�mara actual.
	  void ActivateCamera( cCamera * lpCamera );
	  
	  //Funci�n que se encarga de la liberaci�n de OpenGL. 
      bool Deinit();

	  // Functio  to get active camera
	  cCamera * GetActiveCamera() { return mpActiveCamera; }
	
	  // Get world view proyection matrix
	  const cMatrix &GetWVPMatrix() { return mWVPMatrix; }

	  // Get world matrix
	  const cMatrix &GetWorldMatrix() { return mWorldMatrix; }

	  friend class cSingleton<cGraphicManager>;

   protected:
      cGraphicManager() { ; } // Protected constructor.

	  cWindow*    mpWindow; // Puntero a la ventana en la que se debe renderizar.
	  GLuint      mPixelFormat; // Formato de pixel.
      HGLRC       mHRC; // Handle  al contexto de renderizado de OpenGL.

	  cCamera *   mpActiveCamera; //Puntero a la c�mara actual. 
	  cMatrix     mWorldMatrix; //Matriz de mundo actual.
	  cMatrix	  mWVPMatrix; // World view proyection matrix.

   private:
	   //Funci�n que crea el contexto de renderizado.
	   bool CreateContext( cWindow * lpWindow );

	   //Funci�n que inicializa el estado de OpenGL.
	   void InitializeGLState();

	   //Funcion que actualiza la matriz ModelView en OpenGL.
	   void RefreshWorldView();

};

#endif