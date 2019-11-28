#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/*///////////////////////////
Consultas:
Esta bien que pese tanto el archivo

Con dimension 1000 y obst aleatorios 10.000 se rellenan las puntas, y no el centro de la matriz

Segmentation fault con dimension cercana a 7.000 (cuando usas los randoms)
el error esta en generarListaCoordenadasLibres.


segmentation fault con dimension mayor a 20.000 (teniendo los randoms comentados)

///////////////////////////*/
//Recibe un array de punteros a char, su dimension y libera la memoria asignada a estos
void liberarMemoriaChar(char **matriz, int dimensionMatriz){
    int i;
    for(i=0;i<dimensionMatriz;i++){
        free(matriz[i]);
    }
    free(matriz);
}
//Recibe un array de punteros a int, su dimension y libera la memoria asignada a estos
void liberarMemoriaInt(int **arrayInt, int dimensionMatriz){
    int i;
    for(i=0;i<dimensionMatriz;i++){
        free(arrayInt[i]);
    }
    free(arrayInt);
}
//Recibe una matriz de caracteres, una posicion y la dimension de la matriz y devuelve 1 si la posicion es ubicable en la matriz,
// o un mensaje de error en caso de no ser ubicable
int esUbicable(char **matriz, int pos[],int dimension){
    if( (0 <= pos[0]-1) && (pos[0]-1 < dimension) && (0 <= pos[1]-1) && (pos[1]-1 < dimension) && matriz[pos[0]-1][pos[1]-1]=='0'){
        return 1;
    }else{
        printf("Se intento acceder a un lugar que estaba ocuapdo o fuera de rango, por lo que el programa va a cerrarse.");
        exit (-1);
    }
}
// Funcion de debugeo (BORRAR)
void mostrarMatriz(char **matriz,int dimension){
    int i, j;
    for(i=0;i<dimension;i++){
        for(j=0;j<dimension;j++){
            printf("%c",matriz[i][j]);
        }
        printf("\n");
    }
}
//Recibe una matriz de caracteres, su dimension, un puntero a int y devuelve una lista con las coordenadas de la matriz en las
// que habia un '0'. Tambien modifica el puntero a int, el cual pasa a contener el largo de la lista que a devolver.
int **generarListaCoordenadasLibres(char **matriz, int dimension, long long *contadorCoordenadasLibres, int casillasOcupadas){
    int i,j,**listaCoordenadasLibres;
    long long tamanioMax=(dimension*dimension)-casillasOcupadas;
    listaCoordenadasLibres=(int**)malloc(sizeof(int*)*tamanioMax);
    for(i=0;i<dimension;i++){
        for(j=0;j<dimension;j++){
            if(matriz[i][j]=='0'){
                listaCoordenadasLibres[(*contadorCoordenadasLibres)]=(int*)malloc(sizeof(int)*2); 
                if(listaCoordenadasLibres[(*contadorCoordenadasLibres)]==NULL){
                    printf("Se rompe cuando el iterador es igual a: %I64d",(*contadorCoordenadasLibres));
                }

                listaCoordenadasLibres[(*contadorCoordenadasLibres)][0]=j;
                listaCoordenadasLibres[(*contadorCoordenadasLibres)][1]=i;
                (*contadorCoordenadasLibres)++;
            }
        }
        //Tira segmentation fault con dimension cercana o mayor a 7000 
        //printf("linea: %i  (%d,%d)\n",i,listaCoordenadasLibres[(*contadorCoordenadasLibres)-1][0],listaCoordenadasLibres[(*contadorCoordenadasLibres)-1][1]);
    }
    return listaCoordenadasLibres;
}
// Coloca '1' en lugares aleatorios libres de la matriz (cantObstAleatorios) veces, siendo cantObstAleatorios un dato ingresado al programa
void generarParedesRandom(char **matriz, int dimension, int cantObstAleatorios, int casillasOcupadas){
    int i,random,**listaCoordenadasLibres;
    long long *contadorCoordenadasLibres,cont=0;
    contadorCoordenadasLibres=&cont;
    srand(time(NULL));
    listaCoordenadasLibres=generarListaCoordenadasLibres(matriz, dimension, contadorCoordenadasLibres, casillasOcupadas);
    int contCopy=cont;
    for(i=0;i<cantObstAleatorios;i++){
        random=(rand()%cont);
        matriz[listaCoordenadasLibres[random][1]][listaCoordenadasLibres[random][0]]='1';
        listaCoordenadasLibres[random][0]=listaCoordenadasLibres[cont-1][0];
        listaCoordenadasLibres[random][1]=listaCoordenadasLibres[cont-1][1];
        cont--;
    }
    liberarMemoriaInt(listaCoordenadasLibres,contCopy);
}

char **crearMatriz(int dimension){
    int i;
    char **matriz=malloc(sizeof(char*)*dimension);
    for(i=0;i<dimension;i++){
        matriz[i]=malloc(sizeof(char)*dimension+1);
        memset(matriz[i],'0',(dimension*sizeof(char))+1);
        matriz[i][dimension]='\0';
        }
    return matriz;
}

//Recibe un puntero al archivo de entrada y un puntero a la direccion de la matriz, parsea los datos del archivo y
// devuelve el laberinto en forma de matriz de caracteres
char **generarLaberinto(FILE *fp){
    int dimension, i, cantObstAleatorios,pos[2],casillasOcupadas=2;
    char buf[1010],**matriz;
    fscanf(fp,"%*s %d %*s %*s",&dimension);
    matriz=crearMatriz(dimension);
    fscanf(fp, "%s", buf);
    for(i=0;strcmp(buf,"obstaculos")!=0;i++){
        sscanf(buf, "(%d,%d)",&pos[0],&pos[1]);
        if(esUbicable(matriz,pos,dimension)){
            casillasOcupadas++;
            matriz[pos[1]-1][pos[0]-1]='1';
        }
        fscanf(fp, "%s", buf);  
    } 
    fscanf(fp, "%*s %d", &cantObstAleatorios);

    fscanf(fp, "%*s %*s (%d,%d)" ,&pos[0],&pos[1]);
    if(esUbicable(matriz,pos,dimension))
        matriz[pos[1]-1][pos[0]-1]='I';
    
    fscanf(fp, "%*s (%d,%d)", &pos[0], &pos[1]);
    if(esUbicable(matriz,pos,dimension))
        matriz[pos[1]-1][pos[0]-1]='X';
    generarParedesRandom(matriz, dimension, cantObstAleatorios, casillasOcupadas);
    return matriz;
}
//Recibe un puntero al archivo a escribir, una matriz y su dimension, y escribe en este archivo la matriz de caracteres
void escribirOutput(FILE *fp, char **matriz, int dimensionMatriz){
    int i=0;
    for(i=0;i<dimensionMatriz;i++){
        fprintf(fp, "%s\n", matriz[i]);
    }
}
//Recibe los argumentos del main y devuelve un msje de error si no se le pasa el archivo de entrada al programa
void ingresoDeArchivos(int argc, char *argv[]){
    if (argc!=2){
        printf("Por favor ingrese un archivo de entrada valido");
        exit(-1);
    }
}

int main(int argc, char *argv[]){
    int dimensionMatriz;
    char **matriz;
    FILE *fp;

    ingresoDeArchivos(argc,argv);

    fp = fopen( "example.txt", "r");
    matriz=generarLaberinto(fp);
    dimensionMatriz=strlen(matriz[0]);
    fclose(fp);

    fp = fopen("laberinto.txt", "w+");
    escribirOutput(fp, matriz, dimensionMatriz);
    fclose(fp);

    liberarMemoriaChar(matriz, dimensionMatriz);
}

