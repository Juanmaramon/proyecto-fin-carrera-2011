#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "../../Utility/ResourceHandle.h"
#include "../../Graphics/GLHeaders.h"
#include "..\..\Libraries\Bullet\include\btBulletCollisionCommon.h"
#include "..\..\Libraries\Bullet\include\btBulletDynamicsCommon.h"
#include "..\..\Libraries\Bullet\include\BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h"
#include "..\..\Physics\cPhysics.h"

#define MAP_SIZE	1024	// This is the size of our .raw height map
#define BULLET_MAP_SIZE	64	// This is the size of our bullet submesh
#define NSUBMESH (MAP_SIZE*MAP_SIZE)/(BULLET_MAP_SIZE*BULLET_MAP_SIZE)  
#define CHUNK	BULLET_MAP_SIZE*BULLET_MAP_SIZE
#define STEP_SIZE	8		// This is width and height of each QUAD
static const float s_gridHeightScale = 1.0;

class Heightmap{

public:

	bool Load();
	void Render();
	void Create(btScalar& minHeight, btScalar& maxHeight);
	int  Height(int X,int Y);
	void Deinit(void);
	void clearWorld(void);

private:

	bool LoadRawFile(char *file,int size);
	void SetTextureCoord(float x, float z);

	unsigned char HeightMap[MAP_SIZE*MAP_SIZE]; // 0..255 values
	//typedef unsigned char subheightmap[CHUNK];
	unsigned char subheightmaps[NSUBMESH][CHUNK];
	unsigned char smallHeightMap[BULLET_MAP_SIZE*BULLET_MAP_SIZE];
	int disp_list_id;
	cResourceHandle tex_floor, tex_detail, tex_water;
	bool detail;
	int  detail_level;
};

#endif