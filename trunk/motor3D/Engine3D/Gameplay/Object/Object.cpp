#include "Object.h"
#include "..\..\Graphics\GraphicManager.h"
#include "..\..\Graphics\Materials\Material.h"
#include "..\..\Graphics\Meshes\Mesh.h"
#include "..\Scene\Scene.h"
#include "..\..\Input\InputManager.h"
#include "..\..\Input\InputConfiguration.h"

// Reset name, world matrix, and vectors
void cObject::Init()
{
	macName = "";
	mWorldMatrix.LoadIdentity();
	mMeshHandles.resize(0);
	mMaterialHandles.resize(0);

}

void cObject::Deinit(){ 
	macName = "";
	mWorldMatrix.LoadIdentity();
	mMeshHandles.empty();
	mMaterialHandles.empty();
};

void cObject::AddMesh( cResourceHandle lMeshHandle, cResourceHandle lMaterialHandle ) 
{
	mMeshHandles.push_back( lMeshHandle );
	mMaterialHandles.push_back( lMaterialHandle );
}

void cObject::Render()
{
	cMatrix lWorldMatrix;
	// Set World Matrix
	std::string name = this->GetName().c_str();

	cGraphicManager::Get().SetWorldMatrix(mWorldMatrix);
	
	if (name.compare("mustang_metralleta") == 0){
		// Rotaciones del ratón
		float lfYaw = GetValue( eIA_MouseYaw );
		float lfPitch = GetValue( eIA_MousePitch );
		//cGraphicManager::Get().DrawLine(cVec3(-146.0f, 41.0f, 10.0f ), cVec3(lfYaw, lfPitch, 1.f), cVec3(1.f, 0.f, 0.f));
		//cGraphicManager::Get().DrawLine(cVec3(-146.0f, 41.0f, 10.0f ), cVec3(-146.f, 41.f, 300.f), cVec3(1.f, 0.f, 0.f));

		//cGraphicManager::Get().DrawPoint(cVec3(-146.f, 41.f, 10.f),cVec3(1.f, 0.f, 0.f));
		//cGraphicManager::Get().DrawAxis();	
	}
	for (unsigned luiIndex = 0; luiIndex < mMeshHandles.size(); ++luiIndex){
		// Set the material
		cMaterial* lpMaterial = ( cMaterial* ) mMaterialHandles[luiIndex].GetResource();
		cMesh *lpMesh = (cMesh *)mMeshHandles[luiIndex].GetResource();

		// Prepare all the parameters for the render
		lpMaterial->PrepareRender();
		lpMesh->PrepareRender(mMaterialHandles[luiIndex]);

		// Set the fist pass
		bool lbContinue = lpMaterial->SetFirstPass();
		while (lbContinue) {
			// Render Mesh
			lpMesh->RenderMesh();
			// Prepare the next pass
			lbContinue = lpMaterial->SetNextPass();
		}	
	}
	// Restore the world matrix
	lWorldMatrix.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorldMatrix);
}

// 
void cObject::Update( float lfTimestep ){
	for ( unsigned luiIndex = 0; luiIndex < mMeshHandles.size(); ++luiIndex ){
		cMesh *lpMesh = (cMesh *)mMeshHandles[luiIndex].GetResource();
		lpMesh->Update(lfTimestep);
	}
}
