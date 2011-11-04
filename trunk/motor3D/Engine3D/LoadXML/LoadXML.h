//Clase que se encarga de cargar y mostrar el contenido de un fichero XML

#ifndef LoadXML_H
#define LoadXML_H

#include "../Graphics/Fonts/FontProperties.h"
//Includes para usar TinyXML
#include <tinystr.h>
#include <tinyxml.h>

class cLoadXML
{
	protected:		       		
		TiXmlDocument mDoc; //Instancia de la clase "TiXmlDocument" para manejar el fichero XML.

		//Funci�n que vuelca el contenido de los atributos de un elemento del fichero XML, y
		// devuelve el n�mero de atributos del elemento.
		int Output_attributes(TiXmlElement* lpElement, unsigned int luiIndent);

        //Funci�n que devuelve una cadena de caraceteres en blanco concatenados con el s�mbolo "+", si se indica true el 
		// par�metro correspondiente, para crear la indentaci�n (sangr�a) durante el volcado del fichero XML.
		const char * GetIndent(unsigned int luiNumIndents, bool lbSimbolPlus);
    
	public:
		cLoadXML() { ; } //constructor

		//Funci�n para cargar el fichero XML indicado.
		//Devuelve true cuando se carga correctamente y false en caso contrario.
		bool Init(const char* kpcFileName);

		TiXmlDocument* GetTiXmlDocument();

		//Funci�n que vuelca el contenido del fichero XML desde un nodo padre.
		void Output_fromNodeParent(TiXmlNode* lpParent, unsigned int luiIndent = 0);		

		//Funci�n para finalizar la clase
		bool DeInit();

		//Load window's parameters function
		void cLoadXML::ParseAndLoad(TiXmlNode* lpParent, unsigned int luiIndent, cFontProperties* mpFontProps);

};

#endif