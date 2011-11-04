#include "Keyboard.h"
#include "InputManager.h"

#include <assert.h>

//M�todo que inicializa el dispositivo.
void cKeyboard::Init()
{	
	//Clear the input buffer.
	//Se inicializa el array de booleanos a false.
	memset(mabInputBuffer, 0, kuiInputChanelSize);
	 
    //Se crea el objeto teclado. 
	assert(cInputManager::Get().mpOISInputManager);
	OIS::InputManager * lpOISInputManager = cInputManager::Get().mpOISInputManager;
	mpOISKeyboard = (OIS::Keyboard*)lpOISInputManager->createInputObject(OIS::OISKeyboard, true );

	//Se registran los callbacks. 
	mpOISKeyboard->setEventCallback( this );
	mbIsValid = true;
}

//M�todo que libera el dispositivo.
void cKeyboard::Deinit(void)
{
   assert(cInputManager::Get().mpOISInputManager);
   
   //Se destruye el objeto teclado.
   cInputManager::Get().mpOISInputManager->destroyInputObject(mpOISKeyboard);
   
   //Se resetean los valores de las variables.
   mpOISKeyboard = 0;
   mbIsValid = false;
}

//Implementaci�n de los callbacks:
 
bool cKeyboard::keyPressed( const OIS::KeyEvent &lArg )
{
   mabInputBuffer[lArg.key] = true;
   return true;
}
 
bool cKeyboard::keyReleased( const OIS::KeyEvent &lArg )
{
   mabInputBuffer[lArg.key] = false;
   return true;
}

//M�todo que se llamar� en cada frame para actualizar el estado del dispositivo.
void cKeyboard::Update(void)
{
   //This fires off buffered events for keyboards
   assert(mpOISKeyboard);
   //Le indicamos a la librer�a (OIS) que debe actualizar el estado del device (teclado).
   mpOISKeyboard->capture();
}

//M�todo que permitir� conocer el estado de uno de los canales de entrada (si una tecla est� pulsada o no).
float cKeyboard::Check(unsigned luiEntry)
{
   if (mabInputBuffer[luiEntry])
   {
      return 1.0f;
   }
   return 0.0f;
}
