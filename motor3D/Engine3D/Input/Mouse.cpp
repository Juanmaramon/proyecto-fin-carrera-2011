#include <cassert>
#include "Mouse.h"
#include "InputManager.h"

//M�todo que inicializa el dispositivo.
void cMouse::Init()
{
   //Clear the input buffer.
   //Se inicializa el array de float a 0.0 en todas sus posiciones.
   memset(mafInput, 0, eMouse_Total*4);      
   
   assert(cInputManager::Get().mpOISInputManager);  
   
   //Se crea el objeto Mouse.
   mpOISMouse = (OIS::Mouse*)cInputManager::Get().mpOISInputManager->createInputObject( OIS::OISMouse, true );
   
   //Se registran los callbacks.
   mpOISMouse->setEventCallback( this );

   //Se indica el �rea de nuestra pantalla.
   mpOISMouse->getMouseState().height = 480;
   mpOISMouse->getMouseState().width = 640;


   mbIsValid = true;  
}

//M�todo que libera el dispositivo.
void cMouse::Deinit(void)
{
   assert(cInputManager::Get().mpOISInputManager);
   //Se destruye el objeto Mouse.
   cInputManager::Get().mpOISInputManager->destroyInputObject( mpOISMouse );
   //Se resetean los valores de las variables.
   mpOISMouse = 0;
   mbIsValid = false;
}

//M�todo que se llamar� en cada frame para actualizar el estado del dispositivo.
void cMouse::Update(void)
{   
      //This fires off buffered events for Mouse
      assert(mpOISMouse);
	  //Le indicamos a la librer�a (OIS) que debe actualizar el estado del device (Mouse).
      mpOISMouse->capture();   
}

//M�todo que permitir� conocer el valor de uno de los canales de entrada (bot�n, ejes).
float cMouse::Check(unsigned luiEntry)
{
   if ( luiEntry < eMouse_Total )
   {
      return mafInput[luiEntry];
   }
   return 0.0f;
}

//Implementaci�n de los callbacks:

bool cMouse::mousePressed( const OIS::MouseEvent &lArg, OIS::MouseButtonID id )
{
   if ( id <= eMouse_ButtonLast )
   {
      mafInput[id] = 1.0f;
   }
   //	std::cout << std::endl << lArg.device->vendor() << ". Button Pressed # " << liButton;
	return true;
}

bool cMouse::mouseReleased( const OIS::MouseEvent &lArg, OIS::MouseButtonID id )
{
   if ( id <= eMouse_ButtonLast )
   {
      mafInput[id] = 0.0f;
   }
   //	std::cout << std::endl << lArg.device->vendor() << ". Button Released # " << liButton;
   return true;
}

bool cMouse::mouseMoved( const OIS::MouseEvent &lArg )
{   
	mafInput[eMouse_AxisX] = (float)lArg.state.X.abs;
	mafInput[eMouse_AxisY] = (float)lArg.state.Y.abs;
	mafInput[eMouse_AxisZ] = (float)lArg.state.Z.abs;
	
	return true;	

}
