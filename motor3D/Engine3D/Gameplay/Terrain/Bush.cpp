#include "Bush.h"
#include "../../Graphics/Textures/Texture.h"

void Bush::Init(cResourceHandle* bush, cResourceHandle* bushMask, cMatrix position){
	mBushTexture = bush;
	mBushTextureMask = bushMask;
	mBushPosition = position;
}

void Bush::Update(float lfTimestep) {}

void Bush::Render(){

	cGraphicManager::Get().SetWorldMatrix(mBushPosition);

	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR,GL_ZERO);

	cTexture* lpTexture = (cTexture*) mBushTextureMask->GetResource();

	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

	// Se aplica el billboard para ignorar la rotacion y orientar el quad a la camara
	cGraphicManager::Get().BillboardCheatSphericalBegin();

		glBegin(GL_QUADS);

			 /*glTexCoord2f(0,0); glVertex3f(-5, -5, 0);	
			 glTexCoord2f(0,1); glVertex3f(5, -5, 0);
			 glTexCoord2f(1,1); glVertex3f(5, 5, 0);
			 glTexCoord2f(1,0); glVertex3f(-5, 5, 0); */

			 glTexCoord2f(0,1); glVertex3f(-1, -1, 0);	
			 glTexCoord2f(1,1); glVertex3f(1, -1, 0);
			 glTexCoord2f(1,0); glVertex3f(1, 1, 0);
			 glTexCoord2f(0,0); glVertex3f(-1, 1, 0);

		glEnd();

	cGraphicManager::Get().BillboardEnd();

	glBlendFunc(GL_ONE, GL_ONE);

	lpTexture = (cTexture*) mBushTexture->GetResource();

	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

	// Se aplica el billboard para ignorar la rotacion y orientar el quad a la camara
	cGraphicManager::Get().BillboardCheatSphericalBegin();

		glBegin(GL_QUADS);

			 /*glTexCoord2f(0,0); glVertex3f(-5, -5, 0);	
			 glTexCoord2f(0,1); glVertex3f(5, -5, 0);
			 glTexCoord2f(1,1); glVertex3f(5, 5, 0);
			 glTexCoord2f(1,0); glVertex3f(-5, 5, 0); */

			 glTexCoord2f(0,1); glVertex3f(-1, -1, 0);	
			 glTexCoord2f(1,1); glVertex3f(1, -1, 0);
			 glTexCoord2f(1,0); glVertex3f(1, 1, 0);
			 glTexCoord2f(0,0); glVertex3f(-1, 1, 0);

		glEnd();

	cGraphicManager::Get().BillboardEnd();
		
	glDisable(GL_BLEND);

	cMatrix mReset;
	cGraphicManager::Get().SetWorldMatrix(mReset.LoadIdentity());

}