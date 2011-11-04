/*
Esta Clase implementa la interfaz cGenericDevice.
La clase cKeyboard representa a un dispositivo, el teclado.

En la implementaci�n de esta clase haremos uso de la librer�a OIS, y todas las llamadas
relativas a esta librer�a quedar�n encapsuladas por la clase cKeyboard.

Los dispositivos en OIS funcionan en su mayor�a a trav�s de callbacks (funci�n que recibe como 
argumento la direcci�n o puntero de otra funci�n para poder ejecutarla) y el teclado no es una 
excepci�n. 
Por lo tanto, habr� un callback para cuando se pulsa una tecla (funci�n "keyPressed") y otro 
callback para cuando la tecla se suelta (funci�n "keyReleased"). 
Por ello, lo que haremos ser� tener un array de 256 booleanos ("mabInputBuffer") y cada vez que recibamos una llamada 
indicando que se ha pulsado una tecla, pondremos el booleano correspondiente a esa tecla a true.
Cuando recibamos la llamada contraria (tecla soltada), pondremos el booleano correspondiente 
otra vez a false.

Esta clase es HIJA de cGenericDevice (PADRE) y por tanto heredar� de ella.
Los m�todos en el padre han sido declarados de tipo VIRTUAL, para que cada hijo 
los implemente a su manera.
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H
 
#include <Windows.h>
#include "GenericDevice.h"
 
#include "OISInputManager.h"
#include "OISException.h"
#include "OISKeyboard.h"
#include "OISEvents.h"
 
class cKeyboard : public cGenericDevice, public OIS::KeyListener
{
   public:
      cKeyboard() : mbIsValid(false) { ; }

	  //Inicializa el dispositivo.
      void Init();

	  //Libera el dispositivo.
      void Deinit(void);

	  //Se llamar� en cada frame para actualizar el estado del dispositivo.
      void Update(void);

	  //Permitir� conocer el estado de uno de los canales de entrada (si una tecla est� pulsada o no).
      float Check(unsigned luiEntry);

	  //Indicar� si el dispositivo es v�lido y le podemos consultar el estado de sus canales.
      inline bool IsValid(void) { return mbIsValid; }
 
      //Listeners for the keyboard.
	  //Estas funciones son los callbacks (funci�n que recibe como 
      // argumento la direcci�n o puntero de otra funci�n para poder ejecutarla).
      bool keyPressed( const OIS::KeyEvent &lArg ); //Tecla pulsada.
      bool keyReleased( const OIS::KeyEvent &lArg ); //Tecla soltada.
 
   private:

      //El puntero mpOISKeyboard contiene el puntero al objeto teclado suministrado por OIS.
      OIS::Keyboard* mpOISKeyboard;	  
 
      static const unsigned kuiInputChanelSize = 256;

	  //Array de booleanos que guardar� el estado de las teclas (pulsada: true, no pulsada: false).
      bool mabInputBuffer[kuiInputChanelSize];
      bool mbIsValid;
};
 
#endif