import subprocess
import sys

def crearLaberinto():
    response = subprocess.run(["./a.exe", "example.txt"])
    if(response.returncode!=0):
        print("La creacion del laberinto fallo, el programa va a cerrarese")
        sys.exit()

#Hacer pytest
def leerArchivo(archivo):
    f = open (archivo,'r')
    matriz = f.read()
    f.close()
    return matriz
#comentar y hacer testeo
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

def resolverLaberinto(matriz):
    inicio=obtenerInicio(matriz)
    flag=True
    encontrado=False
    pos=inicio
    camino=[]
    visitados=[inicio]
    queue=[inicio] 
    while queue:
        if(matriz[pos[1]][pos[0]]=='X'):
            encontrado=True
            camino=queue.copy()
            camino.append(pos)
        pos=queue.pop()
        flag=True
        if(verificar(matriz,pos[0],pos[1]-1,visitados)):
            flag=False
            visitados.append([pos[0],pos[1]-1])
            queue.append(pos)
            queue.append([pos[0],pos[1]-1])

        if(flag and verificar(matriz, pos[0]+1,pos[1],visitados)):
            flag=False
            visitados.append([pos[0]+1,pos[1]])
            queue.append(pos)
            queue.append([pos[0]+1,pos[1]])

        if(flag and verificar(matriz, pos[0],pos[1]+1,visitados)):
            flag=False
            visitados.append([pos[0],pos[1]+1])
            queue.append(pos)
            queue.append([pos[0],pos[1]+1])
        
        if(flag and verificar(matriz, pos[0]-1,pos[1],visitados)):
            flag=False
            visitados.append([pos[0]-1,pos[1]])
            queue.append(pos)
            queue.append([pos[0]-1,pos[1]])  
    if(encontrado):
        return camino
    else:
        return [[-1,-1]]
    

def main():
    #esto esta hardcodeado, cambiar para pasarlo como param al main
    archivo="laberinto.txt"
    #########
    camino=[]
    crearLaberinto()
    lista=leerArchivo(archivo)
    matriz=lista.split()
    camino=resolverLaberinto(matriz)
    
    while (camino==[[-1,-1]]):
        crearLaberinto()
        lista=leerArchivo(archivo)
        matriz=lista.split()
        camino=resolverLaberinto(matriz)
    for pos in camino:
        pos[0]+=1
        pos[1]+=1
    print("camino=",camino)

main()