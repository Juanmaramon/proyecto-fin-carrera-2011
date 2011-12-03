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

//Para configurar el InputManager hay que llamar a su Init (en cGame::Init)
//pas�ndole la tabla kaActionMapping (de InputConfiguration.cpp).
extern tActionMapping kaActionMapping[];

extern const bool _DEBUG = true;

//Funci�n para inicializar el juego.
bool cGame::Init()
{	
	mbFinish = false;
	//	LoadResources();
	//Se rellena la estructura de tipo cApplicationProperties: 
	mProperties.macApplicationName = "Proyecto fin master";
	mProperties.mbFullscreen = false;
	mProperties.muiBits = 16;
	mProperties.muiWidth = 640;
	mProperties.muiHeight = 480;	
	
	//Se inicializa el objeto ventana con la propiedades establecidas:
    bool lbResult = cWindow::Get().Init( mProperties );
		
	if ( lbResult )
	{
		//Se inicializa la clase que encapsula las operaciones de OpenGL:
		lbResult = cGraphicManager::Get().Init( &cWindow::Get() );   
		if (lbResult)
		{
			// Init Camera 3D
			m3DCamera.Init();  
			float lfAspect = (float)mProperties.muiWidth/(float)mProperties.muiHeight;
			m3DCamera.SetPerspective(45.0f, lfAspect,0.1f,1000.0f);
			//Se pone la c�mara en la posici�n (5,5,5) de nuestro mundo,
			// apuntando al origen del mismo, e indicando que el vector UP de la c�mara apunta hacia arriba.
			//m3DCamera.SetLookAt( cVec3(5.0f, 5.f, 5.f), cVec3(0.0f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );		

			//Se aleja la c�mara para ver bien la escena que vamos a cargar posteriormente.
			//m3DCamera.SetLookAt( cVec3(5.f, 4.3f, 5.f),cVec3(0.f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );
			m3DCamera.SetLookAt( cVec3(0.f, 1.5f, 20.f),cVec3(0.f, 1.5f, 0.f), cVec3(0.0f, 1.f, 0.f) );

			glClearColor(1.0f, 0.9f, 0.7f, 1.0f);

			//Se inicializa la clase que contendr� la lista de personajes.
			lbResult = cCharacterManager::Get().Init();
			//Se inicializa la clase que contendr� los comportamientos de los personajes.
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

			//Se inicializa la clase cInputManager que representa el gestor de entrada (keyboard, gamepad, mouse, ...).
			//Se le pasa la tabla "kaActionMapping" (de InputConfiguration.cpp) que indica la relaci�n entre las acciones y los dispositivos.
			//Tambi�n se pasa "eIA_Count" (de InputConfiguration.h) que indica cuantas acciones de entrada hay.
			cInputManager::Get().Init( kaActionMapping, eIA_Count );

			// Initialization of physics object 
			cPhysics::Get().Init();

			//Se inicializa la clase que gestiona la texturas indicando que habr� 1, por ejemplo.
			cTextureManager::Get().Init(20);

			// Terrain object
			if (!mHeightmap.Load()) OutputDebugString("Heightmap terrain load error!");

			//Se inicializa la clase que gestiona los materiales.
			cMaterialManager::Get().Init(10);

			// Init of effects management
			cEffectManager::Get().Init(10);

			//Init Camera 2D, para las cadenas de texto.
			//Se inicializa la c�mara 2D con una perspectiva ortogonal.
			//El (0,0) est� situado en el centro de la pantalla y las dimensiones est�n 
			// establecidas en p�xeles.
			float lfRight = (float)mProperties.muiWidth / 2.0f;
			float lfLeft = -lfRight;
			float lfTop = (float)mProperties.muiHeight / 2.0f;
			float lfBottom = -lfTop;
			m2DCamera.Init();
			m2DCamera.SetOrtho(lfLeft, lfRight, lfBottom, lfTop, 0.1f, 100.0f);
			//Se pone la c�mara2D en la posici�n (0,0) de nuestro mundo,
			// apuntando al origen del mismo, e indicando que el vector UP de la c�mara apunta hacia arriba.
			m2DCamera.SetLookAt( cVec3(0.0f, 0.0f, 1.f), cVec3(0.0f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );

			// Init the Font.
			mFont.Init("./Data/Fonts/Test1.fnt");

			//Se inicializa el gestor de mallas: habr� 4 mallas en la escena (mirar ./Data/Scene/) + Skeletal mesh.
			cMeshManager::Get().Init(5);

			//Se inicializa el gestor de escenas.
			cSceneManager::Get().Init(10);   

			//Se carga la escena.
			//mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/dragonsmall.DAE" ); 		
			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/duck_triangulate.dae" ); 
			//mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/plane.DAE" );

			// Physics object in the game
			cPhysicObject mModelObject = *((cPhysicObject*) ((cScene *)mScene.GetResource())->getSubObject( 0 ));

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

			cSkeletalManager::Get().Init(5);
			// Inits skeleton model
			cSkeletalManager::Get().LoadResource("Skeleton", "./Data/Skeletal/SkeletonModel.xml");
		
			// Loads skeleton 
			mSkeletalMesh = cMeshManager::Get().LoadResource("Skeleton", "Skeleton", kuiSkeletalMesh);

			// Get skeleton mesh
			cSkeletalMesh* lpSkeletonMesh=(cSkeletalMesh*)mSkeletalMesh.GetResource();

			// Play idle animation 
			lpSkeletonMesh->PlayAnim("Idle", 1.0f, 1.0f);
			
			// Load Skeleton meshes
			cResourceHandle lMaterial = cMaterialManager::Get().LoadResource("Skeleton", "./Data/Material/SkeletonMaterial.xml");
		
			assert(lMaterial.IsValidHandle());
			mObject.AddMesh(mSkeletalMesh, lMaterial);	
		
			mBoxModel.InitBox( 0.0f, cVec3( 0.3f, 1.0f, 0.3f ) );
			mObject.CreatePhysics( &mBoxModel );
			lScaleMatrix.LoadScale(0.01f);
			mObject.SetScaleMatrix( lScaleMatrix );

			cMatrix lMatrix;
			lMatrix.LoadTranslation(cVec3(0.f, -1.f, 0.f));
			mObject.SetDrawOffsetMatrix( lMatrix );
			mObject.SetKinematic( );	

			//CharacterPos::Get().Init(cVec3(0.f, 0.f, 15.f), 329.9f, 1);
			CharacterPos::Get().Init(cVec3(0.f, 0.f, 15.f), /*3.139f*/ 0.f, 1, 0.1f);
		}		
		else
		{
			//Si algo falla se libera la ventana.
			cWindow::Get().Deinit();
		}
	}

	// Application start time
	mfAcTime = 0.0f;

    return lbResult;	
}

//Funci�n para actualizar el juego.
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

	bool lbmoveFront = IsPressed( eIA_MoveFront );
	bool lbmoveBack = IsPressed( eIA_MoveBack );
	bool lbmoveLeft = IsPressed( eIA_MoveLeft );
	bool lbmoveRight = IsPressed( eIA_MoveRight );
	
	cVec3 lCamaraPos = m3DCamera.GetPosition();

	// Actualiza el flag de movimiento del personaje
	CharacterPos::Get().ResetChanged();

	if ( lbmoveFront ) {
		CharacterPos::Get().MoveFront();
		lCamaraPos.z += CharacterPos::Get().GetVelocity();
	}
	if ( lbmoveBack ){
		CharacterPos::Get().MoveBack();
		lCamaraPos.z -= CharacterPos::Get().GetVelocity();
	}
	if ( lbmoveLeft ){
		CharacterPos::Get().StrafeLeft();
	}
	if ( lbmoveRight ){
		CharacterPos::Get().StrafeRight();
	}

	if (CharacterPos::Get().IsChanged()){
		if (_DEBUG){
			char buf[2048];
			//sprintf(buf,"Position (x,y,z), (angle) is (%2.0f%,%2.0f,%2.0f), (%2.f) \n",lTranslateOffset.rows[3].x,lTranslateOffset.rows[3].y,lTranslateOffset.rows[3].z, 
			//		CharacterPos::Get().GetYaw());
			OutputDebugString(buf);	
		}
	}

	mObject.SetPosition(CharacterPos::Get().GetCharacterPosition() + (CharacterPos::Get().GetFront() * CharacterPos::Get().GetVelocity())); 

	// Actualiza c�mara y personaje
	m3DCamera.SetView(lCamaraPos);
	mObject.Update(lfTimestep);

/*	if ( lbmoveLeft ) {
		//lCamaraPos.x -= -10.f;
		lOffsetMatrix.LoadRotation( cVec3( 0.f, 1.f, 0.f ), 0.01f );	
		// m3DCamera.SetView(lCamaraPos);
		//lTranslateOffset.LoadTranslation(cVec3(-0.1f, 0.0f, 0.0f ));
		//mObject.SetPosition( mObject.GetPosition( ) + cVec3(-1.f, 0.0f, 0.0f ) );		

		
	}else if ( lbmoveRight ) {
		//lTranslateOffset.LoadTranslation(cVec3(0.1f, 0.0f, 0.0f ));
		lOffsetMatrix.LoadRotation( cVec3( 0.f, 1.f, 0.f ), -0.01f );	
		//lCamaraPos.x -= 10.f;
		//m3DCamera.SetView(lCamaraPos);
		//mObject.SetPosition( mObject.GetPosition( ) + cVec3( 1.f, 0.0f, 0.0f ) );
	}
	
	cVec4 rows2[4];
	rows2[0] = mObject.GetDrawOffsetMatrix().rows[0];
	rows2[1] = mObject.GetDrawOffsetMatrix().rows[1];
	rows2[2] = mObject.GetDrawOffsetMatrix().rows[2];
	rows2[3] = mObject.GetDrawOffsetMatrix().rows[3];
	mObject.SetDrawOffsetMatrix(mObject.GetDrawOffsetMatrix() * (lTranslateOffset * lOffsetMatrix));
    rows2[0] = mObject.GetDrawOffsetMatrix().rows[0];
	rows2[1] = mObject.GetDrawOffsetMatrix().rows[1];
	rows2[2] = mObject.GetDrawOffsetMatrix().rows[2];
	rows2[3] = mObject.GetDrawOffsetMatrix().rows[3];

	if ( lbmoveLeft || lbmoveRight ) {
		//float fVar = (lbmoveLeft ? -0.05f : 0.05f);
		//float x = mObject.GetDrawOffsetMatrix().rows[0].x * fVar;	
		//float z = mObject.GetDrawOffsetMatrix().rows[0].z * fVar;	
		
		float fVar = 0.0f;
		if (mObject.GetDrawOffsetMatrix().rows[0].x > -1.0f && mObject.GetDrawOffsetMatrix().rows[0].x < 0.0f && mObject.GetDrawOffsetMatrix().rows[0].z > 0.0f ) {
			if (lbmoveLeft) {
				lCamaraPos.z += 0.05f;		
				lCamaraPos.x -= 0.05f;
			}
			else {
				lCamaraPos.z -= 0.05f;		
				lCamaraPos.x += 0.05f;
			}
		}
		else if (mObject.GetDrawOffsetMatrix().rows[0].x > 0.0f && mObject.GetDrawOffsetMatrix().rows[0].x < 1.0f && mObject.GetDrawOffsetMatrix().rows[0].z > 0.0f ) {
			if (lbmoveLeft) {
				lCamaraPos.z += 0.05f;		
				lCamaraPos.x += 0.05f;
			}
			else {
				lCamaraPos.z -= 0.05f;		
				lCamaraPos.x -= 0.05f;
			}			
		}
		else if (mObject.GetDrawOffsetMatrix().rows[0].x < 1.0f && mObject.GetDrawOffsetMatrix().rows[0].x > 0.0f && mObject.GetDrawOffsetMatrix().rows[0].z < 0.0f ) {
			if (lbmoveLeft) {
				lCamaraPos.z -= 0.05f;		
				lCamaraPos.x += 0.05f;
			}
			else {
				lCamaraPos.z += 0.05f;		
				lCamaraPos.x -= 0.05f;
			}			
		}
		else if (mObject.GetDrawOffsetMatrix().rows[0].x < 0.0f && mObject.GetDrawOffsetMatrix().rows[0].x > -1.0f && mObject.GetDrawOffsetMatrix().rows[0].z < 0.0f ) {		
			if (lbmoveLeft) {
				lCamaraPos.z -= 0.05f;		
				lCamaraPos.x -= 0.05f;
			}
			else {
				lCamaraPos.z += 0.05f;		
				lCamaraPos.x += 0.05f;
			}
		}	
	
		m3DCamera.SetView(lCamaraPos);

		//m3DCamera.SetLookAt( -lCamaraPos, cVec3(0.0f, 1.5f, 0.0f), cVec3(0.0f, 1.f, 0.f) );
	}    */	



	// Update bullet physics object
	cPhysics::Get().Update(lfTimestep);

	//Se actualizan los personajes:
	cCharacterManager::Get().Update(lfTimestep);

	// Update physic objects
	for ( unsigned int luiIndex = 0; luiIndex < 10; ++luiIndex) {
		maSphereObjects[luiIndex].Update(lfTimestep);
	}

	// Check if the animation keys (stop/start) are pressed
	cSkeletalMesh* lpSkeletonMesh =(cSkeletalMesh*)mSkeletalMesh.GetResource();

	static bool mbJogging = false;

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
	}

	// Updates skeleton state
	//lpSkeletonMesh->Update(lfTimestep);

	
	//Se comprueba si hay que cerrar la aplicaci�n, por ejemplo a causa de 
	// que el usuario haya cerrado la ventana. 
	mbFinish = mbFinish || cWindow::Get().GetCloseApplication()
		         //Tambi�n se verifica si se ha producido la acci�n de entrada que cierra la aplicaci�n:
				 // Pulsar la tecla ESC del teclado, el bot�n 1 del JoyStick o el bot�n central del rat�n.
	             || IsPressed(eIA_CloseApplication);
	//Si es as�, se cambia el booleano
	// que controla el cierre de la aplicaci�n.

	if ( mbFinish )
	{
		return;
	}	
}

//Funci�n para renderizar el juego:
void cGame::Render()
{
	/* Orden de las instrucciones
	�    Limpiado de Buffers
	�    Activaci�n de C�mara 3D
	�    Renderizado de Geometr�a 3D s�lida and render of the skeleton mesh
	�    Render of bullet debug info
	�    Renderizado de Geometr�a 3D con transparencia
	�    Activaci�n de C�mara 2D
	�    Renderizado de C�mara 2D
	�    Effectos de postprocesado
	�    Intercambio de los bufferes
	*/

	// 1) Limpiado de Buffers
	// -------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
	 
	// 2) Activaci�n de C�mara 3D
	// -------------------------------------------------------------
	cGraphicManager::Get().ActivateCamera( &m3DCamera );
	 
	// 3) Renderizado de Geometr�a 3D s�lida
	// -------------------------------------------------------------
	// Set the world matrix
	cMatrix lWorld;
	lWorld.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	//Desactivamos la textura porque a�n no tenemos materiales asignados a las mallas de la escena.
//	glDisable(GL_TEXTURE_2D);
	//Para renderizar la escena llamaremos al render de la escena a trav�s de mScene (manejador de la escena, de tipo cResourceHandle).
	//((cScene *)mScene.GetResource())->Render();
//	glEnable(GL_TEXTURE_2D);

	// 3.0) Display the terrain mesh.
	mHeightmap.Render();

	lWorld.LoadTranslation(cVec3(0.f, 0.f, 0.f));
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	// Render the debug lines
	cGraphicManager::Get().DrawGrid();
	cGraphicManager::Get().DrawAxis();

	cGraphicManager::Get().DrawPoint( cVec3(1.5f, 0.0f, 1.5f), cVec3(1.0f, 0.0f, 1.0f) );
	cGraphicManager::Get().DrawLine( cVec3(-1.5f, 0.0f, -1.5f), cVec3(-1.5f, 0.0f, 1.5f), cVec3(1.0f, 1.0f, 0.0f) );

	lWorld.LoadIdentity();	
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	// 3.1) Se dibujan los personajes:
	cCharacterManager::Get().Render();


	// 3.2) Draws debug info of bullet
	cPhysics::Get().Render();	


	// Render physic objects
	for ( unsigned int luiIndex = 0; luiIndex < 10; ++luiIndex) {
		maSphereObjects[luiIndex].Render();	
	}		

	// 3.3) Render of the skeleton mesh
	// -------------------------------------------------------------
	cMatrix lCurrPos, lTranslateOffset, lRotateOffset;
	lCurrPos.LoadIdentity();
	lCurrPos.LoadRotation(cVec3(0.f, 1.f, 0.f), CharacterPos::Get().GetYaw());
	lCurrPos.SetPosition(CharacterPos::Get().GetCharacterPosition());


	//mObject.SetWorldMatrix(lRotateOffset * lTranslateOffset);
	//mObject.ReloadPhysicPosition(lRotateOffset, lTranslateOffset);
	//lCurrPos = lRotateOffset * lTranslateOffset;
	cGraphicManager::Get().SetWorldMatrix(lCurrPos);

	mObject.Render();

	lWorld.LoadIdentity();	
	cGraphicManager::Get().SetWorldMatrix(lWorld);
	//cSkeletalMesh* lpSkeletonMesh = (cSkeletalMesh*)mSkeletalMesh.GetResource();
	//lpSkeletonMesh->RenderSkeleton();	

	 
	// 4) Renderizado de Geometr�a 3D con transparencia
	// -------------------------------------------------------------


	// 5) Activaci�n de C�mara 2D  
	// -------------------------------------------------------------
	cGraphicManager::Get().ActivateCamera( &m2DCamera );
	 
	// 6) Renderizado de C�mara 2D
	// -------------------------------------------------------------
	//Se dibujan algunas cadenas de texto.
	glEnable(GL_TEXTURE_2D);
	mFont.SetColour( 1.0f, 0.0f, 0.0f );
	mFont.Write(0,200,0, "A�o Tot� ping�ino() �!�?", 0, FONT_ALIGN_CENTER);
	 
	mFont.SetColour( 0.0f, 1.0f, 1.0f );
	mFont.WriteBox(100,100,0,100, "Esto es un test \nmultilinea", 0, FONT_ALIGN_CENTER);	


	 
	// 7) Efectos de postprocesado
	// -------------------------------------------------------------
	 
	// 8) Intercambio de los bufferes
	// -------------------------------------------------------------
	cGraphicManager::Get().SwapBuffer();
}

//Funci�n para cargar los recursos necesarios para el juego.
bool cGame::LoadResources( void )
{
   return true;
}

//Funci�n para finalizar el juego.
bool cGame::Deinit()
{
	//Se deinicializa en el orden inverso a la inicializaci�n:

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