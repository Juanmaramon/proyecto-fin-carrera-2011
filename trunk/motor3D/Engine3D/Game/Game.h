#ifndef Game_H
#define Game_H

#include "..\Utility\Singleton.h"
#include "..\Window\ApplicationProperties.h"
#include "..\Graphics\Camera.h"
#include "..\Graphics\Fonts\acgfx_font.h"
#include "..\Gameplay\Object\Object.h"
#include "..\Physics\cPhysics.h"
#include "..\Physics\cPhysicObject.h"
#include "../Graphics/Textures/Texture.h"

#include "..\Gameplay\CharacterPos\CharacterPos.h"
#include "..\Graphics\GodCamera.h"
#include "..\Gameplay\Vehicle\Vehicle.h"
#include "..\Gameplay\Scene\Skybox.h"
#include "..\Gameplay\Vehicle\Mustang.h"
#include "..\Gameplay\Terrain\Heightmap.h"
#include "..\Gameplay\Vehicle\Truck.h"
#include "..\Gameplay\Terrain\Ruin.h"
#include "..\Gameplay\Terrain\Bush.h"

#define RUINS_NUMBER 5

#define BUSH_NUMBER 20

//Clase que hace uso del Patrón Singleton definido en Singleton.h, para iniciar, actualizar, dibujar
// y finalizar el juego
class cGame : public cSingleton<cGame>
{
	friend class cSingleton<cGame>;
protected:
		cGame() { ; } // Protected constructor	
		//Variable privada para gestionar si tenemos que salir de la aplicación.
		bool mbFinish;
		//Clase para los atributos de la ventana (cWindow) que se creará en el método Init.
		cApplicationProperties mProperties;
        //Instancia de la clase cCamera que representa la camera de nuestro juego.
		cCamera m3DCamera;
		//Instancia de la clase cCamera. Será una cámara 2D para mostrar texto.
		cCamera m2DCamera;
		//Instancia de la clase cFont para crear texto.
		cFont mFont;

		cPhysicObject mModelObject;
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

		// Mustang destruido
		cResourceHandle mMustangExtDes;
		cResourceHandle mMustangIntDes;
		cObject mMustangExteriorDes, mMustangInteriorDes;

		// Enemigo 1 (Truck)
		cResourceHandle mTruckNeu;
		cResourceHandle mTruckArm;
		cResourceHandle mTruckExt;
		Truck mTruck;
		cObject mTruckTire;
		cObject mTruckExterior;
		cObject mTruckWea;

		// Enemigo 1 destruido (Truck)
		cResourceHandle mTruckExtDes;
		cResourceHandle mTruckWeaDes;
		cObject mTruckExteriorDes;
		cObject mTruckWeaponDes;

		// Ruina
		cResourceHandle mRuinModel;
		cObject mRuinObs;
		Ruin mRuin;
		std::vector<Ruin> maRuins;

		// Matorral
		cResourceHandle mBushTexture, mBushTextureMask;
		Bush mBush;
		std::vector<Bush> maBushes;

		cResourceHandle mCrosshair;

		cResourceHandle mItemCongelacion;

		cResourceHandle mMad_drive_loading, mMad_drive, mLoading;

		// Instance that manages skeletal animation part of the engine
		cResourceHandle mSkeletalMesh;
		// Bullet physics object
		//cPhysics mPhysics;

		// Terrain
		Heightmap mHeightmap;

		// Cámara de juego o godmode
		bool mbInGame;
		// Modo de rasterizacion solida/wireframe
		bool mbRasterizationMode;
		// Camara godmode
		GodCamera mGodCamera;
		// Vehiculo 
		//Vehicle mVehicle;
		// Skybox
		Skybox mSkybox;
		// Weapon muzzle y distintas texturas del HUD
		cResourceHandle mWeaponMuzzle1, mWeaponMuzzle2, mWeaponMuzzle3, 
			mArrowEnemy, mPositiveAmmunition, mNegativeAmmunition,
			mPositivelive, mNegativelive, mExplosion_sprite, 
			mExplosion_sprite1, mParticle, mhud1, mhud1_mask;
		// Contador para efecto de niebla variable
		int miFogStep;
		float mfFogDensity;
		bool mbFogIncreasing;

		// State de la barra de progreso para la carga de recursos del juego
		int State;

public:
	
	//Función para inicializar el juego	
	bool Init();

	//Función para actualizar el juego
	void Update( float lfTimestep );

	//Función para renderizar el juego
	void Render();

	//Función para finalizar el juego
	bool Deinit();

	//Función que devuelve el valor de mbFinish, para chequear el final del juego.
	inline bool HasFinished() { return mbFinish; }

	//Función para cargar los recursos necesarios para el juego.
	bool LoadResources( void );

	bool LoadWindowProperties( void );
	bool Load3DCameraProperties( void );
	bool Load2DCameraProperties( void );
	bool LoadIAProperties( void );
	bool LoadManagerProperties( void );
	bool LoadVehicles( void );
	bool LoadHUD( void );
	bool LoadEffects( void );
	bool LoadObstacles( void );
	bool LoadObjects( void );
	bool LoadWorld( void );

	// Time since application start
	inline float GetAcumulatedTime() { return mfAcTime; }	
	
	// Get camera object
	cCamera Get3DCamera(){ return m3DCamera; };
	// Get camera object
	cCamera Get2DCamera(){ return m2DCamera; };
	
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

	Mustang GetMustang() { return mMustang; }
	Truck GetTruck() { return mTruck; }
	
	unsigned int GetGameWidth() { return mProperties.muiWidth; }
	unsigned int GetGameHeight() { return mProperties.muiHeight; }

	// Renderiza la barra de progreso de carga
	void RenderProgressBar( float progress);
	void RenderTitleLoad ( float progress );
	void RenderProgress( float progress);

	// Inicializa si esta dañado o no los vehiculos
	void InitDamageStates();

};

#endif