#include "InputConfiguration.h"
#include "InputManager.h"
 

//Tabla (array) cuyos elementos son del tipo "tActionMapping" de InputManager.h.
//Esta tabla indica la relaci�n entre las acciones y los dispositivos.
//El primer par�metro de cada terna es del enumerado "eInputActions" del fichero InputConfiguration.h,
// y el segundo par�metro es del enumerado "eDevices" de InputManager.h.
tActionMapping kaActionMapping[] = {
	{ eIA_CloseApplication, eKeyboard, OIS::KC_ESCAPE }, // Esta fila indica que la acci�n salir con el teclado est� asociada a la tecla escape.	 
	{ eIA_ReloadEffectManager, eKeyboard, OIS::KC_R },	 // This row associate R keyboard key with the reload action
	{ eIA_PlayJog, eKeyboard, OIS::KC_P },				 // This row associate P keyboard key with the Play the Joy animation action
	{ eIA_MoveBack, eKeyboard, OIS::KC_S },				 // This row associate S keyboard key with the Stop the Joy animation action
	{ eIA_MoveLeft, eKeyboard, OIS::KC_A },				 // This row associate S keyboard key with the Stop the Joy animation action
	{ eIA_MoveRight, eKeyboard, OIS::KC_D },			 // This row associate S keyboard key with the Stop the Joy animation action
	{ eIA_PlayWave, eKeyboard, OIS::KC_Q },				 // This row associate Q keyboard key with the Stop the Wave animation action
	{ eIA_MoveFront, eKeyboard, OIS::KC_W },			 // This row associate W keyboard key with the Stop the Wave animation action
	{ eIA_MoveFront, eKeyboard, OIS::KC_UP },			 // This row associate UP keyboard key with the front movement of the skeleton
	{ eIA_MoveBack, eKeyboard, OIS::KC_DOWN },			 // This row associate DOWN keyboard key with the back movement of the skeleton
	{ eIA_MoveLeft, eKeyboard, OIS::KC_LEFT },			 // This row associate LEFT keyboard key with the left movement of the skeleton
	{ eIA_MoveRight, eKeyboard, OIS::KC_RIGHT },		 // This row associate RIGHT keyboard key with the right movement of the skeleton
	{ eIA_CloseApplication, ePad, ePad_Button1 },		 // Esta fila indica que la acci�n salir con el JoyStick/GamePad est� asociada al bot�n 1.	                                                     	 
	{ eIA_MouseYaw, eMouse, eMouse_AxisX },				 // Esta fila indica que la coordenada X del rat�n ser� el grado de giro sobre el eje Y
	{ eIA_MousePitch, eMouse, eMouse_AxisY },			 // Esta fila indica que la coordenada Y del rat�n ser� el grado de giro sobre el eje X
	{ eIA_SwitchCamera, eKeyboard, OIS::KC_1 },			 // Esta fila asocia la tecla 1 con el cambiar de modo de c�mara (game/godmode)
	//	{ eIA_CloseApplication, eMouse, eMouse_ButtonMiddle }, //Esta fila indica que la acci�n salir con el mouse est� asociada al bot�n central.	
	{ eIA_SwitchFillLineMode, eKeyboard, OIS::KC_2 },	 // Esta fila asocia la tecla 1 con el cambiar la rasterizacion de solida a modo wireframe.
	{ eIA_SwitchCameraAux, eKeyboard, OIS::KC_SPACE },		 // Esta fila asocia la tecla barra espaciadora con la funcion de cambiar la orientacion de la camara.
	{ eIA_Fire, eMouse, eMouse_ButtonLeft },		 // Esta fila asocia la tecla izquierda del raton con la accion de disparo.
	{ -1, -1, -1}  // End of the table (Marca el final de la tabla)
};