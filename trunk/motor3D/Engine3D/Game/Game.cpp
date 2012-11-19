#include "Game.h"
#include "..\Window\Window.h"
#include "..\DebugClass\Debug.h"
#include "..\Graphics\GraphicManager.h"
#include "..\Input\InputConfiguration.h"
#include "..\Input\InputManager.h"
#include "..\Graphics\Textures\TextureManager.h" 
#include "..\Graphics\Meshes\MeshManager.h"
#include "..\Gameplay\Scene\Scene.h"
#include "..\Gameplay\Scene\SceneManager.h"
#include "..\Graphics\Materials\MaterialManager.h"
#include "..\Graphics\Effects\EffectManager.h"
#include "..\Graphics\Skeletal\cSkeletalManager.h"
#include "..\Graphics\Skeletal\cSkeletalMesh.h"
#include "..\Character\CharacterManager.h"
#include "..\Character\Behaviour\BehaviourManager.h"
#include "..\Lua\LuaFunctions.h"
#include "..\LuaManager\cLuaManager.h"
#include "..\Physics\cPhysicsDebugDraw.h"

/*#include "..\Gameplay\Terrain\texture.cpp"
#include "..\Gameplay\Terrain\Map.h"
#include "..\Gameplay\Terrain\MapTexture.h"
#include "..\Gameplay\Terrain\Entity.h"
#include "..\Gameplay\Terrain\World.h"*/

//Para configurar el InputManager hay que llamar a su Init (en cGame::Init)
//pasándole la tabla kaActionMapping (de InputConfiguration.cpp).
extern tActionMapping kaActionMapping[];

//Función para inicializar el juego.
bool cGame::Init()
{	
	mbFinish = false;

	bool lbResult;

	lbResult = LoadResources();

	cMatrix lScaleMatrix, lOffsetMatrix;

	// Preparamos ya la escala y el offset que usaran el resto de objetos
	lScaleMatrix.LoadScale( .02f );
	lOffsetMatrix.LoadTranslation( cVec3( 0.0f, 0.0f, 0.0f ) );
	// Inicializamos el modelo de esfera
	mSphereModel.InitSphere( 1.0f, 2.0f );
	for ( unsigned int luiIndex = 0; luiIndex < 10; ++luiIndex) {
		maSphereObjects.push_back( mModelObject );
		cPhysicObject &lSphereObject = maSphereObjects.back();
		cMatrix lTransMatrix;
		lTransMatrix.LoadTranslation( cVec3( 0.0, 4.0 + 5.0 * luiIndex, 0.5 * luiIndex ) ); // Las creamos formando una columna
		lSphereObject.SetWorldMatrix( lTransMatrix );
		lSphereObject.SetScaleMatrix( lScaleMatrix );
		lSphereObject.SetDrawOffsetMatrix( lOffsetMatrix );
		lSphereObject.CreatePhysics( &mSphereModel );
	}	

	// Play idle animation 
	//lpSkeletonMesh->PlayAnim("Idle", 1.0f, 1.0f);
	

	// Posicionamiento inicial del personaje
	/*cMatrix lRotation, lTranslation;
	lTranslation.LoadTranslation(cVec3(0.f, -1.f, 0.f));
	mObject.SetDrawOffsetMatrix( lTranslation );
	mObject.SetKinematic( );	

	CharacterPos::Get().Init(cVec3(0.f, 0.f, 15.f), C_720PI, 10, 0.5f);*/

	// Estamos en modo juego, no depuración
	mbInGame = true;
	// Se inicializa en modo rasterizacion solida
	mbRasterizationMode = true;

	mMustang.Init(&mExt, &mInt, &mMet, &mTire, &mWeaponMuzzle1, &mWeaponMuzzle2, &mWeaponMuzzle3, &mArrowEnemy, &mPositiveAmmunition, &mNegativeAmmunition, 
		&mMustangExteriorDes, &mMustangInteriorDes
		, &mExplosion_sprite, &mExplosion_sprite1, &mParticle, &mCrosshair, &mhud1, &mhud1_mask, &mPositivelive, &mNegativelive);

	// Incializacion de enemigo 1 (Truck)
	mTruck.Init(cVec3(0.f, 0.0f, -90.f), &mTruckExterior, &mTruckWea, &mTruckTire, &mWeaponMuzzle1, &mWeaponMuzzle2, &mWeaponMuzzle3, &mTruckExteriorDes, &mTruckWeaponDes, &mExplosion_sprite, &mExplosion_sprite1, &mParticle);

	// Inicializa obstaculo (ruina)

	float a = - MAP_SIZE / 2;
	float b = MAP_SIZE / 2;

	for ( unsigned int luiIndex = 0; luiIndex < RUINS_NUMBER; ++luiIndex) {
		
		float random_x = ((b - a) * ((float)rand() / RAND_MAX)) + a;
		float random_z = ((b - a) * ((float)rand() / RAND_MAX)) + a;
		float random_angle = PI * rand();

		cMatrix lmRuinPos, lmRuinRotate, lmRuinPosRot;
		lmRuinPos.LoadTranslation(cVec3(random_x, -0.5f, random_z));
		lmRuinRotate.LoadRotation(cVec3(0.f, 1.f, 0.f), random_angle);

		lmRuinPosRot = lmRuinPos * lmRuinRotate;
		mRuin.Init(&mRuinObs, lmRuinPosRot);

		maRuins.push_back( mRuin );
		
	}

	// Inicializa obstaculo (matorrales)

	for ( unsigned int luiIndex = 0; luiIndex < BUSH_NUMBER; ++luiIndex) {
		
		float random_x = ((b - a) * ((float)rand() / RAND_MAX)) + a;
		float random_z = ((b - a) * ((float)rand() / RAND_MAX)) + a;
		
		cMatrix lmBushPos;
		lmBushPos.LoadTranslation(cVec3(random_x, -3.0f, random_z));

		//lmRuinPos.LoadTranslation(cVec3(0.f, -0.5f, 0.f));
		mBush.Init(&mBushTexture, &mBushTextureMask, lmBushPos);

		maBushes.push_back( mBush );
		
	}

	GLfloat density = 0.0002f;

	//GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0}; 
	GLfloat fogColor[4] = {1.0f, 0.9f, 0.7f, 1.0f};

	glEnable (GL_FOG);

	glFogi (GL_FOG_MODE, GL_EXP2);


	glFogfv (GL_FOG_COLOR, fogColor);

	glFogf (GL_FOG_DENSITY, density);

	glHint (GL_FOG_HINT, GL_NICEST);

	// Contador de animacion niebla
	miFogStep = 0;
	mfFogDensity = density;
	mbFogIncreasing = true;

	// Application start time
	mfAcTime = 0.0f;

    return lbResult;	
}

//Función para actualizar el juego.
void cGame::Update( float lfTimestep )
{ 			
	//Se actualiza la ventana:
	cWindow::Get().Update();

	// Updates application duration time
	mfAcTime += lfTimestep;

	//Se actualiza el InputManager.
	cInputManager::Get().Update(lfTimestep);

	// Checks if the effect has to be reloaded
	bool lbreloadEffect = IsPressed(eIA_ReloadEffectManager);
	if (lbreloadEffect) {
		cEffectManager::Get().Reload();
	}

	// Se actualiza niebla
	miFogStep ++;

	if (miFogStep == 1000) {
		if (mbFogIncreasing) {
			if (mfFogDensity >= 0.0005f)
				mbFogIncreasing = false;
			else
				mfFogDensity += 0.00001f;
		} else {
			if (mfFogDensity <= 0.0002f)
				mbFogIncreasing = true;
			else
				mfFogDensity -= 0.00001f;
		}

		miFogStep %= 1000;
	}
	glFogf (GL_FOG_DENSITY, mfFogDensity);

	// Recoge input teclado
	bool lbmoveFront = IsPressed( eIA_MoveFront );
	bool lbmoveBack = IsPressed( eIA_MoveBack );
	bool lbmoveLeft = IsPressed( eIA_MoveLeft );
	bool lbmoveRight = IsPressed( eIA_MoveRight );	
	bool lbswitchCamera = BecomePressed( eIA_SwitchCamera );
	bool lbswitchRasterizationMode = BecomePressed( eIA_SwitchFillLineMode );

	// Switch entre camara de juego/godmode
	if (lbswitchCamera)
		mbInGame = !mbInGame;
	// Switch entre el modo de rasterizacion solido/wireframe
	if (lbswitchRasterizationMode)
		mbRasterizationMode = !mbRasterizationMode;

	// Actualiza el flag de movimiento del personaje
	CharacterPos::Get().ResetChanged();
	// Calcula velocidad * tiempo transcurrido
	CharacterPos::Get().Update(lfTimestep);

	// Rotaciones del ratón
	float lfYaw = GetValue( eIA_MouseYaw );
	float lfPitch = GetValue( eIA_MousePitch );

	bool lbAuxCamera = IsPressed( eIA_SwitchCameraAux );

	// En modo juego, actualiza jugador
	if (mbInGame){
		mGodCamera.ResetYawPitch();
		if ( lbmoveFront && mMustang.IsAlive() ) {
			//CharacterPos::Get().MoveFront();
			//mVehicle.MoveForward(lfTimestep);
			mMustang.MoveForward(lfTimestep);
		}
		else if ( lbmoveBack && mMustang.IsAlive() ){
			//CharacterPos::Get().MoveBack();
			//mVehicle.Break(lfTimestep);
			mMustang.Break(lfTimestep);	
		}
		if ( lbmoveLeft && mMustang.IsAlive() ){
			//CharacterPos::Get().TurnLeft();
			//mVehicle.SteeringLeft(lfTimestep);
			mMustang.SteeringLeft(lfTimestep);
		}
		else if ( lbmoveRight && mMustang.IsAlive() ){
			//CharacterPos::Get().TurnRight();
			//mVehicle.SteeringRight(lfTimestep);
			mMustang.SteeringRight(lfTimestep);
		}

		// Actualizacion de cámara de juego
		// Si se pulsa la tecla de camara auxiliar se cambia la orientacion de la camara
		float lfDistance, lfYOffset;
		cVec3 lvYViewOffset;
		if (lbAuxCamera) {
			lfDistance = -4.f;	
			lfYOffset = 2.8f;
			lvYViewOffset = cVec3(0.f, 3.f, 0.f);
		} else {
			lfDistance = 8.f;
			lfYOffset = 3.3f;
			lvYViewOffset = cVec3(0.f, 0.0f, 0.f);
		}

		//cVec3 vVector = CharacterPos::Get().GetCharacterPosition() - CharacterPos::Get().GetFront() * lfDistance;

		/*m3DCamera.SetLookAt( cVec3(vVector.x,
								   vVector.y + 1.5f,
								   vVector.z),
								   CharacterPos::Get().GetCharacterPosition(), 
								   cVec3(0.0f, 1.f, 0.f) );*/
		//cVec3 vVector = mVehicle.GetChasisPos() - mVehicle.GetChasisRot() * lfDistance;
		cVec3 vVector = mMustang.GetVehicleBullet()->GetChasisPos() - mMustang.GetVehicleBullet()->GetChasisRot()  * lfDistance;

		m3DCamera.SetLookAt( cVec3(vVector.x,
								   vVector.y + lfYOffset,
								   vVector.z),
								   mMustang.GetVehicleBullet()->GetChasisPos() + lvYViewOffset, 
								   cVec3(0.0f, 1.f, 0.f) );

		bool lbFireMainWeapon = IsPressed( eIA_Fire );

		// Si no esta vivo el jugador entonces no puede disparar
		(!mMustang.IsAlive())?lbFireMainWeapon = false : lbFireMainWeapon = lbFireMainWeapon; 

		// Resetea estados de daño
		InitDamageStates();

		mMustang.Update(lfTimestep, lfYaw, lfPitch, lbAuxCamera, lbFireMainWeapon);
		mTruck.Update(lfTimestep);

	// Modo Godmode	
	} else {
		// Actualizacion camara godmode
		if ( lbmoveFront ) {
			mGodCamera.MoveFront(lfTimestep);				
		}
		else if ( lbmoveBack ){
			mGodCamera.MoveBack(lfTimestep);
		}
		if ( lbmoveLeft ){
			mGodCamera.MoveLeft(lfTimestep);
		}
		else if ( lbmoveRight ){
			mGodCamera.MoveRight(lfTimestep);
		}
		if (lfYaw || lfPitch){
			mGodCamera.MoveYawPitch(lfYaw, lfPitch, lfTimestep);
		}

		// Resetea estados de daño
		InitDamageStates();

		mMustang.Update(lfTimestep, lfYaw, lfPitch);
		mTruck.Update(lfTimestep);
	}

	//mObject.SetPosition(CharacterPos::Get().GetCharacterPosition(), CharacterPos::Get().GetYaw());

	// Actualiza personaje
	//mObject.Update(lfTimestep);

	// Update bullet physics object
	cPhysics::Get().Update(lfTimestep);

	//Se actualizan los personajes:
	cCharacterManager::Get().Update(lfTimestep);

//	((cScene *)mScene.GetResource())->Update(lfTimestep);

	// Update physic objects
	for ( unsigned int luiIndex = 0; luiIndex < 10; ++luiIndex) {
		maSphereObjects[luiIndex].Update(lfTimestep);
	}

	// Ruinas
	for ( unsigned int luiIndex = 0; luiIndex < RUINS_NUMBER; ++luiIndex) {
		maRuins[luiIndex].Update(lfTimestep);
	}

	//((cScene *)mMusExt.GetResource())->Update(lfTimestep);
	((cScene *)mMusNeu.GetResource())->Update(lfTimestep);

	// Check if the animation keys (stop/start) are pressed
	cSkeletalMesh* lpSkeletonMesh =(cSkeletalMesh*)mSkeletalMesh.GetResource();

	/*static bool mbJogging = false;

	bool lbPlayJogPressed = BecomePressed(eIA_PlayJog);
	bool lbStopJogPressed = BecomePressed(eIA_StopJog);
	bool lbPlayWavePressed = BecomePressed(eIA_PlayWave);
	bool lbStopWavePressed = BecomePressed(eIA_StopWave);

	// Jog animation
	if (lbPlayJogPressed && !mbJogging){
		mbJogging = true;
		lpSkeletonMesh->PlayAnim("Jog", 1.0f, 0.1f);
		lpSkeletonMesh->StopAnim("Idle", 0.1f);
	}else if (lbStopJogPressed && mbJogging){
		mbJogging = false;
		lpSkeletonMesh->PlayAnim("Idle", 1.0f, 0.1f);
		lpSkeletonMesh->StopAnim("Jog", 0.1f);
	}

	// Wave animation
	if (lbPlayWavePressed){
		lpSkeletonMesh->PlayAnim("Wave", 1.0f, 0.1f, 0.1f);
	}else if (lbStopWavePressed){
		lpSkeletonMesh->StopAnim("Wave", 0.1f);
	}*/
	
	//Se comprueba si hay que cerrar la aplicación, por ejemplo a causa de 
	// que el usuario haya cerrado la ventana. 
	mbFinish = mbFinish || cWindow::Get().GetCloseApplication()
		         //También se verifica si se ha producido la acción de entrada que cierra la aplicación:
				 // Pulsar la tecla ESC del teclado, el botón 1 del JoyStick o el botón central del ratón.
	             || IsPressed(eIA_CloseApplication);
	//Si es así, se cambia el booleano
	// que controla el cierre de la aplicación.

	if ( mbFinish )
	{
		return;
	}	
}

void cGame::InitDamageStates(){
	mMustang.GettingDamage(false);
	mTruck.GettingDamage(false);
}

//Función para renderizar el juego:
void cGame::Render()
{
	/* Orden de las instrucciones
	•    Limpiado de Buffers
	•    Activación de Cámara 3D
	•    Renderizado de Geometría 3D sólida and render of the skeleton mesh
	•    Render of bullet debug info
	•    Renderizado de Geometría 3D con transparencia
	•    Activación de Cámara 2D
	•    Renderizado de Cámara 2D
	•    Effectos de postprocesado
	•    Intercambio de los bufferes
	*/

	// 1) Limpiado de Buffers
	// -------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
	 
	// 2) Activación de Cámara 3D
	// -------------------------------------------------------------
	if (mbInGame)
		cGraphicManager::Get().ActivateCamera( &m3DCamera );
	else	
		cGraphicManager::Get().ActivateCamera( &mGodCamera );

	// Modo de rasterizacion solida/wireframe
	if (mbRasterizationMode){
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		cPhysicsDebugDraw::Get( ).setDebugMode( cPhysicsDebugDraw::DBG_NoDebug );
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cPhysicsDebugDraw::Get( ).setDebugMode( cPhysicsDebugDraw::DBG_DrawWireframe );
	}

	// 3) Renderizado de Geometría 3D sólida
	// -------------------------------------------------------------
	// Set the world matrix
	cMatrix lWorld;
	lWorld.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	// 3.0) Pinta el skybox
	mSkybox.Render();

	// 3.1) Display the terrain mesh.
	mHeightmap.Render();
//	mEntity.EntityRender();

	lWorld.LoadTranslation(cVec3(0.f, 0.f, 0.f));
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	// Render the debug lines
	//cGraphicManager::Get().DrawGrid();
	//cGraphicManager::Get().DrawAxis();

	lWorld.LoadIdentity();	
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	// 3.2) Se dibujan los personajes:
	//cCharacterManager::Get().Render();

	// 3.3) Draws debug info of bullet
	cPhysics::Get().Render();

	// Render physic objects
	for ( unsigned int luiIndex = 0; luiIndex < 10; ++luiIndex ) {
		maSphereObjects[luiIndex].Render();	
	}	

	// Ruinas
	for ( unsigned int luiIndex = 0; luiIndex < RUINS_NUMBER; ++luiIndex ) {
		maRuins[luiIndex].Render();	
	}	

	// Matorrales
	for ( unsigned int luiIndex = 0; luiIndex < BUSH_NUMBER; ++luiIndex ) {
		maBushes[luiIndex].Render();	
	}	

	mTruck.Render();

	mMustang.Render();

	// 3.4) Render of the skeleton mesh
	// -------------------------------------------------------------
	cMatrix lRotation, lCurrPos;
	lCurrPos.LoadIdentity();
	lRotation.LoadRotation(cVec3(0.f, 1.f, 0.f), CharacterPos::Get().GetYaw());
	lCurrPos.SetPosition(CharacterPos::Get().GetCharacterPosition());

	cGraphicManager::Get().SetWorldMatrix(lRotation * lCurrPos);

//	mObject.Render();

	lWorld.LoadIdentity();	
	cGraphicManager::Get().SetWorldMatrix(lWorld);
	//cSkeletalMesh* lpSkeletonMesh = (cSkeletalMesh*)mSkeletalMesh.GetResource();
	//lpSkeletonMesh->RenderSkeleton();	

	 
	// 4) Renderizado de Geometría 3D con transparencia
	// -------------------------------------------------------------

	// 5) Activación de Cámara 2D  
	// -------------------------------------------------------------
	cGraphicManager::Get().ActivateCamera( &m2DCamera );
	 
	// 6) Renderizado de Cámara 2D
	// -------------------------------------------------------------
	// Se dibuja la flecha que persigue los enemigos
	mMustang.RenderArrowEnemy();
	// Se renderiza el HUD
	mMustang.RenderHUD();
	// Se renderiza la mirilla
	mMustang.Crosshair();

	// Correccion sobre el blending para que se vea bien las letras del HUD
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//Se dibujan algunas cadenas de texto.
	/*glEnable(GL_TEXTURE_2D);
	mFont.SetColour( 1.0f, 0.0f, 0.0f, 0.9f );
	char string_lives[251];
	sprintf(string_lives, "Lives: %d", mMustang.GetCurrentLives());
	//mFont.Write(mProperties.muiWidth - 300, mProperties.muiHeight - 40, 0, string_lives, 0, FONT_ALIGN_RIGHT);
	if (mMustang.IsAlive())
		mFont.Write(500, -280, 0, string_lives, 0, FONT_ALIGN_RIGHT); 
	else 	
		mFont.Write(470, -280, 0, "You are death!", 0, FONT_ALIGN_RIGHT);*/

	//mFont.SetColour( 0.0f, 1.0f, 1.0f, 0.9f );
	//mFont.WriteBox(100, 100, 0, 100, "Esto es un test \nmultilinea", 0, FONT_ALIGN_CENTER);
	 
	// 7) Efectos de postprocesado
	// -------------------------------------------------------------
	 
	// 8) Intercambio de los bufferes
	// -------------------------------------------------------------
	cGraphicManager::Get().SwapBuffer();
}

bool cGame::LoadWindowProperties( void ) {

	//Se rellena la estructura de tipo cApplicationProperties: 
	mProperties.macApplicationName = "Mad Drive";
	mProperties.mbFullscreen = false;
	mProperties.muiBits = 16;
	mProperties.muiWidth = 1024;	
	mProperties.muiHeight = 768;

	//Se inicializa el objeto ventana con la propiedades establecidas:
    bool lbResult = cWindow::Get().Init( mProperties );

	return lbResult;
}

bool cGame::Load3DCameraProperties( void ) {

	// Init Camera 3D
	m3DCamera.Init();  
	// Inicializa camara godmode
	mGodCamera.Init();
	float lfAspect = (float)mProperties.muiWidth/(float)mProperties.muiHeight;
	m3DCamera.SetPerspective(45.0f, lfAspect,0.1f, 5000.0f);
	mGodCamera.SetPerspective( 45.0f, lfAspect, 0.1f, 10000.0f );

	//Se aleja la cámara para ver bien la escena que vamos a cargar posteriormente.
	m3DCamera.SetLookAt( cVec3(0.f, 1.5f, 20.f), cVec3(0.f, 1.5f, 0.f), cVec3(0.0f, 1.f, 0.f) );

	// Resetea color de fondo
	glClearColor(1.0f, 0.9f, 0.7f, 1.0f);
	// Ocultar geometria no mostrada
	glEnable(GL_CULL_FACE);

	return true;

}

bool cGame::Load2DCameraProperties( void ) {

	//Init Camera 2D, para las cadenas de texto.
	//Se inicializa la cámara 2D con una perspectiva ortogonal.
	//El (0,0) está situado en el centro de la pantalla y las dimensiones están 
	// establecidas en píxeles.
	float lfRight = (float)mProperties.muiWidth / 2.0f;
	float lfLeft = -lfRight;
	float lfTop = (float)mProperties.muiHeight / 2.0f;
	float lfBottom = -lfTop;
	m2DCamera.Init();
	m2DCamera.SetOrtho(lfLeft, lfRight, lfBottom, lfTop, 0.1f, 100.0f);
	//Se pone la cámara2D en la posición (0,0) de nuestro mundo,
	// apuntando al origen del mismo, e indicando que el vector UP de la cámara apunta hacia arriba.
	m2DCamera.SetLookAt( cVec3(0.0f, 0.0f, 1.f), cVec3(0.0f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );

	return true;

}

bool cGame::LoadIAProperties( void ) {
	//Se inicializa la clase que contendrá la lista de personajes.
	bool lbResult = cCharacterManager::Get().Init();
	//Se inicializa la clase que contendrá los comportamientos de los personajes.
	lbResult = lbResult && cBehaviourManager::Get().Init();
	if (lbResult)
	{
		//Se inicializa Lua.
		cLuaManager::Get().Init();
		//Se registran las funciones C++ para usar desde Lua.
		RegisterLuaFunctions();
		//Cargamos el fichero de script
		cLuaManager::Get().DoFile("Data/Scripts/character.lua");
		cCharacterManager::Get().DebugCharacter();
		cLuaManager::Get().DoFile("Data/Scripts/patrol.lua");
	}

	return lbResult;
}

bool cGame::LoadManagerProperties( void ) {

	//Se inicializa la clase cInputManager que representa el gestor de entrada (keyboard, gamepad, mouse, ...).
	//Se le pasa la tabla "kaActionMapping" (de InputConfiguration.cpp) que indica la relación entre las acciones y los dispositivos.
	//También se pasa "eIA_Count" (de InputConfiguration.h) que indica cuantas acciones de entrada hay.
	cInputManager::Get().Init( kaActionMapping, eIA_Count );

	// Initialization of physics object 
	cPhysics::Get().Init();

	//Se inicializa la clase que gestiona los materiales.
	cMaterialManager::Get().Init(30);

	// Init of effects management
	cEffectManager::Get().Init(20);

	// Init the Font.
	//mFont.Init("./Data/Fonts/Test1.fnt");
	mFont.Init("./Data/Fonts/font2.fnt");

	//Se inicializa el gestor de mallas: habrá 4 mallas en la escena (mirar ./Data/Scene/) + Skeletal mesh.
	cMeshManager::Get().Init(30);

	//Se inicializa el gestor de escenas.
	cSceneManager::Get().Init(30);  

	cSkeletalManager::Get().Init(5);

	return true;

}

bool cGame::LoadVehicles( void ) {
			
	mMusExt = cSceneManager::Get().LoadResource( "Mustang_exterior", "./Data/Scene/mustang_exterior1.dae" ); 			
	mMusInt = cSceneManager::Get().LoadResource( "Mustang_interior", "./Data/Scene/mustang_interior.dae" ); 
	mMusMet = cSceneManager::Get().LoadResource( "Mustang_metralleta", "./Data/Scene/mustang_metralleta.dae" ); 
	mMustangExtDes = cSceneManager::Get().LoadResource( "Mustang_exterior_des", "./Data/Scene/mustang_destruido_exterior.dae" ); 			
	mMustangIntDes = cSceneManager::Get().LoadResource( "Mustang_interior_des", "./Data/Scene/mustang_destruido_interior.dae" ); 
	mMusNeu = cSceneManager::Get().LoadResource( "Mustang_neumatico", "./Data/Scene/mustang_neumatico.dae" ); 
	
	mTruckNeu = cSceneManager::Get().LoadResource( "Truck_neumatico", "./Data/Scene/enemigo1_rueda.dae" );
	mTruckArm = cSceneManager::Get().LoadResource( "Truck_arma", "./Data/Scene/enemigo1_arma.dae" );
	mTruckExt = cSceneManager::Get().LoadResource( "Truck_exterior", "./Data/Scene/enemigo1_exterior.dae" );
	mTruckExtDes = cSceneManager::Get().LoadResource( "Truck_exterior_destruido", "./Data/Scene/enemigo1_destruido_exterior.dae" ); 			
	mTruckWeaDes = cSceneManager::Get().LoadResource( "Truck_arma_destruido", "./Data/Scene/enemigo1_destruido_arma.dae" ); 

	return true;

}

bool cGame::LoadHUD( void ) {

	mWeaponMuzzle1 = cTextureManager::Get().LoadResource( "Weapon_muzzle1", "Data/Scene/images/flash_disparo/muzzle_flash_1.jpg" ); 
	mWeaponMuzzle2 = cTextureManager::Get().LoadResource( "Weapon_muzzle2", "Data/Scene/images/flash_disparo/muzzle_flash_2.jpg" );
	mWeaponMuzzle3 = cTextureManager::Get().LoadResource( "Weapon_muzzle3", "Data/Scene/images/flash_disparo/muzzle_flash_3.jpg" );
	mArrowEnemy = cTextureManager::Get().LoadResource( "Arrow_enemy", "Data/Scene/images/hud/enemy_arrow.jpg" );
	mPositiveAmmunition = cTextureManager::Get().LoadResource( "Positive_ammunition", "Data/Scene/images/hud/icono_municion_positiva_hud.png" );
	mNegativeAmmunition = cTextureManager::Get().LoadResource( "Negative_ammunition", "Data/Scene/images/hud/icono_municion_negativa_hud.png" );
	mPositivelive = cTextureManager::Get().LoadResource( "Positive_live", "Data/Scene/images/hud/icono_vida_positiva_hud.png" );
	mNegativelive = cTextureManager::Get().LoadResource( "Negative_live", "Data/Scene/images/hud/icono_vida_negativa_hud.png" );
	mCrosshair = cTextureManager::Get().LoadResource( "Mirilla", "./Data/Scene/images/mirilla/crosshair.png" );
	mItemCongelacion = cSceneManager::Get().LoadResource( "Item_congelacion", "./Data/Scene/especial_congelacion.dae" );
	mhud1 = cTextureManager::Get().LoadResource( "Hud1", "Data/Scene/images/hud/hud_mad_drive1.png" );
	mhud1_mask = cTextureManager::Get().LoadResource( "Hud1_mask", "Data/Scene/images/hud/hud_mad_drive1_mask.png" );

	return true;

}

bool cGame::LoadEffects( void ) {

	mExplosion_sprite = cTextureManager::Get().LoadResource( "Explosion_sprite", "./Data/Scene/images/explosion/explosion_sprite1.tga" ); 
	mExplosion_sprite1 = cTextureManager::Get().LoadResource( "Explosion_sprite1", "Data/Scene/images/explosion/texture_explosion1.tga" ); 
	mParticle = cTextureManager::Get().LoadResource( "Particle_system", "Data/Scene/images/sist_particulas/fog_text.png" );
	
	return true;

}

bool cGame::LoadObstacles( void ) {

	mRuinModel = cSceneManager::Get().LoadResource( "Ruina", "./Data/Scene/mad_drive_escombros_ruina.dae" ); 
	mBushTexture = cTextureManager::Get().LoadResource( "Matorral", "./Data/Scene/images/matorral/matorral_1.png" );
	mBushTextureMask = cTextureManager::Get().LoadResource( "Matorral_mask", "./Data/Scene/images/matorral/matorral_mask.png" );
			
	return true;

}

bool cGame::LoadWorld( void ) {

	// Inicializa skybox
	mSkybox.Init();

	// Terrain object
	mHeightmap.Load();

	return true;

}

bool cGame::LoadObjects( void ) {

	// Physics object in the game
	mModelObject = *((cPhysicObject*) ((cScene *)mItemCongelacion.GetResource())->getSubObject( 0 ));

	// Inicializacion del vehiculo (Mustang)
	mTire = *((cObject*) ((cScene *)mMusNeu.GetResource())->getSubObject(0));
	mExt = *((cObject*) ((cScene *)mMusExt.GetResource())->getSubObject(0));
	mInt = *((cObject*) ((cScene *)mMusInt.GetResource())->getSubObject(0));
	mMet = *((cObject*) ((cScene *)mMusMet.GetResource())->getSubObject(0));

	mMustangExteriorDes = *((cObject*) ((cScene *)mMustangExtDes.GetResource())->getSubObject(0));
	mMustangInteriorDes = *((cObject*) ((cScene *)mMustangIntDes.GetResource())->getSubObject(0));

	mTruckExterior = *((cObject*) ((cScene *)mTruckExt.GetResource())->getSubObject(0));
	mTruckWea = *((cObject*) ((cScene *)mTruckArm.GetResource())->getSubObject(0));
	mTruckTire = *((cObject*) ((cScene *)mTruckNeu.GetResource())->getSubObject(0));
	mTruckExteriorDes = *((cObject*) ((cScene *)mTruckExtDes.GetResource())->getSubObject(0));
	mTruckWeaponDes = *((cObject*) ((cScene *)mTruckWeaDes.GetResource())->getSubObject(0));

	mRuinObs = *((cObject*) ((cScene *)mRuinModel.GetResource())->getSubObject(0));

	return true;

}

void cGame::RenderProgressBar ( float progress ) 
{

	cMatrix lmTranslation;
	lmTranslation.LoadTranslation(cVec3(-300.f, 10.f, 0.f));

	cGraphicManager::Get().SetWorldMatrix(lmTranslation);
	
	glDisable(GL_TEXTURE_2D);
		cGraphicManager::Get().DrawRect(0.f, 0.f, 600.f * progress, 2.f, cVec3(progress, 0.f, 0.f));
		//cGraphicManager::Get().DrawRect(0.f, 0.f, 200.f, 2.f, cVec3(1.f, 0.f, 0.f)); 
	glEnable(GL_TEXTURE_2D);

	cGraphicManager::Get().SetWorldMatrix(lmTranslation.LoadIdentity());

}

void cGame::RenderTitleLoad ( float progress ) 
{

	// Titulo MAD DRIVE

	cMatrix lmTranslation;
	lmTranslation.LoadTranslation(cVec3(0.f, 220.f, 0.f));

	cGraphicManager::Get().SetWorldMatrix(lmTranslation);
	
	cTexture* lpTexture;
	if (progress == 1.f)
		lpTexture = (cTexture*) mMad_drive.GetResource();
	else
		lpTexture = (cTexture*) mMad_drive_loading.GetResource();
	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					// scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	// scale linearly when image smalled than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBegin(GL_QUADS);

		 glTexCoord2f(0.0f, 1.0f); glVertex3f(-90, -50, 0);
		 glTexCoord2f(1.0f, 1.0f); glVertex3f(90, -50, 0);
		 glTexCoord2f(1.0f, 0.0f); glVertex3f(90, 50, 0);
		 glTexCoord2f(0.0f, 0.0f); glVertex3f(-90, 50, 0);

	glEnd();

	cGraphicManager::Get().SetWorldMatrix(lmTranslation.LoadIdentity());

	// LOADING

	lmTranslation.LoadTranslation(cVec3(0.f, 70.f, 0.f));

	cGraphicManager::Get().SetWorldMatrix(lmTranslation);
	
	lpTexture = (cTexture*) mLoading.GetResource();
	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					// scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	// scale linearly when image smalled than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBegin(GL_QUADS);

		 glTexCoord2f(0.0f, 1.0f); glVertex3f(-90, -50, 0);
		 glTexCoord2f(1.0f, 1.0f); glVertex3f(90, -50, 0);
		 glTexCoord2f(1.0f, 0.0f); glVertex3f(90, 50, 0);
		 glTexCoord2f(0.0f, 0.0f); glVertex3f(-90, 50, 0);

	glEnd();

	cGraphicManager::Get().SetWorldMatrix(lmTranslation.LoadIdentity());

}

void cGame::RenderProgress ( float progress ) 
{

	RenderTitleLoad(progress);
	RenderProgressBar(progress);
	
	cGraphicManager::Get().SwapBuffer();

}

//Función para cargar los recursos necesarios para el juego.
bool cGame::LoadResources( void )
{

	bool lbResult;

	lbResult = LoadWindowProperties();

	if ( lbResult )
	{
		//Se inicializa la clase que encapsula las operaciones de OpenGL:
		lbResult = cGraphicManager::Get().Init( &cWindow::Get() );   

		// Clean Window context
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
		// Resetea color de fondo
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		if (lbResult)
		{
			//Se inicializa la clase que gestiona la texturas indicando que habrá 1, por ejemplo.
			cTextureManager::Get().Init(60);

			mMad_drive_loading = cTextureManager::Get().LoadResource( "Mad_drive_loading", "Data/Scene/images/hud/mad_drive_loading.png" );
			mMad_drive = cTextureManager::Get().LoadResource( "Mad_drive", "Data/Scene/images/hud/mad_drive.png" );
			mLoading = cTextureManager::Get().LoadResource( "Loading", "Data/Scene/images/hud/loading.png" );

			Load2DCameraProperties();

			cGraphicManager::Get().ActivateCamera( &m2DCamera );

			RenderProgress(0.1f);

			Load3DCameraProperties();

			RenderProgress(0.2f);

			LoadIAProperties();

			LoadManagerProperties();

			RenderProgress(0.3f);

			//Se carga la escena.
			//mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/dragonsmall.DAE" ); 		
			//*mDuck = cSceneManager::Get().LoadResource( "Duck", "./Data/Scene/duck_triangulate.dae" ); 
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/mad_drive_escombros_cactus.dae" ); 
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/mad_drive_escombros_carretera.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/mad_drive_escombros_matorral.dae" ); 
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/mad_drive_escombros_palmera.dae" ); 
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/mad_drive_escombros_tronco.dae" ); 

			LoadVehicles();

			RenderProgress(0.5f);

			LoadHUD();

			RenderProgress(0.6f);

			LoadEffects();

			RenderProgress(0.7f);

			LoadObstacles();

			RenderProgress(0.85f);

//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/enemigo2_exterior.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/enemigo2.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/enemigo2_arma.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/enemigo2_rueda.dae" );
//		    mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/mad_drive_escombros_ruina.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/combustible.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/especial_invencibilidad.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/especial_superturbo.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/municion.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/enemigo1_destruido_arma.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/enemigo1_destruido_exterior.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/enemigo2_destruido_arma.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/enemigo2_destruido_exterior.dae" );
//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/mustang_destruido_exterior.dae" );
			//mScene1 = cSceneManager::Get().LoadResource( "TestLevel1", "./Data/Scene/mustang_destruido_interior.dae" );
			
			// Loads skeleton 
		//	cResourceHandle mustang_mesh = cMeshManager::Get().LoadResource("testlevel1", "./Data/Scene/mustang_destruido_interior.dae");

			/*if (!((cScene *)mScene.GetResource())->Init("TestLevel1", "./Data/Scene/mustang_destruido_interior.dae")){
				OutputDebugStr("Error loading object!");
			}*/

//			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/plane.DAE" );

			//cPhysicObject mModelObject = *((cPhysicObject*) ((cScene *)mScene.GetResource())->getSubObject( 1 ));


			// Inits skeleton model
			//cSkeletalManager::Get().LoadResource("Skeleton", "./Data/Skeletal/SkeletonModel.xml");
		
			// Loads skeleton 
			//mSkeletalMesh = cMeshManager::Get().LoadResource("Skeleton", "Skeleton", kuiSkeletalMesh);

			// Get skeleton mesh
			//cSkeletalMesh* lpSkeletonMesh=(cSkeletalMesh*)mSkeletalMesh.GetResource();

			// Load Skeleton meshes
			//cResourceHandle lMaterial = cMaterialManager::Get().LoadResource("Skeleton", "./Data/Material/SkeletonMaterial.xml");
		
			//assert(lMaterial.IsValidHandle());
			//mObject.AddMesh(mSkeletalMesh, lMaterial);	
		
			// Bounding box del personaje
			/*mBoxModel.InitBox( 0.0f, cVec3( 0.3f, 1.0f, 0.3f ) );
			mObject.CreatePhysics( &mBoxModel );
			lScaleMatrix.LoadScale(0.01f);
			mObject.SetScaleMatrix( lScaleMatrix );*/

			LoadObjects();

			RenderProgress(1.0f);
			
			LoadWorld();

		} else {
			//Si algo falla se libera la ventana.
			cWindow::Get().Deinit();
		}
	}

   return lbResult;
}

//Función para finalizar el juego.
bool cGame::Deinit()
{
	//Se deinicializa en el orden inverso a la inicialización:

	mMustang.Deinit();
	mTruck.Deinit();

	cMaterialManager::Get().Deinit();

	//Se libera el manejador de escenas.
	cSceneManager::Get().Deinit();

	//Se libera el manejador de mallas.
	cMeshManager::Get().Deinit();

	//Deinicializamos Lua
	cLuaManager::Get().Deinit();
	//Se libera la clase que contiene la lista de personajes:
	bool lbResult =  cCharacterManager::Get().Deinit();
	//Se libera la clase que contiene la lista de comportamientos:
	lbResult = lbResult && cBehaviourManager::Get().Deinit();	

	//Se libera la clase fuente.
	mFont.Deinit();

    //Se libera el gestor de texturas.
	cTextureManager::Get().Deinit();  
	// Deinitialization of terrain
	mHeightmap.Deinit();
	// Deinitialization of physics object 
	cPhysics::Get().Deinit();
	//Se libera el InputManager:
	cInputManager::Get().Deinit();
	//Se libera OpenGL (clase cGraphicManager):
	lbResult = cGraphicManager::Get().Deinit();
	//Se libera la ventana:
	lbResult = lbResult && cWindow::Get().Deinit();
	return lbResult;	
}