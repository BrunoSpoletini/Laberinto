def crearTablero(n):
    tablero = [None] * n
    for i in range(n):
        tablero[i]= [0]*n
    return tablero

def imprimirTablero(tablero):
    length=len(tablero)
    for i in range(length):
        for j in range(length):
            print(tablero[i][j], end="")
        print("")


def siguienteNodo(tablero, posicion):
    #La idea es: comprobar los 4 ejes, si son el objetivo, 1, 0 o fuera de rango, y ejecutar la fun en el menor de esos puntos +1?
    
    #if ()

def main():
    n=7
    tablero=crearTablero(n)
    imprimirTablero(tablero)

main()