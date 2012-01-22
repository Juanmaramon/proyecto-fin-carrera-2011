#ifndef SKYBOX_H
#define SKYBOX_H

#include "..\..\Utility\Singleton.h"
#include "..\..\MathLib\MathLib.h"
#include "../../Utility/ResourceHandle.h"
#include "../../Graphics/GLHeaders.h"

class Skybox : public cSingleton<Skybox>{

private:
	cResourceHandle tex_front, tex_back, tex_left, tex_right, tex_top, tex_bottom;
public:
	void CreateSkyBox(float x, float y, float z, float width, float height, float length);
	int LoadSkyBoxTextures (char *front, char *back, char *left, char *right, char *top, char *bottom);
	void Init();
	void Render();
};

#endif