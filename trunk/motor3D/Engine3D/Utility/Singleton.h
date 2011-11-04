//Plantilla para el patr�n Singleton

#ifndef Singleton_H
#define Singleton_H

template<typename T> class cSingleton
{
   public:
      static T& Get()
      {
         static T lInstance;
         return lInstance;
       }
};

#endif 