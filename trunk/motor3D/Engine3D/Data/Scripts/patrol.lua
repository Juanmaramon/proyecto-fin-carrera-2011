--REQUIRES--

--DECLARACION DE VARIABLES GLOBALES--

--Inicializar el array con las posiciones de cada estado por cada patrullero:
lPos = {}
lPos[1] = {}

--Posiciones del patrullero 
lPos[1][1] = {x=3.0, y=0.0, z=3.0}
lPos[1][2] = {x=4.0, y=0.0, z=-5.0}
lPos[1][3] = {x=-3.0, y=0.0, z=-2.0}
lPos[1][4] = {x=-1.0, y=0.0, z=5.0}

--Estado actual es cero (cambiará a 1 cuando se llame a NextPoint)
lCurrentStates = {}
lCurrentStates[1] = 0

--Numero de estados
lPointCount = 4


--DEFINICION DE FUNCIONES--
function NextEndPoint( liCharacterId )
	lNextState = lCurrentStates[ liCharacterId ] + 1
	if ( lNextState > lPointCount ) then
		lNextState = 1
	end
	--Cambiar al siguiente estado
	lCurrentStates[ liCharacterId ] = lNextState

	--Actualizar el nuevo punto a alcanzar
	SetPatrolTarget( liCharacterId, lPos[liCharacterId][lNextState].x, lPos[liCharacterId][lNextState].y, lPos[liCharacterId][lNextState].z)

end


---Función renderiza el circuito (Path) que debe seguir el patrullero identificado por
--- liCharacterId.
---Será llamada desde C++.
function DrawPath( liCharacterId )
	---Se invoca a la función Drawline (dibuja la línea que une 2 puntos) de C++, tantas veces como sea necesario
	--- para dibujar todo el circuito.
	lPuntoA = {}
	lPuntoB = {}
	for i = 1, (#lPos[liCharacterId] - 1) do
		lPuntoA = lPos[liCharacterId][i]
		lPuntoB = lPos[liCharacterId][i+1]
		---Los puntos vienen dados en forma de array {x, y, z}
		DrawLine( lPuntoA.x, lPuntoA.y, lPuntoA.z, lPuntoB.x, lPuntoB.y, lPuntoB.z )
	end
	---Se dibuja la línea que une el último punto con el primero.
	---El último punto queda almacenado en lPuntoB al salir del bucle
	lPuntoA = lPos[liCharacterId][1]
	DrawLine( lPuntoB.x, lPuntoB.y, lPuntoB.z, lPuntoA.x, lPuntoA.y, lPuntoA.z )	
	return #lPos
end



--CODIGO LUA--

