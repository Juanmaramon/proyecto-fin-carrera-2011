#include "Camera.h"

//Funci�n que inicializa la clase Camera.
void cCamera::Init(){
	//Se inicializan las 3 matrices a la identidad:
	mView.LoadIdentity();
	mProj.LoadIdentity();
	mViewProj.LoadIdentity();
}

//Funci�n que define la matriz de vista. 
//Recibe 3 argumentos, que son:
//    D�nde est� la c�mara.
//    Punto al que mira la c�mara.
//    Un  vector   que  indica  la  direcci�n   Up  de  la  c�mara  antes  de  posicionarla  y 
//     orientarla con los otros dos par�metros. Normalmente este valor apunta hacia 
//     arriba (0,1,0).
void cCamera::SetLookAt( const cVec3 &lvPosition, 
                         const cVec3 &lvTarget, 
                         const cVec3 &lvUpWorld ){
   mView.LoadLookAt(lvPosition, lvTarget, lvUpWorld);
   RefreshViewProj();
}


//Funci�n para definir la matriz de proyecci�n en ortogonal (objetos lejanos y cercanos son
// del mismo tama�o. Este tipo de proyecci�n es muy �til para renderizar texto e interfaces de usuario)
//Los primeros cuatro par�metros definen el tama�o del �rea que se va a renderizar, 
// mientras que los otros 2 definen el clipping con respecto a la distancia a la c�mara.  Es 
// decir, que lo que la c�mara mostrar� por pantalla ser� todo aquello que se encuentre a 
// una distancia mayor que el lfZNear y menor que el lfZFar. El valor lfZNear siempre 
// tiene que ser mayor que 0.
void cCamera::SetOrtho( float  lfLeft, float  lfRight, 
						float  lfBottom, float  lfTop, 
						float  lfZNear, float  lfZNFar ){
   mProj.LoadOrtho(lfLeft, lfRight, lfBottom, lfTop, lfZNear, lfZNFar);
   RefreshViewProj();
}

//Funci�n para definir la matriz de proyecci�n en perspectiva (objetos lejanos se muestran m�s peque�os).
//La matriz de proyecci�n perspectiva es la que normalmente se usa para renderizar el 
// mundo en 3D.
//Los par�metros son el �ngulo de apertura de la c�mara en el eje Y, la relaci�n de 
// aspecto entre el ancho y el alto y los valores lfZNear y lfZFar que tienen el mismo 
// significado que en la proyecci�n ortogonal.
void cCamera::SetPerspective( float  lfFOVY_Rad, float  lfAspect,
								float  lfZNear, float  lfZFar ){
   mProj.LoadPerpective(lfFOVY_Rad, lfAspect, lfZNear, lfZFar);
   RefreshViewProj();
}

void cCamera::SetYaw(float rotation){
	cMatrix lYaw; 
	mView *= lYaw.LoadRotation(mView.GetUp(), rotation) ;  
}

void cCamera::SetPitch(float rotation){
	cMatrix lRot; 
	mView *= lRot.LoadRotation(cVec3(1.0f,0.0f,0.0f), rotation) ;
}