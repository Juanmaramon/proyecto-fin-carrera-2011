//Clase que representa la ventana del juego.

#ifndef Window_H
#define Window_H

#include <Windows.h>

#include "..\Utility\Singleton.h" 
#include "ApplicationProperties.h"

//Por sencillez, asumiremos que nuestro juego tiene una s�la ventana,
// por lo que la clase cWindow ser� un singleton.
class cWindow : public cSingleton<cWindow>
{
	friend class cSingleton<cWindow>;
	protected:
		cWindow() { ; } // Protected constructor

		HINSTANCE   mInstance; // Holds The Instance Of The Application; Manejador para acceder al objeto que 
							   //	contiene la informaci�n asociada a la ejecuci�n de nuestra aplicaci�n. 
		HWND        mWnd;      // Holds Our Window Handle; Manejador para acceder al objeto que representa a la ventana.
		HDC         mDC;       // Private GDI Device Context; Manejador para acceder al Contexto de Dispositivo o HDC de la ventana
							   //   y poder pintar sobre ella.	
		
		cApplicationProperties mProperties; //Este atributo ser� una copia de los par�metros de entrada
											// del m�todo Init.
		bool mbCloseApplication; //Este atributo  ser� consultado por la clase cGame para saber si 
								 // debe cerrar el juego porque el usuario ha cerrado la ventana.

	public:
		//Esta funci�n se encargar� de crear la ventana con los par�metros que recibe.
		bool Init( cApplicationProperties &lProperties );

		//Esta funci�n gestiona los mensajes provenientes de la ventana.
		static LRESULT CALLBACK WndProc( HWND lWnd, UINT lMsg, WPARAM lWParam, LPARAM lLParam);

		//Esta funci�n se encargar� de actualizar la ventana y atender a posibles eventos que 
		// provengan de la misma.
		void Update();	

		//Esta funci�n sirve para que la clase cGame pueda acceder al booleano mbCloseApplication.
		inline bool GetCloseApplication() { return mbCloseApplication; }

		//Esta funci�n accede al handle de la ventana. 
		inline HWND GetHWND() { return mWnd; }

		//OpenGL (represantado en la clase cGraphicManager) necesita acceder al contexto del dispositivo de la ventana, 
		// al n�mero de bits por p�xel y a las dimensiones de la ventana,
		// por lo que son necesarias las siguientes funciones.
		inline unsigned GetWidth()      { return mProperties.muiWidth; }
		inline unsigned GetHeight()     { return mProperties.muiHeight; }
		inline unsigned GetBpp()        { return mProperties.muiBits; }
		inline HDC &GetHDC()            { return mDC; }

		//Esta funci�n se encargar� de eliminar la ventana.
		bool Deinit();

};

#endif