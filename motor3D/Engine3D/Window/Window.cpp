#include "Window.h"

//Esta funci�n se encargar� de crear la ventana con los par�metros que recibe.
bool cWindow :: Init( cApplicationProperties &lProperties )
{
	//Esta funci�n se divide en varios bloques: 1) Inicializaci�n de los atributos,
	// 2) Registro de la clase ventana, 3) Preparaci�n de la pantalla para el modo Fullscreen,
	// 4) Creaci�n de la ventana y 5) Acceso al contexto de dispositivo.

	//1) 
	mDC = NULL;          
	mWnd = NULL;            
	mInstance = GetModuleHandle(NULL); //La funci�n GetModuleHandle devuelve el handle (manejador) de tipo HINSTANCE de la aplicaci�n.
	mProperties = lProperties;
	mbCloseApplication = false;

	//2)
	//Antes de poder crear la ventana en Windows, es necesario indicarle al sistema 
    // operativo una serie de par�metros (registro de la ventana) que necesitar� m�s tarde para la creaci�n
	// de la ventana: 

	// Windows Class Structure
	WNDCLASS lWndClass;

	//Redraw On Size, And Own DC For Window.
	// En el atributo �style� indicamos a windows que la ventana se tiene que refrescar (y 
	//  repintar) cuando se mueva o re-escale en horizontal o vertical. Adem�s se le indica que
	//  la ventana tendr� un contexto de dispositivo propio:
	lWndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	//WndProc Handles Messages.
	// El atributo �lpfnWndProc� establece la funci�n que se encargar� de procesar (manejar) los 
	//  mensajes provenientes de la ventana (por ejemplo, recibir� un mensaje cuando el 
    //  usuario cierre la ventana):
	lWndClass.lpfnWndProc = (WNDPROC) WndProc;

	//No Extra Window Data:
	lWndClass.cbClsExtra = 0;
	//No Extra Window Data:
	lWndClass.cbWndExtra = 0; 
	
	//Set The Instance.
	// El atributo �hInstance� guardar� el contexto de la aplicaci�n:
	lWndClass.hInstance = mInstance;
	
	//Load The Default Icon.
	// El atributo �hIcon� establece el icono de la ventana. 
    //  Se configura el valor por defecto:
	lWndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);

	//Load The Arrow Pointer.
	// El atributo �hCursor� establece el icono que usara el cursor.
    //  Se configura el valor por defecto:
	lWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	//No Background Required For GL.
	// El atributo �hbrBackground� sirve para configurar el fondo de la ventana (color, �). 
    //  Ser� OpenGL quien controle el fondo. Por ello lo dejamos vac�o:
	lWndClass.hbrBackground = NULL;

	//We Don't Want A Menu.
	// El atributo �lpszMenuName� sirve para establecer el men� de la aplicaci�n.
	//  No se usar� men� de Windows en la aplicaci�n:
	lWndClass.lpszMenuName = NULL; 

	//Set The Class Name.
	// El atributo �lpszClassName� sirve para indicar el nombre con el que se 
    //  registrar� la ventana. Se registrar� con el mismo nombre que se usar� para el t�tulo:
	lWndClass.lpszClassName = lProperties.macApplicationName.c_str();
	
    //Se llama a la  funci�n  RegisterClass para registrar la ventana. 
	// Se toma su valor de retorno y se comprueba que no se haya 
    //  producido ning�n error. Si se produjo, se sale indicando que la inicializaci�n no se 
	//  puedo realizar con �xito:
	int liRet = RegisterClass(&lWndClass);
	if (!liRet) // Attempt To Register The Window Class
	{
	   DWORD lError = GetLastError();
	   MessageBox(NULL,"��Fallo al registrar la ventana!!",
				   "ERROR",MB_OK|MB_ICONEXCLAMATION);
	  return false;
	}


	//3)
	//Antes de crear la ventana es necesario comprobar que la resoluci�n est� permitida por 
	//la pantalla y cambiarla en el caso de que la pantalla la acepte:
	if (lProperties.mbFullscreen)
	{
	   // Device Mode
	   DEVMODE lScreenSettings;
	   // Clear the memory     
	   memset(&lScreenSettings,0,sizeof(lScreenSettings));
	   lScreenSettings.dmSize = sizeof(lScreenSettings);
	   lScreenSettings.dmPelsWidth = lProperties.muiWidth;
	   lScreenSettings.dmPelsHeight = lProperties.muiHeight;
	   lScreenSettings.dmBitsPerPel = lProperties.muiBits;
	   lScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	   //Se intenta establecer el modo de Pantalla Completa y se obtiene el resultado.
	   LONG lResult = ChangeDisplaySettings(&lScreenSettings,CDS_FULLSCREEN);
	   //Si no se pudo establecer pantalla completa se pregunta si se quiere pasar a modo ventana.
	   if (lResult != DISP_CHANGE_SUCCESSFUL)
	   {
		   if (MessageBox(NULL,"El modo de Pantalla Completa solicitado no es soportado por\n su tarjeta de v�deo. �Desea usar el modo Ventana?",
						  "ERROR", MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
		   {
		       //Se pasa a modo Ventana.
			   mProperties.mbFullscreen = false;
		   }
		   else
		   {
			   MessageBox(NULL,"El programa se cerrar� ahora.","ERROR",
							 MB_OK|MB_ICONSTOP);
			   return false;
		   }
	   } 
	}
	/*El c�digo anterior simplemente crea una estructura en la que establece la 
	resoluci�n de la pantalla y los bits por pixel, y llama a la funci�n "ChangeDisplaySettings".
	Si la funci�n puede cambiar la resoluci�n, la cambia y si no, 
	devuelve un error.  
	Si se produce un error, se muestra un mensaje preguntando al jugador se desea 
	ejecutar en una ventana en lugar de en pantalla completa. Si el jugador desea cambiar 
	el modo  de  ejecuci�n, se cambia el booleano correspondiente y se contin�a con la 
	inicializaci�n. Si no, se sale de la aplicaci�n.*/


	//4)
	DWORD lExStyle;
	DWORD lStyle;

	//Se establece el Estilo de la Ventana dependiendo de si se va a ejecutar
	// en  pantalla completa o si se va a ejecutar en ventana.
	//Modo Pantalla Completa:
	if (mProperties.mbFullscreen) 
	{
	   // Window Extended Style
	   lExStyle=WS_EX_APPWINDOW;
	   // Windows Style
	   lStyle=WS_POPUP;
	   // Hide Mouse Pointer
	   ShowCursor(FALSE); 
	}
	//Modo Ventana:
	else
	{
	   // Window Extended Style
	   lExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	   // Windows Style
	   lStyle=WS_OVERLAPPEDWINDOW;
	}
	RECT lWindowRect;
	lWindowRect.left = (long)0;
	lWindowRect.right = (long)lProperties.muiWidth;
	lWindowRect.top = (long)0;
	lWindowRect.bottom = (long)lProperties.muiHeight;
	 
	/*Es necesario Calcular el Tama�o Real de la Ventana.
	Si se quiere ejecutar el juego en una resoluci�n de 800x600, est� es el 
	espacio que usaremos en la ventana. Pero la ventana puede tener adem�s una barra de 
	t�tulo, unos bordes, etc, por lo que el tama�o real de la ventana ser� el tama�o que 
	usaremos nosotros m�s lo que usar� el sistema operativo con el estilo que le hemos 
	indicado. El c�lculo de este tama�o lo har� la funci�n AdjustWindowRectEx:  */
	AdjustWindowRectEx(&lWindowRect, lStyle, FALSE, lExStyle);
	 
	/*Una vez que se ha calculado el tama�o real de la ventana, se pasa a crearla.
	La llamada CreateWindowEx  se encarga de Crear la Ventana. El primer par�metro es 
	parte del estilo de la ventana (que se estableci� anteriormente).
	Los siguientes par�metros son el nombre de la clase que con el que se registr� la ventana 
	en las secciones previas y el t�tulo a mostrar en la ventana. A continuaci�n se establece 
	el resto del estilo de la ventana, seguido de sus dimensiones y posici�n en la pantalla. 
	Los dos siguientes par�metros son NULL, porque esta ventana no depende de otra 
	ventana y no tenemos men�s. Para terminar, se le indica el handle de la aplicaci�n y los 
	par�metros de usuario que son NULL: */
	// Create The Window
	mWnd = CreateWindowEx( lExStyle,
	   lProperties.macApplicationName.c_str(),  // Class Name             
	   lProperties.macApplicationName.c_str(),  // Window Title
	   lStyle |                              // Defined Window Style
	   WS_CLIPSIBLINGS |                     // Required Window Style
	   WS_CLIPCHILDREN,                      // Required Window Style
	   0, 0,                                 // Window Position
	   lWindowRect.right-lWindowRect.left,   // Calculate Window Width
	   lWindowRect.bottom-lWindowRect.top,   // Calculate Window Height
	   NULL,                                 // No Parent Window
	   NULL,                                 // No Menu
	   mInstance,                            // Instance
	   NULL);
	
	//Si se produce error al crear la ventana, se muestra un mensaje y se finaliza:
	if (!mWnd)
	{
		 Deinit();      // Reset The Display
		 MessageBox(NULL,"Error al crear la Ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		 return false;                                  
	}
	//Si no se produce error al crear la ventana, se visualiza la ventana y se establece el foco en ella.
	ShowWindow(mWnd,SW_SHOW);    
	SetForegroundWindow(mWnd);
	SetFocus(mWnd);

	
	//5)
	//Se accede al contexto del dispositivo de la ventana que se acaba de crear:
	mDC = GetDC(mWnd);//Funci�n para obtener el contexto de dispositivo.
	//Si se produce un error se muestra un mensaje y se finaliza:
	if (!mDC)
	{
		  Deinit();
		  MessageBox(NULL,"No se puede acceder al Contexto de Dispositivo (HDC).","ERROR",MB_OK|MB_ICONEXCLAMATION);
		  return false;
	}
	//Si toda ha ido bien, se ha terminado la inicializaci�n de la ventana y se devuelve true.
	return true;
}

//Esta funci�n gestiona los mensajes provenientes de la ventana.
//Par�metros:
// HWND lWnd -> es el handle de la ventana que ha disparado el evento (la �nica que tenemos).
// UINT lMsg -> tipo del mensaje que hemos recibido.
// WPARAM lWParam y LPARAM lLParam -> son par�metros cuyo contenido depende del tipo de mensaje.
LRESULT CALLBACK cWindow::WndProc( HWND lWnd, UINT lMsg, WPARAM lWParam, LPARAM lLParam)    
{
	//Se act�a seg�n el tipo de mensaje recibido.
	switch (lMsg)
	{
		//Se procesa (captura) el mensaje de cierre de ventana , indicando 
		// a Windows que queremos que se cierre la aplicaci�n:
		case WM_CLOSE:          
		{
			PostQuitMessage(0);                                  
			return 0;                                      
		}
		//Se procesa (captura) el mensaje que env�a el sistema operativo 
		// avisando de que el protector de pantalla va a activarse o que la pantalla quiere entrar 
        // en modo ahorro de energ�a. 
		// En este caso se impide que el sistema operativo lleve a cabo dichas acciones:
		case WM_SYSCOMMAND:                                              
		{
			 switch (lWParam)                                     
			 {
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0; // Interrupt the action
			 }
			 break;
		}
	}
	//Todos los mensajes que no se controlen se ignoran dejando que el sistema operativo los gestione,
	// para ello se invoca la funci�n DefWindowProc:
    return DefWindowProc(lWnd,lMsg,lWParam,lLParam);
}

//Esta funci�n se encargar� de actualizar la ventana y atender a posibles eventos que 
// provengan de la misma.
// Es decir, le indica a Windows cuando debe procesar los mensajes que tiene pendiente la 
// ventana.
void cWindow::Update()
{
   MSG lMsg;  

   //Si hay mensajes en espera, se llama a las funciones de Windows para procesar dichos 
   // mensajes:
   if (PeekMessage(&lMsg,NULL,0,0,PM_REMOVE))  
   {
      //Se comprueba si se ha recibido un mensaje de salida y de ser as�, establecemos el
	  // atributo mbCloseApplication a true:	   
      if (lMsg.message==WM_QUIT)                     
      {
         mbCloseApplication = true;            
      }
      else
      {
         TranslateMessage(&lMsg);
		 DispatchMessage(&lMsg);
      }
   }
}

//Esta funci�n se encargar� de eliminar la ventana.
bool cWindow::Deinit()
{
   //Se comprueba si tenemos que restablecer la resoluci�n original de la 
   // pantalla:
   if (mProperties.mbFullscreen)
   {
      //Volver a la resoluci�n del escritorio:
      ChangeDisplaySettings(NULL,0);
      ShowCursor(TRUE); 
   }
   //Se comprueba cada uno de los recursos que hemos adquirido en la 
   // inicializaci�n y se liberan: 

   if (mDC && !ReleaseDC(mWnd, mDC))
   {
      mDC=NULL;
      return false;
   }
   if (mWnd && !DestroyWindow(mWnd))
   {
      mWnd=NULL;
      return false;
   }
   if (!UnregisterClass(mProperties.macApplicationName.c_str(),mInstance))
   {
      mInstance=NULL;
      return false;
   }
   return true;
}