#ifndef ApplicationProperties_H
#define ApplicationProperties_H

#include <iostream> //Para std.

//Esta estructura ser� la que se pase como par�metro a la funci�n Init de la clase cWindow.
struct cApplicationProperties
{
   // General Application Parameters
   std::string macApplicationName; //Nombre de la aplicaci�n y de la ventana
   unsigned muiWidth; //Ancho de la Ventana
   unsigned muiHeight; //Alto de la Ventana
   unsigned muiBits; //Bits que tendr� cada p�xel de la ventana. Normalmente 24 (RGB) � 32 (RGBA) bits por p�xel.
   bool mbFullscreen; //True si tiene que ejecutar a pantalla completa
};

#endif