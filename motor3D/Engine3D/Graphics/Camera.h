//Clase que representa la c�mara del juego.

#ifndef Camera_H
#define Camera_H

#include "..\MathLib\MathLib.h"

class cCamera
{
   public:
      //Funci�n que inicializa la clase Camera.
      void Init();

	  //Para mantener la coherencia de los datos de la clase, cada vez que se modifique
	  // una de las dos matrices (vista o proyecci�n) se deber� actualizar la matriz mViewProj,
	  // usando esta funci�n que multiplica nView * mProj.
	  inline void RefreshViewProj()   { mViewProj = mView * mProj; }

	  //Funci�n que define la matriz de vista. 
	  //Recibe 3 argumentos, que son:
	  //    D�nde est� la c�mara.
	  //    Punto al que mira la c�mara.
	  //    Un  vector   que  indica  la  direcci�n   Up  de  la  c�mara  antes  de  posicionarla  y 
	  //     orientarla con los otros dos par�metros. Normalmente este valor apunta hacia 
	  //     arriba (0,1,0).
	  void SetLookAt( const cVec3 &lvPosition, 
                         const cVec3 &lvTarget, 
                         const cVec3 &lvUpWorld );

	  //Funci�n para definir la matriz de proyecci�n en ortogonal (objetos lejanos y cercanos son
	  // del mismo tama�o. Este tipo de proyecci�n es muy �til para renderizar texto e interfaces de usuario)
	  //Los primeros cuatro par�metros definen el tama�o del �rea que se va a renderizar, 
	  // mientras que los otros 2 definen el clipping con respecto a la distancia a la c�mara.  Es 
	  // decir, que lo que la c�mara mostrar� por pantalla ser� todo aquello que se encuentre a 
	  // una distancia mayor que el lfZNear y menor que el lfZFar. El valor lfZNear siempre 
	  // tiene que ser mayor que 0.
	  void SetOrtho( float  lfLeft, float  lfRight, 
					  float  lfBottom, float  lfTop, 
					  float  lfZNear, float  lfZNFar );

	  //Funci�n para definir la matriz de proyecci�n en perspectiva (objetos lejanos se muestran m�s peque�os).
	  //La matriz de proyecci�n perspectiva es la que normalmente se usa para renderizar el 
	  // mundo en 3D.
	  //Los par�metros son el �ngulo de apertura de la c�mara en el eje Y, la relaci�n de 
	  // aspecto entre el ancho y el alto y los valores lfZNear y lfZFar que tienen el mismo 
	  // significado que en la proyecci�n ortogonal.
	  void SetPerspective( float  lfFOVY_Rad, float  lfAspect,
							float  lfZNear, float  lfZFar );

	  //Funciones que devuelven informaci�n relativa a la orientaci�n y posici�n de la c�mara.
	  cVec3 GetFront()       const   { return mView.GetFront(); }
	  cVec3 GetLeft()        const   { return mView.GetLeft();  }
	  cVec3 GetRight()       const   { return mView.GetRight(); }
	  cVec3 GetUp()          const   { return mView.GetUp();    }
	  cVec3 GetPosition()    const   { return mView.GetPosition();    }
 
	  //Funciones que devuelven las matrices de esta clase (camera).
	  const cMatrix &GetView() const { return mView; }
	  const cMatrix &GetProj() const { return mProj; }
	  const cMatrix &GetViewProj() const { return mViewProj; }	

	  void SetView(cVec3 view){ mView.SetPosition(view); RefreshViewProj(); }

	  //void MoveCamera(float speed);

   private:

	//La c�mara se define a trav�s de dos matrices: La matriz de vista (view) y la matriz de 
	// proyecci�n (projection). La matriz de vista determina d�nde est� la c�mara en nuestro 
	// juego y que orientaci�n tiene. La matriz de proyecci�n determina las caracter�sticas de 
	// la �lente� de la c�mara y como se realizar� la proyecci�n del mundo 3D sobre el plano 
	// de la 2D de la pantalla.
      cMatrix mView;
      cMatrix mProj;
	  //Se a�ade una matriz adicional llamada mViewProj que contendr� la multiplicaci�n 
	  // entre la matriz de vista y la de proyecci�n.
      cMatrix mViewProj;
};

#endif