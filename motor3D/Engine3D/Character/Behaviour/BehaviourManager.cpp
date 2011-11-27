#include "BehaviourManager.h"
#include <assert.h>

//Devuelve true
bool cBehaviourManager::Init(){
	return true;
}

//Elimina todos los comportamientos de la lista.
bool cBehaviourManager::Deinit(){
	//Se libera la memoria reservada
	for (cBehaviourListIt lpIt = mBehaviours.begin(); lpIt != mBehaviours.end(); ++lpIt )
	{
		delete *lpIt;
	}
	//Se eliminan todos los compartamientos de las lista.
	mBehaviours.clear();
	return true;
}

//Crea un nuevo comportamiento del tipo indicado en leBehaviour y lo
//inserta en la lista,devolviendo un puntero al comportamiento creado.
cBehaviourBase* cBehaviourManager::CreateBehaviour( eBehaviours leBehaviour ){
	
	cBehaviourBase * lpBehaviour;

	//Reservamos memoria para un objeto de tipo cBehaviour y obtenemos un puntero a dicho objeto
	switch(leBehaviour){
		case eCHASER_NO_ORIENTATION: lpBehaviour = new cChaserNoOrientation;
									 break;	
		case eCHASER_SNAP_ORIENTATION: lpBehaviour = new cChaserSnapOrientation;
									 break;
 		case eCHASER_WITH_ORIENTATION: lpBehaviour = new cChaserWithOrientation;
									 break;
		case ePATROL: lpBehaviour = new cPatrol;
									 break;
	}
	
	//Nos aseguramos de que el puntero no es NULL
	assert(lpBehaviour);

	//A�adimos a la lista un puntero al comportamiento reci�n creado
	mBehaviours.push_back(lpBehaviour);	

	//Devolvemos un puntero al comportamiento que acabamos de crear
	return lpBehaviour;
}

//Elimina un comportamiento de la lista.
void cBehaviourManager::DestroyBehaviour(cBehaviourBase* lpBehaviour){
	mBehaviours.remove(lpBehaviour);
}

//Asumiremos que esta clase no realizar� ninguna llamada al Init() y
//Deinit() de los comportamientos. Cada personaje al que se le asigne
//un comportamiento ser� el encargado de llamar al Init() y Deinit()
//del comportamiento cuando corresponda. Esto es as�, ya que la
//inicializaci�n y destrucci�n de un comportamiento est�n relacionados
//con el momento en que se asocia un comportamiento a un personaje.
//Recuerden que deben llamar al DestroyBehaviour del comportamiento
//asignado a un personaje en la funci�n Deinit() del personaje, justo
//despu�s de llamar al Deinit() de dicho comportamiento.