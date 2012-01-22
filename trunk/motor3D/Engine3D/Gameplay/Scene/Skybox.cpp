#include "Skybox.h"
#include "../../Graphics/Textures/TextureManager.h"
#include "../../Graphics/Textures/Texture.h"

//////////////////////////////////////////////////////////
//	Function: "CreateSkyBox"
//
//	Input:	x,y,z   		            Center of SkyBox.
//			width, height, length		Size of SkyBox.
//
//	Purpose: Create SkyBox.
//////////////////////////////////////////////////////////
void Skybox::CreateSkyBox(float x, float y, float z, float width, float height, float length){
    // This centers the sky box around (x, y, z)
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	cTexture* lpTexture;

	if (tex_back.IsValidHandle()){
	    // Bind the BACK texture of the sky map to the BACK side of the cube
		lpTexture = (cTexture*) tex_back.GetResource();
		glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* scale linearly when image bigger than texture*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); /* scale linearly when image smalled than texture*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Start drawing the side as a QUAD
	    glBegin(GL_QUADS);		
		// Assign the texture coordinates and vertices for the BACK Side
/*		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,	y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,	y, z);			*/
	
/*		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,	y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,	y, z);			*/

		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,	y + height, z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,	y, z);

	    glEnd();
    }

   if (tex_front.IsValidHandle()){
	    // Bind the FRONT texture of the sky map to the FRONT side of the box
		lpTexture = (cTexture*) tex_front.GetResource();
		glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* scale linearly when image bigger than texture*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); /* scale linearly when image smalled than texture*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	    // Start drawing the side as a QUAD
	    glBegin(GL_QUADS);	
		// Assign the texture coordinates and vertices for the FRONT Side
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,	y, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,	y + height, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	    glEnd();
   }

   if (tex_bottom.IsValidHandle()){
	    // Bind the BOTTOM texture of the sky map to the BOTTOM side of the box
		lpTexture = (cTexture*) tex_bottom.GetResource();
		glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* scale linearly when image bigger than texture*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); /* scale linearly when image smalled than texture*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	    // Start drawing the side as a QUAD
	    glBegin(GL_QUADS);		
		// Assign the texture coordinates and vertices for the BOTTOM Side
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,	y, z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,	y, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	    glEnd();
  }

   if (tex_top.IsValidHandle()){
	    // Bind the TOP texture of the sky map to the TOP side of the box
		lpTexture = (cTexture*) tex_top.GetResource();
		glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* scale linearly when image bigger than texture*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); /* scale linearly when image smalled than texture*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	    // Start drawing the side as a QUAD
	    glBegin(GL_QUADS);		
		// Assign the texture coordinates and vertices for the TOP Side
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,	y + height, z + length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,	y + height, z);
	    glEnd();
  }

   if (tex_left.IsValidHandle()){
	    // Bind the LEFT texture of the sky map to the LEFT side of the box
		lpTexture = (cTexture*) tex_left.GetResource();
		glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* scale linearly when image bigger than texture*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); /* scale linearly when image smalled than texture*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	    // Start drawing the side as a QUAD
	    glBegin(GL_QUADS);		
		// Assign the texture coordinates and vertices for the LEFT Side
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,	y + height, z);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,	y + height, z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,	y, z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,	y, z);		
	    glEnd();
  }

   if (tex_right.IsValidHandle()){
	    // Bind the RIGHT texture of the sky map to the RIGHT side of the box
		lpTexture = (cTexture*) tex_right.GetResource();
		glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); /* scale linearly when image bigger than texture*/
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); /* scale linearly when image smalled than texture*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	    // Start drawing the side as a QUAD
	    glBegin(GL_QUADS);		
		// Assign the texture coordinates and vertices for the RIGHT Side
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);

	    glEnd();
  }
}

//////////////////////////////////////////////////////////
//	Function: "LoadSkyBoxTextures"
//
//	Input:	front,back,left,right,top,bottom   		  Name of the Textures to load for the six faces of the SkyBox.
//			textures[6]                               Array where the texture objects created are going to be stored in the same order as passed. 
//                                   
//	Purpose: Load and create texture objects.
//////////////////////////////////////////////////////////
int Skybox::LoadSkyBoxTextures (char *front, char *back, char *left, char *right, char *top, char *bottom){

	if (front != NULL){
		tex_front = cTextureManager::Get().LoadResource("sky_front", front);
		assert(tex_floor.IsValidHandle());
	} else {
		//tex_front = NULL;
	}
	if (back != NULL){
		tex_back = cTextureManager::Get().LoadResource("sky_back", back);
		assert(tex_back.IsValidHandle());	
	} else {
		//tex_back = 0;
	}
	if (left != NULL){
		tex_left = cTextureManager::Get().LoadResource("sky_left", left);
		assert(tex_left.IsValidHandle());	
	} else {
		//tex_left = 0;
	}

	if (right != NULL){
		tex_right = cTextureManager::Get().LoadResource("sky_right",right);
		assert(tex_right.IsValidHandle());	
	} else {
		//tex_right = 0;
	}

	if (top != NULL){
		tex_top = cTextureManager::Get().LoadResource("sky_top", top);
		assert(tex_back.IsValidHandle());
	} else {
		//tex_top = 0;
	}

	if (bottom != NULL){
		tex_bottom = cTextureManager::Get().LoadResource("sky_bottom", bottom);
		assert(tex_bottom.IsValidHandle());
	} else {
		//tex_bottom = 0;
	}

	return 0;
}

void Skybox::Init(){
	LoadSkyBoxTextures("Data/Scene/images/heat_ft.jpg", "Data/Scene/images/heat_bk.jpg","Data/Scene/images/heat_lf.jpg", "Data/Scene/images/heat_rt.jpg", "Data/Scene/images/heat_up.jpg", NULL);
}

void Skybox::Render(){
    glColor3f(1.f, 1.f, 1.f);
    CreateSkyBox(0, 5, 0, 1390, 1200, 1390);   // Render Inner Layer
}