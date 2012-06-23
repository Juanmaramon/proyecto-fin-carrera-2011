#include "Bullet.h"
#include "..\..\Graphics\GraphicManager.h" 

void Bullet::Init(float weapon_range) {

	miId = 0;
	mbDocreate = true;
	// Rango máximo del arma
	mfWeapon_range = weapon_range;
}

void Bullet::Create(cVec3 posIni, cVec3 posEnd, float angle) {
	
	if (miId >= 1 /*MAX_BULLETS*/)
		mbDocreate = false;

	if (mbDocreate) {
		bullets lvData;
		lvData.ini_position = posIni;
		lvData.end_position = posEnd;
		lvData.angle = angle;
		lvData.active = true;
		container.push_back(lvData);

		miId++;
		char buff[256];
		sprintf(buff, "id: %d, bx: %2.2f, by: %2.2f, bz: %2.2f, angle: %2.2f\n", miId, posIni.x, posIni.y, posIni.z, angle);
		//OutputDebugStr(buff);
	}

}

void Bullet::Update() {

	if (miId > 0 || mbDocreate) {
		for (int l = 0; l < miId; l++) {

			bullets lvElement = container[l];
			lvElement.ini_position.x += cos(lvElement.angle * (PI/180)) * BULLET_SPEED;
			lvElement.ini_position.z += sin(lvElement.angle * (PI/180)) * BULLET_SPEED;

			// Y se mantiene constante
			
			container[l] = lvElement;

			if (miId == 1) {
				char buff[256];
				sprintf(buff, "id: %d, ini_x: %2.2f, ini_y: %2.2f, ini_z: %2.2f, angle: %2.2f, end_x: %2.2f, end_y: %2.2f, end_z: %2.2f\n", miId, lvElement.ini_position.x, lvElement.ini_position.y, lvElement.ini_position.z, lvElement.angle, lvElement.end_position.x, lvElement.end_position.y, lvElement.end_position.z);
				//OutputDebugStr(buff);
			}
		}
	}

}

void Bullet::Render() {

	if (miId > 0 || mbDocreate) {
		for(int l = 0; l < miId; l++) {

			bullets lvElement = container[l];

			cMatrix lmPosMatrix;
			lmPosMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), lvElement.angle);
			lmPosMatrix.LoadTranslation(cVec3(lvElement.ini_position.x, lvElement.ini_position.y, lvElement.ini_position.z));

			cGraphicManager::Get().SetWorldMatrix(lmPosMatrix);

			glBegin(GL_QUADS);

				glVertex3f(0, 0, 0);

				glVertex3f(5, 0, 0);

				glVertex3f(5, 10, 0);

				glVertex3f(0, 10, 0);

			glEnd();

			cGraphicManager::Get().SetWorldMatrix(lmPosMatrix.LoadIdentity());
		}
	}

	/*if (mbDocreate) {
		miId++;
		mbDocreate = false;	
	}*/

}

void Bullet::Deinit() {

	container.empty();

}