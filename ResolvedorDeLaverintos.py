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
                inicio.append(i)
                inicio.append(j)
    return inicio

def sigPos(matriz, pos):
    if(matriz[pos[0]][pos[1]]=='X'):
        return pos
   # else if(matriz[pos[0]][pos[1]]==):


def resolverLaberinto(matriz):
    inicio=obtenerInicio(matriz)
    
    #for i in range(matriz):

def main():
    #esto esta hardcodeado, cambiar para pasarlo como param al main
    archivo="laberinto.txt"
    #########

    crearLaberinto()
    lista=leerArchivo(archivo)
    matriz=lista.split()
    print(matriz)
    resolverLaberinto(matriz)




main()