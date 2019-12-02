import subprocess
import sys

def crearLaberinto():
    response = subprocess.run(["./a.exe", "example.txt"])
    if(response.returncode!=0):
        print("La creacion del laberinto fallo, el programa va a cerrarse")
        sys.exit()


def leerArchivo(archivo):
    f = open (archivo,'r')
    matriz = f.read()
    f.close()
    matriz=matriz.split()
    return matriz


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

def verificar(matriz, pos , visitados,dimension):
    if (pos[0]<0 or dimension<=pos[0] or dimension<=pos[1] or pos[1]<0):
        return False
    return not(matriz[pos[1]][pos[0]]=='1' or ([pos[0],pos[1]] in visitados))

#Esta funcion siempre devuelve true, y esta hecha para acotar el codigo repetido
# pero a la vez lo hace mas confuso, la dejo o la saco?
def visitado(visitados, queue,pos):
    visitados.append(pos)
    queue.append(pos)

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



def nodoAVisitar(matriz,dimension,queue,pos,visitados,objetivo):
    #print(pos)
    if(abs(pos[0]-1-objetivo[0]) <= abs(pos[0]+1-objetivo[0])):
        if(abs(pos[1]-1-objetivo[1]) <= abs(pos[1]+1-objetivo[1])):
            print((pos[0]-1-objetivo[0]) , (pos[0]+1-objetivo[0]))
            visitarNodoEnOrden(matriz,dimension,queue,visitados,[[pos[0]-1,pos[1]],[pos[0],pos[1]-1],[pos[0]+1,pos[1]],[pos[0],pos[1]+1]])
        else:
            visitarNodoEnOrden(matriz,dimension,queue,visitados,[[pos[0]-1,pos[1]],[pos[0],pos[1]+1],[pos[0]+1,pos[1]],[pos[0],pos[1]-1]])
    else:
        if(abs(pos[1]-1-objetivo[1]) <= abs(pos[1]+1-objetivo[1])):
            visitarNodoEnOrden(matriz,dimension,queue,visitados,[[pos[0]+1,pos[1]],[pos[0],pos[1]-1],[pos[0]-1,pos[1]],[pos[0],pos[1]+1]])
        else:
            visitarNodoEnOrden(matriz,dimension,queue,visitados,[[pos[0]+1,pos[1]],[pos[0],pos[1]+1],[pos[0]-1,pos[1]],[pos[0],pos[1]-1]])

    


    
def resolverLaberinto(matriz):
    objetivo=[]
    inicio=obtenerInicio(matriz,objetivo)
    dimension=len(matriz[0])
    encontrado=False

    pos=inicio
    camino=[]
    visitados=[inicio]
    queue=[inicio]

    while (len(queue)!=0) and (not encontrado):
        
        pos=queue[len(queue)-1]
        nodoAVisitar(matriz,dimension,queue,pos,visitados,objetivo)
        print("len queue ",(len(queue)!=0) and (not encontrado))
        if(matriz[pos[1]][pos[0]]=='X'):
            encontrado=True
            queue.pop()
            camino=queue.copy()
        print(queue)

    if(encontrado):
        return camino
    else:
        return [[-1,-1]]

def generarCamino(matriz,archivo):
    camino=resolverLaberinto(matriz)
    while (camino==[[-1,-1]]):
        crearLaberinto()
        matriz=leerArchivo(archivo)
        camino=resolverLaberinto(matriz)
    return camino

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
    assert(matriz == ['I0111', '10001', '00100', 'X0000', '00000'] )

def testObtenerInicio(laberinto):
    f=open(laberinto)
    objetivo=[]
    inicio=obtenerInicio(laberinto,objetivo)
    f.close()
    assert inicio,[0, 0]

def tests(laberinto):
    testLeerArchivo(laberinto)
    #testObtenerInicio(laberinto)

def main():
    #esto esta hardcodeado, cambiar para pasarlo como param al main
    archivo="laberinto.txt"
    #########

    crearLaberinto()
    matriz=leerArchivo(archivo)
    camino=generarCamino(matriz,archivo)
    imprimirSalida(camino)

    #print(sys.argv[])
    #print(len(sys.argv[]))

main()

