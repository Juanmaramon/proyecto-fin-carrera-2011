#include "ChaserSnapOrientation.h"
#include <string>

//Inicializa los atributos
void cChaserSnapOrientation::Init(cCharacter * lpCharacter){
		mpCharacter = lpCharacter;
		mTarget = cVec3(0.0f, 0.0f, 0.0f);		
}

//Deinicializa los atributos
void cChaserSnapOrientation::Deinit(){
	mpCharacter = NULL;
}

//Implementa la variante del comportamiento perseguidor
//correspondiente
void cChaserSnapOrientation::Update(float lfTimestep){

	//Calcular el vector distancia (diferencia entre la posici�n del
	//perseguidor y la posici�n del objetivo a perseguir)
	cVec3 lDistanceVec = - (mTarget, mpCharacter-> GetPosition());
	//Calcular la distancia que se mover� el perseguidor teniendo en
	//cuenta su velocidad m�xima
	float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;
	//Si la distancia que el perseguidor se va a desplazar es mayor o
	//igual a la distancia que hay a su objetivo, se mueve s�lo hasta el
	//punto objetivo (para no pasarse). Si no, se mueve la distancia
	//establecida (para irse acercando //poco a poco al objetivo)
	if (lfDisplacement >= lDistanceVec.Length())
	{
		mpCharacter->SetPosition( mTarget );
	}
	else
	{
		//La nueva posici�n del perseguidor ser� igual a su posici�n inicial
		//m�s el desplazamiento que debe hacer en la direcci�n indicada por el
		//vector distancia (para moverse as� en direcci�n a su objetivo)
		mpCharacter->SetPosition(mpCharacter->GetPosition() +
		( lfDisplacement * lDistanceVec.Normalize() ));
	}

	//Una vez el perseguidor se haya movido (ver comportamiento anterior),
	//debemos orientarlo. Para orientarlo hay que modificar el Yaw del
	//perseguidor de la //siguiente forma:
	if (lDistanceVec.Length() > 0.001f)
	{
		mpCharacter->SetYaw( atan2f(lDistanceVec.x, lDistanceVec.z) );
	}
	//Es necesario a�adir esa condici�n en el if para asegurar que la
	//distancia al objetivo es mayor a cero, ya que la funci�n atan2f
	//genera un error si es cero o un n�mero muy pr�ximo a cero.

}

//Setea el objetivo
void cChaserSnapOrientation::SetTarget(cVec3 target){
	mTarget = target;
}