#include "Explosion.h"
#include "../../Graphics/Textures/Texture.h"
#include "..\..\Graphics\GraphicManager.h"
#include "..\..\Game\Game.h" 
#include "..\..\Graphics\Camera.h"
#include <iostream>
#include <algorithm>
#include <vector>

void Explosion::Init(cResourceHandle* explosion_sprite, cResourceHandle* explosion_sprite1, cResourceHandle* flash, cResourceHandle* particleTexture) {

	mExplosion_sprite = explosion_sprite;
	mExplosion_sprite1 = explosion_sprite1;
	mExplosion_flash = flash;
	miExplosionSeq = 0;
	miExplosionStep = 0;
	mTranslate.LoadIdentity();
	mAlive = false;

	InitParticles(particleTexture);
}

void Explosion::SetPosition (cMatrix position) {
	mTranslate = position;
}

void Explosion::Render() {

	char buff1 [256];

	if (!mAlive)
		return;

	cGraphicManager::Get().SetWorldMatrix(mTranslate);

	// Se aplica el billboard para ignorar la rotacion y orientar el quad a la camara
	cGraphicManager::Get().BillboardCheatSphericalBegin();

	cTexture* lpTexture = (cTexture*) mExplosion_sprite->GetResource();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Explosion animada
	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (miExplosionSeq) {
		case 1:	
								xo = 0.0f;							yo = 0.0f;
								xf = xo + 0.25f;					yf = yo + 0.24f;
								break;	

		case 2:	
								xo = 0.25f;							yo = 0.0f;
								xf = xo + 0.25f;					yf = yo + 0.24f;
								break;

		case 3:	
								xo = 0.50f;							yo = 0.0f;
								xf = xo + 0.25f;					yf = yo + 0.24f;
								break;

		case 4:	
								xo = 0.75f;							yo = 0.0f;
								xf = xo + 0.25f;					yf = yo + 0.24f;
								break;

		case 5:	
								xo = 0.0f;							yo = 0.18f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;	

		case 6:	
								xo = 0.25f;							yo = 0.18f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 7:	
								xo = 0.50f;							yo = 0.18f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 8:	
								xo = 0.75f;							yo = 0.18f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 9:	
								xo = 0.0f;							yo = 0.37f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;
		case 10:	
								xo = 0.25f;							yo = 0.37f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 11:	
								xo = 0.50f;							yo = 0.37f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 12:	
								xo = 0.75f;							yo = 0.37f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 13:	
								xo = 0.0f;							yo = 0.56f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;
		case 14:	
								xo = 0.25f;							yo = 0.56f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 15:	
								xo = 0.50f;							yo = 0.56f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 16:	
								xo = 0.75f;							yo = 0.56f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 17:	
								xo = 0.0f;							yo = 0.75f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 18:	
								xo = 0.25f;							yo = 0.75f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 19:
								xo = 0.50f;							yo = 0.75f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								break;

		case 20:	
								xo = 0.75f;							yo = 0.75f;
								xf = xo + 0.25f;					yf = yo + 0.25f;
								mAlive = false;
								break;

	}

	// Fogonazo blanco
	if (miExplosionSeq == 0) {
		cCamera l2dCamera = cGame::Get().Get2DCamera();
		cGraphicManager::Get().ActivateCamera( &l2dCamera );

		cGraphicManager::Get().SetWorldMatrix(mTranslate.LoadIdentity());

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glColor3ub(255,255,255);

		glBegin(GL_QUADS);

			 glVertex3f(-600, -600, 0);	
			 glVertex3f(600, -600, 0);
			 glVertex3f(600, 600, 0);
			 glVertex3f(-600, 600, 0); 

		glEnd();

		cCamera l3dCamera = cGame::Get().Get3DCamera();
		cGraphicManager::Get().ActivateCamera( &l3dCamera );

		glEnable(GL_TEXTURE_2D);

	} else {

		glBegin(GL_QUADS);

			 glTexCoord2f(xo,yo); glVertex3f(-12, -12, 0);	
			 glTexCoord2f(xo,yf); glVertex3f(12, -12, 0);
			 glTexCoord2f(xf,yf); glVertex3f(12, 12, 0);
			 glTexCoord2f(xf,yo); glVertex3f(-12, 12, 0); 

		glEnd();

	}

	// Flash explosion
	switch (miExplosionSeq) {

		case 1:	
					lpTexture = (cTexture*) mExplosion_flash->GetResource();

					glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

					glEnable(GL_BLEND);
					glBlendFunc(GL_DST_COLOR, GL_ONE);

					glBegin(GL_QUADS);

						 glTexCoord2f(0,0); glVertex3f(-10, -10, 0);	
						 glTexCoord2f(0,1); glVertex3f(10, -10, 0);
						 glTexCoord2f(1,1); glVertex3f(10, 10, 0);
						 glTexCoord2f(1,0); glVertex3f(-10, 10, 0); 

					glEnd();
					break;

	}


	// Mini explosiones
	lpTexture = (cTexture*) mExplosion_sprite1->GetResource();

	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					/* scale linearly when image bigger than texture*/
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	/* scale linearly when image smalled than texture*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_DST_COLOR, GL_ONE);

	float a = -5.0f;
	float b = 5.0f;
	switch (miExplosionSeq) {

		case 1:	
					deviation_x = ((b - a) * ((float)rand() / RAND_MAX)) + a;
					deviation_z = ((b - a) * ((float)rand() / RAND_MAX)) + a;
					break;

		case 7:	
					deviation_x = ((b - a) * ((float)rand() / RAND_MAX)) + a;
					deviation_z = ((b - a) * ((float)rand() / RAND_MAX)) + a;
					break;

		case 20:	
					deviation_x = ((b - a) * ((float)rand() / RAND_MAX)) + a;
					deviation_z = ((b - a) * ((float)rand() / RAND_MAX)) + a;
					break;
	}


	cMatrix lmDeviation;

	if (miExplosionSeq != 0) {

		cGraphicManager::Get().SetWorldMatrix(mTranslate + lmDeviation.LoadTranslation(cVec3(deviation_x, 0.f, deviation_z)));

		// Se aplica el billboard para ignorar la rotacion y orientar el quad a la camara
		cGraphicManager::Get().BillboardCheatSphericalBegin();

		glBegin(GL_QUADS);

			 glTexCoord2f(0,0); glVertex3f(-1, -1, 0);	
			 glTexCoord2f(0,1); glVertex3f(1, -1, 0);
			 glTexCoord2f(1,1); glVertex3f(1, 1, 0);
			 glTexCoord2f(1,0); glVertex3f(-1, 1, 0); 

		glEnd();
	
	}

	sprintf(buff1, " step: %d \n", miExplosionStep);
	//OutputDebugStr(buff1);
	sprintf(buff1, " seq: %d \n", miExplosionSeq);
	//OutputDebugStr(buff1);

	// Incrementa el paso de la animacion
	if (miExplosionStep >= EXPLOSION_MAX_STEPS) {
		
		miExplosionStep = 0;
		miExplosionSeq++;
		miExplosionSeq = miExplosionSeq % 21;

	} else
		miExplosionStep++;

	glDisable(GL_BLEND);
	//glColor4f(1,1,1,1.f);

	cGraphicManager::Get().BillboardEnd();

	cGraphicManager::Get().SetWorldMatrix(mTranslate.LoadIdentity());

}

void Explosion::RenderSmoke() {

	// Actualiza sist. particulas
	StepParticles();

	// Pinta el sist. particulas
	RenderParticles();

}


//Returns a random float from 0 to < 1
float randomFloat() {
	return (float)rand() / ((float)RAND_MAX + 1);
}

//Returns the average velocity of particles produced by the fountain.
cVec3 Explosion::CurVelocity() {
	//return cVec3(0.0f, -3 * cos(mfAngle) , 3 * cos(mfAngle));
	return cVec3(0.5f * randomFloat() - 0.25f, 1.f * randomFloat() + 1.f, 0.5f * randomFloat() - 0.25f);
}

//Alters p to be a particle newly produced by the fountain.
void Explosion::CreateParticle(Particle* p) {
	float a = -60.0f;
	float b = 60.0f;
	float deviation_x = ((b - a) * ((float)rand() / RAND_MAX)) + a;
	float x = deviation_x * 0.01; // + mvParticleSysPos.x;
	float y = rand() % 1 * 0.01;
	float z = rand() % 1 * 0.01;
	p->pos = cVec3(x, 1, 1); 
	p->velocity = CurVelocity() + cVec3(0.5f * randomFloat() - 0.25f,
										0.5f * randomFloat() - 0.25f,
										0.5f * randomFloat() - 0.25f);

	p->color = cVec3(0.329412,0.329412,0.329412); 
	p->timeAlive = 0;
	p->lifespan = randomFloat() + 0.85f; 
}

//Advances the particle fountain by STEP_TIME seconds.
void Explosion::StepParticles() {
	
	for (int i = 0; i < NUM_PARTICLES; i++) {
		Particle* p = particles + i;
		
		p->pos += p->velocity * STEP_TIME;
		p->velocity += cVec3(0.0f, GRAVITY * STEP_TIME, 0.0f);
		p->timeAlive += STEP_TIME;

		if (p->timeAlive >= p->lifespan / 4)
			p->color = cVec3(0.329412,0.329412,0.329412); 
			//p->color = cVec3(0.9,0.9,1.0);
		if (p->timeAlive > p->lifespan) {
			CreateParticle(p);
		}
	}
}

void Explosion::InitParticles(cResourceHandle* textureParticle) {
	mTextureParticle = textureParticle;
	lfTimeUntilNextStep = 0;
	lfColorTime = 0;
	mfAngle = 0;
	mvParticleSysPos.LoadIdentity();
	for(int i = 0; i < NUM_PARTICLES; i++) {
		CreateParticle(particles + i);
	}
	for(int i = 0; i < 5 / STEP_TIME; i++) {
		StepParticles();
	}
}

//Advances the particle fountain by the specified amount of time.
void Explosion::UpdateParticles(float lfTimestep) {
	while (lfTimestep > 0) {
		if (lfTimeUntilNextStep < lfTimestep) {
			lfTimestep -= lfTimeUntilNextStep;
			StepParticles();
			lfTimeUntilNextStep = STEP_TIME;
		}
		else {
			lfTimeUntilNextStep -= lfTimestep;
			lfTimestep = 0;
		}
	}
}

//Returns whether particle1 is in back of particle2
bool CompareParticles(Particle* particle1, Particle* particle2) {
	return particle1->pos[2] < particle2->pos[2];
}

//Draws the particle system.
void Explosion::RenderParticles() {
	vector<Particle*> ps;

	for (int i = 0; i < NUM_PARTICLES; i++) {
		ps.push_back(particles + i);
	}

	//sort(ps.begin(), ps.end(), CompareParticles);

	cGraphicManager::Get().SetWorldMatrix(mvParticleSysPos);

	// Se aplica el billboard para ignorar la rotacion y orientar el quad a la camara
	cGraphicManager::Get().BillboardCheatSphericalBegin();

	glEnable(GL_BLEND);
	/*glBlendFunc(GL_DST_COLOR, GL_ZERO);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	cTexture* lpTexture = (cTexture*) mTextureParticleMask->GetResource();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					// scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	// scale linearly when image smalled than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glBegin(GL_QUADS);

	for (unsigned int i = 0; i < ps.size(); i++) {
		Particle* p = ps[i];
		//glColor4f(p->color[0], p->color[1], p->color[2],
				 // (1 - p->timeAlive / p->lifespan));

		float size = PARTICLE_SIZE / 2;
		
		cVec3 pos = p->pos;
		
		glTexCoord2f(0, 0);		glVertex3f(pos[0] - size, pos[1] - size, pos[2]);
		glTexCoord2f(0, 1);		glVertex3f(pos[0] - size, pos[1] + size, pos[2]);
		glTexCoord2f(1, 1);		glVertex3f(pos[0] + size, pos[1] + size, pos[2]);
		glTexCoord2f(1, 0);		glVertex3f(pos[0] + size, pos[1] - size, pos[2]);

	}

	glEnd();*/

	// Segunda pasada

	//glBlendFunc(GL_ONE, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	cTexture* lpTexture = (cTexture*) mTextureParticle->GetResource();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, lpTexture->GetTextureHandle());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);					// scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	// scale linearly when image smalled than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glBegin(GL_QUADS);

	for (unsigned int i = 0; i < ps.size(); i++) {
		Particle* p = ps[i];
		glColor4f(p->color[0], p->color[1], p->color[2],
				  (1 - p->timeAlive / p->lifespan));

		float size = PARTICLE_SIZE / 2;
		
		cVec3 pos = p->pos;
		
		glTexCoord2f(0, 0);		glVertex3f(pos[0] - size, pos[1] - size, pos[2]);
		glTexCoord2f(0, 1);		glVertex3f(pos[0] - size, pos[1] + size, pos[2]);
		glTexCoord2f(1, 1);		glVertex3f(pos[0] + size, pos[1] + size, pos[2]);
		glTexCoord2f(1, 0);		glVertex3f(pos[0] + size, pos[1] - size, pos[2]);

	}

	glEnd();

	glDisable(GL_BLEND);

	cGraphicManager::Get().BillboardEnd();

	cGraphicManager::Get().SetWorldMatrix(mvParticleSysPos.LoadIdentity());
}