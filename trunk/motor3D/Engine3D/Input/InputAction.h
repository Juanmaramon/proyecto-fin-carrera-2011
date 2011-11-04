/*
Esta clase representa el estado de una acci�n
l�gica (Saltar, Aceptar, ...) para que el juego pueda obtener la 
entrada en forma de esas acciones l�gicas y no como entrada espec�fica de un dispositivo concreto (bot�n 1, tecla A).
*/

#ifndef INPUT_ACTION_H
#define INPUT_ACTION_H
 
class cInputAction
{
	public:

		//Inicializa los valores de los atributos a false o a 0.0f, dependiendo del tipo.
		void Init( void );

		//Ser� llamada en cada frame desde el InputManager para actualizar el 
		// estado de la acci�n.
		void Update (float lfTimestep, float lfValue);
		
		//La mayor parte de las siguientes funciones son para acceder al estado de la 
		// acci�n:

		//Indica si la acci�n est� activa o pulsada.
		inline bool GetIsPressed(void) const { return mbIsPressed;}

		//Indica si la acci�n no est� activa o no pulsada.
		inline bool GetIsReleased(void) const { return !mbIsPressed;}

		//Indica si la acci�n ha pasado de inactiva a activa. S�lo 
		// devuelve true en el primer frame que la acci�n est� activa.
		inline bool GetBecomePressed(void) const {return mbBecomePressed;}

		//Indica si la acci�n ha pasado de activa a inactiva. S�lo 
		// devuelve true en el primer frame que la acci�n est� inactiva.
		inline bool GetBecomeReleased(void)const {return mbBecomeReleased;}

		//Devuelve el tiempo que la acci�n lleva activa.
		inline float GetPressedTime(void) const {return mfTimePressed; }

		//Devuelve el valor de la acci�n.
		inline float GetValue(void) const    { return mfValue; }

	private:
		float mfValue;
		float mfTimePressed;
		bool  mbIsPressed;
		bool  mbWasPressed;
		bool  mbBecomeReleased;
		bool  mbBecomePressed;
};
 
#endif