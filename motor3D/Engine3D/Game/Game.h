#ifndef Game_H
#define Game_H

#include "..\Utility\Singleton.h"
#include "..\Window\ApplicationProperties.h"
#include "..\Graphics\Camera.h"
#include "..\Graphics\Fonts\acgfx_font.h"
#include "..\Gameplay\Object\Object.h"
#include "..\Physics\cPhysics.h"
#include "..\Physics\cPhysicObject.h"

#include "..\Gameplay\CharacterPos\CharacterPos.h"
#include "..\Graphics\GodCamera.h"
#include "..\Gameplay\Vehicle\Vehicle.h"
#include "..\Gameplay\Scene\Skybox.h"
#include "..\Gameplay\Vehicle\Mustang.h"
#include "..\Gameplay\Terrain\Heightmap.h"

//Clase que hace uso del Patr�n Singleton definido en Singleton.h, para iniciar, actualizar, dibujar
// y finalizar el juego
class cGame : public cSingleton<cGame>
{
	friend class cSingleton<cGame>;
protected:
		cGame() { ; } // Protected constructor	
		//Variable privada para gestionar si tenemos que salir de la aplicaci�n.
		bool mbFinish;
		//Clase para los atributos de la ventana (cWindow) que se crear� en el m�todo Init.
		cApplicationProperties mProperties;
        //Instancia de la clase cCamera que representa la camera de nuestro juego.
		cCamera m3DCamera;
		//Instancia de la clase cCamera. Ser� una c�mara 2D para mostrar texto.
		cCamera m2DCamera;
		//Instancia de la clase cFont para crear texto.
		cFont mFont;
		//Instancia de la clase cResourceHandle que se encarga de manejar la escena del juego.
		cResourceHandle mDuck;
		cResourceHandle mMusExt;
		cResourceHandle mMusInt;
		cResourceHandle mMusMet;
		cResourceHandle mMusNeu;
		Mustang mMustang;
		cObject mTire;
		cObject mExt;
		cObject mInt;
		cObject mMet;

		cResourceHandle mItemCongelacion;

		// Instance that manages skeletal animation part of the engine
		cResourceHandle mSkeletalMesh;
		// Bullet physics object
		//cPhysics mPhysics;


		// Terrain
		Heightmap mHeightmap;

		// C�mara de juego o godmode
		bool mbInGame;
		// Modo de rasterizacion solida/wireframe
		bool mbRasterizationMode;
		// Camara godmode
		GodCamera mGodCamera;
		// Vehiculo 
		//Vehicle mVehicle;
		// Skybox
		Skybox mSkybox;

public:
	
	//Funci�n para inicializar el juego	
	bool Init();

	//Funci�n para actualizar el juego
	void Update( float lfTimestep );

	//Funci�n para renderizar el juego
	void Render();

	//Funci�n para finalizar el juego
	bool Deinit();

	//Funci�n que devuelve el valor de mbFinish, para chequear el final del juego.
	inline bool HasFinished() { return mbFinish; }

	//Funci�n para cargar los recursos necesarios para el juego.
	bool LoadResources( void );

	// Time since application start
	inline float GetAcumulatedTime() { return mfAcTime; }	
	
	// Get camera object
	cCamera Get3DCamera(){ return m3DCamera; };
	
	float mfAcTime;

	// Object for skeletal meshes
	cPhysicObject mObject;

	// Physic objects
	std::vector<cPhysicObject> maSphereObjects;
	cPhysicModel mSphereModel;
	cPhysicModel mBoxModel;

	// Devuelve el objeto mapa de altura
//	Heightmap GetHeightmap(){ return mHeightmap; }
	// Devuelve el objeto entity
//	CEntity GetEntity(){ return mEntity; }
};

#endif