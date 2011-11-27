//Clase  cCharacter, la cual representar� a un personaje cualquiera 
//del juego (player,   enemigo   �). Esta clase contar� con la posici�n, orientaci�n, 
//velocidad m�xima y velocidad angular del personaje como atributos.
#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\MathLib\MathLib.h"
#include "..\MathLib\MathConst.h"
#include "Behaviour\BehaviourBase.h"

class cCharacter
{
	private:
		cVec3 mPosition;  //Posici�n del personaje
		float mfYaw;      //Orientaci�n del personaje (yaw)
		float mfSpeed;    //Velocidad m�xima del personaje
		float mfAngSpeed; //Velocidad angular del personaje	
		cBehaviourBase * mpActiveBehaviour; //Comportamiento del personaje
		int miId;
	
	public:
		cCharacter() { ; } //Constructor

		//Inicializa a unos valores por defecto los atributos de la clase.
		//Tambien se asigna el identificador del personaje.
		void Init(int id);

		//Implementaci�n vac�a.
		void Deinit(); 

		//Implementaci�n vac�a.
		void Update( float lfTimestep );

		//Renderiza un punto que representa la posici�n del personaje y una 
		//peque�a l�nea que parte de dicho punto que representa la orientaci�n 
		//del personaje. 
		void Render();

		//Getters
		inline const cVec3 &GetPosition() const { return mPosition; }
		inline float GetYaw() const { return mfYaw; }
		inline float GetSpeed() const { return mfSpeed; }
		inline float GetAngSpeed() const { return mfAngSpeed; }
		inline cBehaviourBase* GetActiveBehaviour() { return mpActiveBehaviour; }
		inline int GetId() const { return miId; }
		
		//Devuelve el vector Front del personaje.
		inline cVec3 GetFront() const { return  cVec3(   sinf(mfYaw),  0.0f, cosf(mfYaw) ); }
		
		//Devuelve el vector Left del personaje.
		//Se define HALF_PI en el fichero Math/MathConst.h como sigue:
		// #define HALF_PI  (PI / 2.0f )
		inline  cVec3 GetLeft() const { return cVec3( sinf(mfYaw + HALF_PI), 0.0f, cosf(mfYaw + HALF_PI) ); }

		//Devuelve el vector Right del personaje.
		inline cVec3 GetRight() const { return -GetLeft(); }
		
		//Setters
		void SetPosition( const cVec3 &lPosition );
		void SetYaw( float lfYaw );
		void SetSpeed( float lfSpeed );
		void SetAngSpeed( float lfAngSpeed );
		void SetActiveBehaviour( cBehaviourBase * lpBehaviour );
};

#endif


