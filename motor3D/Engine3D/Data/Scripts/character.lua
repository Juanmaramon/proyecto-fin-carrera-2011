--REQUIRES--

--DECLARACION DE VARIABLES GLOBALES--
posX = 0.0
posY = 0.0
posZ = 0.0
speed = 1.5
angSpeed = 1.0

--DEFINICION DE FUNCIONES--

--CODIGO LUA--
--Llamamos desde Lua a la funcion CreatePatrol definida en C++
--creara un personaje con comportamiento perseguidor
id = CreatePatrol( posX, posY, posZ, speed, angSpeed )