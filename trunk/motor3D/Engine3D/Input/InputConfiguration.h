/*
En este fichero se define un enum donde se declaran las posibles acciones.
Esto sirve para indicar desde el juego (clase Game) al InputManager la relaci�n que
existe entre las acciones y los dispositivos. 
*/

#ifndef INPUT_CONFIGURATION_H
#define INPUT_CONFIGURATION_H

enum eInputActions {
    eIA_CloseApplication = 0,		// Acci�n para cerrar la aplicaci�n. 
	eIA_ReloadEffectManager = 1,	// Reload action.
	eIA_PlayJog = 2,				// Play animation Jog.
	eIA_StopJog = 3,				// Stop animation Jog.
	eIA_PlayWave = 4,				// Play animation Wave.
	eIA_StopWave = 5,				// Stop animation Wave.
	eIA_MoveFront = 6,				// Front movement.
	eIA_MoveBack = 7,				// Back movement.
	eIA_MoveLeft = 8,               // Left movement.
	eIA_MoveRight = 9,				// Right movement.
	eIA_MouseYaw = 10,				// Rotaci�n eje Y del rat�n.
	eIA_MousePitch = 11,			// Rotaci�n eje X del rat�n.
	eIA_SwitchCamera = 12,			// Switch entre c�mara juego/godmode.
	eIA_SwitchFillLineMode = 13,	// Switch entre rasterizacion solida/wireframe.
    eIA_Count						//Indica cuantas acciones de entrada hay.
}; 

#endif