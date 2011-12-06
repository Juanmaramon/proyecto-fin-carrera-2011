/*
Esta clase representa el gestor de entrada. A�na y coordina todos los componentes.
Se encarga de actualizar y leer los dispositivos, y con la informaci�n que obtiene de ellos 
actualizar las acciones correspondientes.
*/

#ifndef INPUT_MANAGER
#define INPUT_MANAGER
 
#include <vector>
#include "InputAction.h"
#include "InputEntry.h"
#include "Keyboard.h"
#include "Pad.h"
#include "Mouse.h"
#include "..\Utility\Singleton.h"
 
#define IsPressed( ACTION ) cInputManager::Get().GetAction( ACTION ).GetIsPressed();
#define BecomePressed( ACTION ) cInputManager::Get().GetAction( ACTION ).GetBecomePressed();
#define GetValue(ACTION) cInputManager::Get().GetAction( ACTION ).GetValue()

class OIS::InputManager;
 
//Enumerado que identifica los dispositivos de entrada.
enum eDevices
{
    eKeyboard = 0, //Teclado
	ePad,          //JoyStick/GamePad
	eMouse         //Rat�n 
};

//Esta estructura representa una fila de la tabla ("kaActionMapping" de InputConfiguration.cpp)
// que se utilizar� para inicializar el InputManager.
//El primer atributo (miAction) es un identificador de la acci�n, el segundo (miDevice) 
// es un identificador del dispositivo al que se va a mapear la acci�n y el tercero (miChannel)
// es el canal dentro del dispositivo.
struct tActionMapping
{
    int miAction;
    int miDevice;
    int miChannel;
};
 
//Esta clase usar� el patr�n Singleton definido en "..Utility/Singleton.h"
class cInputManager : public cSingleton<cInputManager>
{
   public:

      //Inicializa la clase InputManager.
      void Init( const tActionMapping laActionMapping[], unsigned luiActionCount );

	  //Actualiza la clase InputManager.
      void Update(const float &lfTimestep);

	  //Finaliza la clase InputManager.
      void Deinit();
 
      inline cInputAction &GetAction(const int &lActionId) { return mActions[lActionId]; }
 
      friend class cSingleton<cInputManager>;
 
   protected:
      cInputManager() { ; } // Protected constructor
 
   private:

    //Comprueba el estado del dispositivo, es decir, si �ste es v�lido
    float Check(int liDevice, int liChannel);
 
	//Este vector se encargar� de mantener el estado de cada una de las acciones. 
	//La acci�n que est� en la posici�n 0 del vector se corresponde con la acci�n 
	// con identificador 0 (en el enum "eInputActions"), la que est� en la posici�n 1, 
	// con la del identificador 1, y as� sucesivamente. 
    std::vector <cInputAction> mActions;

	//Array con los dispositivos que existen en el juego. 
	//El orden de los dispositivos en este array deben coincidir con los �ndices asignados 
	// en el enum "eDevices".
    std::vector <cGenericDevice *> mDevices;

	//Estructura que mantiene el mapeado de cada una de las acciones (una acci�n 
    // puede estar mapeada en varios dispositivos).
	//Esta estructura es un vector de vectores del tipo cInputEntry. 
	//Por ejemplo, supongamos que tenemos una acci�n con identificador 
	// 6, pues mMapped[6] devolver� el vector con cada uno de los mapeos de la 
	// acci�n con �ndice 6.  
    std::vector < std::vector < cInputEntry > > mMapped;
 
    // Specific OIS vars
    friend class cKeyboard;
	friend class cPad;
	friend class cMouse;
	//Puntero al sistema de entrada de la librer�a OIS.
    OIS::InputManager * mpOISInputManager;
};
 
#endif