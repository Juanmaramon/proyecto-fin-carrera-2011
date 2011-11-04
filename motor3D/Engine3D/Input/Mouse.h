/*
Esta Clase implementa la interfaz cGenericDevice.
La clase cPad representa a un dispositivo, el Mouse.

En la implementaci�n de esta clase haremos uso de la librer�a OIS, y todas las llamadas
relativas a esta librer�a quedar�n encapsuladas por la clase cMouse.

Los dispositivos en OIS funcionan en su mayor�a a trav�s de callbacks (funci�n que recibe como 
argumento la direcci�n o puntero de otra funci�n para poder ejecutarla) y el Mouse no es una 
excepci�n. 
Habr� un callback para cuando se pulse un bot�n (funci�n "mousePressed"), otro 
callback para cuando el bot�n se suelta (funci�n "mouseReleased") y otro
para cuando se mueve (funci�n "mouseMoved").

Lo que haremos ser� tener un array de float ("mafInput") de tama�o "kuiInputChanelSize" (8 botones + 3 componentes
 de la posici�n (ejes X, Y, Z) del mouse ).
Cada vez que recibamos una llamada indicando que se ha pulsado (o soltado) un bot�n o que se producido cierto
movimiento, pondremos en la posici�n del array, correspondiente al bot�n o movimiento, el valor actualizado.

Esta clase es HIJA de cGenericDevice (PADRE) y por tanto heredar� de ella.
Los m�todos en el padre han sido declarados de tipo VIRTUAL, para que cada hijo 
los implemente a su manera.
*/

#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>
#include "GenericDevice.h"

#include "OISInputManager.h"
#include "OISException.h"
#include "OISMouse.h"
#include "OISEvents.h"


//Enumerado con los botones y movimientos(X,Y) del Mouse, es decir, los diferentes canales de entrada
// que tiene el mando.
//Este enumerado se usar� como referencia para posicionarnos en el array "mafInput".
enum eMouse
{
   // Buttons
   eMouse_ButtonLeft = 0, eMouse_ButtonRight, eMouse_ButtonMiddle,
   eMouse_Button3, eMouse_Button4,	eMouse_Button5, eMouse_Button6,	eMouse_Button7,
   eMouse_ButtonLast = eMouse_Button7,

   // Axis (Ejes)
   eMouse_AxisX, eMouse_AxisY, eMouse_AxisZ,

   eMouse_Total
};


class cMouse : public cGenericDevice, public OIS::MouseListener
{
   public:
      cMouse() : mbIsValid(false) { ; }
	  
	  //Inicializa el dispositivo.
      void Init();                      

	  //Libera el dispositivo.
      void Deinit(void);

	  //Se llamar� en cada frame para actualizar el estado del dispositivo.
      void Update(void);

	  //Permitir� conocer el valor de uno de los canales de entrada (bot�n, coordenada).
      float Check(unsigned luiEntry);

      //Indicar� si el dispositivo es v�lido y le podemos consultar el estado de sus canales.
      inline bool IsValid(void) { return mbIsValid; }

      // Listeners for the Pad.
	  //Estas funciones son los callbacks (funci�n que recibe como 
      // argumento la direcci�n o puntero de otra funci�n para poder ejecutarla).
	  bool mouseMoved( const OIS::MouseEvent &lArg ); //Rat�n movido.      
	  bool mousePressed( const OIS::MouseEvent &lArg, OIS::MouseButtonID id ); //Bot�n pulsado.
	  bool mouseReleased( const OIS::MouseEvent &lArg, OIS::MouseButtonID id ); //Bot�n soltado.	  

   private:

	  //El puntero mpOISMouse contiene el puntero al objeto Mouse suministrado por OIS.	
	  OIS::Mouse* mpOISMouse;
	   
	  //Array de float que guardar� el valor de los diferentes canales de entrada (botones, coordenadas).
	  //Sus posiciones se corresponden con el enumerado "eMouse".
      float mafInput[eMouse_Total];
      
      bool mbIsValid;
};

#endif
