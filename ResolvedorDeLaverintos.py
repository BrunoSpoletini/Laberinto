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


def obtenerInicio(matriz):
    dimension=len(matriz[0])
    inicio=[]
    for i in range(dimension):
        for j in range(dimension):
            if(matriz[i][j]=='I'):
                inicio.append(j)
                inicio.append(i)
    return inicio

def verificar(matriz, x, y , visitados):
    dimension=len(matriz[0])
    if (x<0 or dimension<=x or dimension<=y or y<0):
        return False
    return not(matriz[y][x]=='1' or ([x,y] in visitados))

#Esta funcion siempre devuelve true, y esta hecha para acotar el codigo repetido
# pero a la vez lo hace mas confuso, la dejo o la saco?
def visitado(visitados, queue, x, y):
    visitados.append([x,y])
    queue.append([x,y])
    
def resolverLaberinto(matriz):
    inicio=obtenerInicio(matriz)

    encontrado=False
    pos=inicio
    camino=[]
    visitados=[inicio]
    queue=[inicio] 
    while queue and (not encontrado):
        pos=queue[len(queue)-1]
        if(verificar(matriz,pos[0],pos[1]-1,visitados)):
            visitado(visitados, queue, pos[0], pos[1]-1)
        elif(verificar(matriz, pos[0]+1,pos[1],visitados)):
            visitado(visitados, queue, pos[0]+1, pos[1])
        elif(verificar(matriz, pos[0],pos[1]+1,visitados)):
            visitado(visitados, queue, pos[0], pos[1]+1)
        elif(verificar(matriz, pos[0]-1,pos[1],visitados)):
            visitado(visitados, queue, pos[0]-1, pos[1]) 
        else:
            if(matriz[pos[1]][pos[0]]=='X'):
                encontrado=True
                camino=queue.copy()
            queue.pop()
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
    inicio=obtenerInicio(laberinto)
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
    #ests("./testPython/laberinto1.txt")


main()

