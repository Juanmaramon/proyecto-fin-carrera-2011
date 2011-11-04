#include <windows.h>
#include <iostream>
#include <sstream>
 
#include "InputManager.h"
#include "../Window/Window.h"
 

//M�todo que inicializa el InputManager.
void cInputManager::Init( const tActionMapping laActionMapping[],
                         unsigned luiActionCount )
{
	//Inicializaci�n de la librer�a OIS.    
	OIS::ParamList pl;
 
	//Esta librer�a (OIS), necesita el handle de la ventana, es por ello que
	// se obtiene el handle de la ventana y se le pasa a la librer�a. 
  
	std::ostringstream wnd;
	wnd << (size_t)cWindow::Get().GetHWND();

	pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

	//Default mode is foreground exclusive..but, we want to show mouse - so nonexclusive
	pl.insert(std::make_pair(std::string("w32_mouse"),
		   std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), 
		   std::string("DISCL_NONEXCLUSIVE")));

	//This never returns null.. it will raise an exception on errors
	mpOISInputManager = OIS::InputManager::createInputSystem(pl);

	//Lets enable all addons that were compiled in:
	mpOISInputManager->enableAddOnFactory(OIS::InputManager::AddOn_All);

	//Inicializaci�n de los dispositivos: crear los objetos 
	// dispositivos y llamar a sus m�todos Init.

	//Teclado.
	cGenericDevice * lpDevice = new cKeyboard;
	lpDevice->Init(); 
    
	//Se a�aden los dispositivos al vector mDevices.
	//El orden de los dispositivos en este array deben coincidir 
	// con los �ndices asignados en el enum "eDevices":
	//   eKeyboard = 0 -> El teclado estar� en el �ndice 0 del vector.
	//   ePad = 1      -> El JoyStick/GamePad estar� en el �ndice 1 del vector.
    //   eMouse = 2      -> El Mouse estar� en el �ndice 2 del vector.
	mDevices.push_back(lpDevice);       

	//JoyStick.
	lpDevice = NULL;
	lpDevice = new cPad;
	lpDevice->Init();
	mDevices.push_back(lpDevice);

	//Mouse.
	lpDevice = NULL;
	lpDevice = new cMouse;
	lpDevice->Init();
	mDevices.push_back(lpDevice);

	//Inicializar el vector de InputActions: establecer el tama�o adecuado al vector y 
	// llamar al Init de las acciones. 
	mActions.resize( luiActionCount );
	for (unsigned int i = 0; i < mActions.size(); i++)
	{
		cInputAction lAction;
        lAction.Init(); 
		mActions.at(i) = lAction;
	}

	//Crear la estructura que mantiene el mapeado de cada una de las acciones: 
	// Esta estructura se rellenar� con la tabla (laActionMapping[]) que recibimos por par�metro.
	mMapped.resize( luiActionCount );
	int liIndex = 0;
	while ( laActionMapping[ liIndex ].miAction >= 0 )
	{
		cInputEntry lEntry;
		lEntry.muiDevice = laActionMapping[ liIndex ].miDevice;
		lEntry.muiChannel = laActionMapping[ liIndex ].miChannel;
		mMapped[ laActionMapping[ liIndex ].miAction ].push_back(lEntry);

		++liIndex;
	}
}

//M�todo que se encarga de actualizar el estado de las acciones consultando el 
// estado de los dispositivos.
void cInputManager::Update(const float &lfTimestep)
{
	 // Update the devices
    int liSize = mDevices.size();
    for (int liDeviceId = 0; liDeviceId < liSize; ++liDeviceId){
        if (mDevices[liDeviceId] != NULL)
            mDevices[liDeviceId]->Update();
    }
 
    // For each action
    liSize = mMapped.size();
    for (int lActionId = 0; lActionId < liSize; ++lActionId)
    {
        // For each map
        int liMapSize = mMapped[lActionId].size();
        float lfValue = 0.0f;
        for (int liMapId = 0; liMapId < liMapSize; ++liMapId)
        {
            // Calculate the value of the input
            const cInputEntry& lEntry = mMapped[lActionId][liMapId];
            lfValue += Check(lEntry.muiDevice, lEntry.muiChannel);
        }
 
        // Set the value in the action
        mActions[lActionId].Update(lfTimestep, lfValue);
    }
}

//M�todo comprueba el estado del dispositivo, es decir, si �ste es v�lido
float cInputManager::Check(int liDevice, int liChannel) {
    if (mDevices[liDevice] != NULL && mDevices[liDevice]->IsValid() )
        return (mDevices[liDevice]->Check(liChannel));
    return 0.0f;
}

//M�todo que libera los recursos solicitados.
void cInputManager::Deinit()
{
   // Deinit devices
   for (unsigned luiIndex = 0; luiIndex < mDevices.size(); ++luiIndex)
   {
      mDevices[luiIndex]->Deinit();
      delete mDevices[luiIndex];
   }
   mDevices.resize(0);
   mActions.resize(0);
   mMapped.resize(0);
 
   //Para destruir la librer�a OIS s�lo hay que hacer una llamada a destroyInputSystem.
   if( mpOISInputManager )
   {  
	  OIS::InputManager::destroyInputSystem(mpOISInputManager);
	  mpOISInputManager = NULL;
   }
}