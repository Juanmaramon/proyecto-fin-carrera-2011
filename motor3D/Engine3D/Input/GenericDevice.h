/*
La interfaz GenericDevice contiene la serie de m�todos que debe implementar un dispositivo 
para ser soportado por el InputManager. Esta interfaz est� preparada para trabajar con los 
dispositivos en modo encuesta, ya que pregunta por el estado del dispositivo, en lugar de 
recibir eventos.
*/

#ifndef GENERIC_DEVICE_H
#define GENERIC_DEVICE_H
 
class cGenericDevice
{
	public:
 
	   //Inicializa el dispositivo.
	   virtual void Init() = 0;

	   //Libera el dispositivo.
	   virtual void Deinit(void) = 0;

	   //Se llamar� en cada frame para actualizar el estado del dispositivo.
	   virtual void Update(void) = 0;

	   //Permitir� conocer el estado de uno de los canales de entrada (si un bot�n est� pulsado, ...).
	   virtual float Check(unsigned luiEntry) = 0;

	   //Indicar� si el dispositivo es v�lido y le podemos consultar el estado de sus canales.
	   virtual bool IsValid(void) = 0;
};
 
#endif