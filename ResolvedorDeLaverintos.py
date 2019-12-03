import subprocess
import sys
# Ejecuta un programa en c utilizando el segundo y tercer argumento pasado al archivo de python para generar un laberinto.
# Este nuevo laberinto se guarda en un archivo llamado laberinto.txt
def crearLaberinto():
    response = subprocess.run([sys.argv[1], sys.argv[2], sys.argv[3]])
    if(response.returncode!=0):
        print("\nLa creacion del laberinto fallo, el programa va a cerrarse")
        sys.exit()

# Recibe un archivo con un laberinto y devuelve una matriz de caracteres con el mismo
def leerArchivo(archivo):
    f = open (archivo,'r')
    matriz = f.read()
    f.close()
    matriz=matriz.split()
    return matriz

# Recibe una matriz de caracteres que representa un laberinto y devuelve la posicion del inicio del mismo.
def obtenerInicio(matriz,objetivo):
    dimension=len(matriz[0])
    inicio=[]
    for i in range(dimension):
        for j in range(dimension):
            if(matriz[i][j]=='X'):
                objetivo.append(j)
                objetivo.append(i)
            if(matriz[i][j]=='I'):
                inicio.append(j)
                inicio.append(i)
    return inicio

# Recibe una matriz de caracteres, una posicion, un array de posiciones, y la dimension de la matriz, y devuelve True si la posicion es ubicable en la matriz,
# o False en caso contrario.
def verificar(matriz, pos , visitados,dimension):
    if (pos[0]<0 or dimension<=pos[0] or dimension<=pos[1] or pos[1]<0):
        return False
    return not(matriz[pos[1]][pos[0]]=='1' or ([pos[0],pos[1]] in visitados))

# Recibe un array de posiciones visitadas, una queue de posiciones y una posicion, y agrega la posicion a la queue y al array de visitados
def visitado(visitados, queue,pos):
    visitados.append(pos)
    queue.append(pos)

# Recibe una matriz de caracteres, su dimension, una queue, una lista de elementos visitados, una lista de posiciones, y llama a la funcion visitado
# en el orden dado por la lista de posiciones
def visitarNodoEnOrden(matriz,dimension,queue,visitados,listaPos):
    
    if(verificar(matriz,listaPos[0],visitados,dimension)):
        visitado(visitados, queue, listaPos[0])
    elif(verificar(matriz, listaPos[1],visitados,dimension)):
        visitado(visitados, queue, listaPos[1])
    elif(verificar(matriz, listaPos[2],visitados,dimension)):
        visitado(visitados, queue, listaPos[2])
    elif(verificar(matriz, listaPos[3],visitados,dimension)):
        visitado(visitados, queue, listaPos[3])
    else:
        queue.pop()

# Llama a la funcion visitarNodoEnOrden, pasandole como parametro una lista de posiciones, en orden de prioridad de cual visitar, basado en la 
# distancia de la posicion en la que se encuentra y el objetivo
def nodoAVisitar(matriz,dimension,queue,pos,visitados,objetivo):
    if(abs(pos[0]-1-objetivo[0]) <= abs(pos[0]+1-objetivo[0])):
        if(abs(pos[1]-1-objetivo[1]) <= abs(pos[1]+1-objetivo[1])):
            visitarNodoEnOrden(matriz,dimension,queue,visitados,[[pos[0]-1,pos[1]],[pos[0],pos[1]-1],[pos[0]+1,pos[1]],[pos[0],pos[1]+1]])
        else:
            visitarNodoEnOrden(matriz,dimension,queue,visitados,[[pos[0]-1,pos[1]],[pos[0],pos[1]+1],[pos[0]+1,pos[1]],[pos[0],pos[1]-1]])
    else:
        if(abs(pos[1]-1-objetivo[1]) <= abs(pos[1]+1-objetivo[1])):
            visitarNodoEnOrden(matriz,dimension,queue,visitados,[[pos[0]+1,pos[1]],[pos[0],pos[1]-1],[pos[0]-1,pos[1]],[pos[0],pos[1]+1]])
        else:
            visitarNodoEnOrden(matriz,dimension,queue,visitados,[[pos[0]+1,pos[1]],[pos[0],pos[1]+1],[pos[0]-1,pos[1]],[pos[0],pos[1]-1]])

# Recibe una matriz de caracteres que representa un laberinto y devuelve el camino desde el inicio al objetivo, o [[-1,-1]] en caso de no encontrarlo
def resolverLaberinto(matriz):
    objetivo=[]
    inicio=obtenerInicio(matriz,objetivo)
    dimension=len(matriz[0])
    encontrado=False
    pos=inicio
    visitados=[inicio]
    queue=[inicio]
    while queue and (not encontrado):
        pos=queue[len(queue)-1]
        if(matriz[pos[0]][pos[1]]=='X'):
            encontrado=True
        else:
            nodoAVisitar(matriz,dimension,queue,pos,visitados,objetivo)

    if(encontrado):
        return queue
    else:
        return [[-1,-1]]

# Recibe una matriz de caracteres y un archivo, en caso de que el laberinto sea resoluble, devuelve el camino desde el inicio hasta el objetivo,
# en caso contrario, genera un nuevo laberinto hasta que genere uno resoluble, y devuelve la resolucion de este.
def generarCamino(matriz,archivo):
    camino=resolverLaberinto(matriz)
    while (camino==[[-1,-1]]):
        crearLaberinto()
        matriz=leerArchivo(archivo)
        camino=resolverLaberinto(matriz)
    return camino

# Recibe un array de posiciones y las imprime en un archivo 
def imprimirSalida(camino):
    f = open ('salida.txt','w+')
    list=[]
    for pos in camino:
        list.append("(" + str(pos[0]+1) + "," + str(pos[1]+1) + ")")
    list=''.join(list)
    f.write(list)
    f.close()

def testLeerArchivo(laberinto):
    matriz=leerArchivo(laberinto)
    assert(matriz == ['I00', '110', '00X'] )

def testObtenerInicio(laberinto):
    matriz=leerArchivo(laberinto)
    objetivo=[]
    inicio=obtenerInicio(matriz,objetivo)
    assert (inicio==[0,0])

def testVerificar(laberinto):
    pos=[-1,0]
    matriz=leerArchivo(laberinto)
    dimension=3
    visitados=[]
    assert (verificar(matriz, pos, visitados, dimension) == False)
    pos=[0,2]
    assert (verificar(matriz, pos, visitados, dimension) == True)


def testResolverLaberinto(laberinto1, laberinto2):
    matriz1=leerArchivo(laberinto1)
    matriz2=leerArchivo(laberinto2)
    assert(resolverLaberinto(matriz1)==[[0, 0], [1, 0], [2, 0], [2, 1], [2, 2]])
    assert(resolverLaberinto(matriz2)==[[-1,-1]])

def testGenerarCamino(laberinto1):
    matriz=leerArchivo(laberinto1)
    assert(generarCamino(matriz,laberinto1) == [[0, 0], [1, 0], [2, 0], [2, 1], [2, 2]])


def tests(laberinto1, laberinto2):
    testLeerArchivo(laberinto1)
    testObtenerInicio(laberinto1)
    testVerificar(laberinto1)
    testResolverLaberinto(laberinto1, laberinto2)
    testGenerarCamino(laberinto1)

def main():
    archivoEntrada=sys.argv[3]
    crearLaberinto()
    matriz=leerArchivo(archivoEntrada)
    camino=generarCamino(matriz,archivoEntrada)
    imprimirSalida(camino)
    tests("./testPython/laberinto1.txt","./testPython/laberinto2.txt")

main()

