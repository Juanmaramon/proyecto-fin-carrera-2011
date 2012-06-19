// Clase que representa las balas que dispara una entidad del juego

#ifndef BULLET_H
#define BULLET_H

#include "..\..\MathLib\MathLib.h"
#include <vector>
using namespace std;

#define MAX_BULLETS 128
#define BULLET_SPEED 0.01 

typedef struct						// Create A Structure For Bullet
{
	bool	active;					// Active (Yes/No)
	cVec3	ini_position;			// Position inicial
	cVec3	end_position;			// Posicion final
	float	angle;					// Angulo de la bala
}
bullets;							// Bullet Structure

class Bullet{

private:
	std::vector<bullets> container;
	//int container[128][3];
	int miSpeed, miId;
	bool mbDocreate;
	float mfWeapon_range;

public:
	Bullet() {;}
	void Init(float weapon_range);

	void SetBulletRange(float weapon_range) { mfWeapon_range = weapon_range; }

	void Create(cVec3 posIni, cVec3 posEnd, float angle);

	void Update();

	void Render();

	void Deinit();

};

#endif