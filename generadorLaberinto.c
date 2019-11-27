#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

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
int **generarListaCoordenadasLibres(char **matriz, int dimension, long long *contadorCoordenadasLibres){
    int i,j,**listaCoordenadasLibres;
    listaCoordenadasLibres=malloc((sizeof(int*)*dimension*dimension));
    for(i=0;i<dimension;i++){
        for(j=0;j<dimension;j++){
            if(matriz[i][j]=='0'){
                listaCoordenadasLibres[(*contadorCoordenadasLibres)]=malloc(sizeof(int)*2); 
                listaCoordenadasLibres[(*contadorCoordenadasLibres)][0]=i;
                listaCoordenadasLibres[(*contadorCoordenadasLibres)][1]=j;
                (*contadorCoordenadasLibres)++;
            }
        } 
        //Tira segmentation fault con dimension cercana o mayor a 10000 
        // debug printf("linea: %i  (%d,%d)\n",i,listaCoordenadasLibres[(*contadorCoordenadasLibres)-1][0],listaCoordenadasLibres[(*contadorCoordenadasLibres)-1][1]);
    }
    return listaCoordenadasLibres;
}
// Coloca '1' en lugares aleatorios libres de la matriz (cantObstAleatorios) veces, siendo cantObstAleatorios un dato ingresado al programa
void generarParedesRandom(char **matriz, int dimension, int cantObstAleatorios, int **listaCoordenadasLibres, long long contadorCoordenadasLibres){
    int i,random;
    srand(time(NULL));
    for(i=0;i<cantObstAleatorios;i++){
        random=(rand()%contadorCoordenadasLibres);
        matriz[listaCoordenadasLibres[random][1]][listaCoordenadasLibres[random][0]]='1';
        listaCoordenadasLibres[random][0]=listaCoordenadasLibres[contadorCoordenadasLibres-1][0];
        listaCoordenadasLibres[random][1]=listaCoordenadasLibres[contadorCoordenadasLibres-1][1];
        contadorCoordenadasLibres--;
    }
}



//Recibe los datos necesarios para crear el laberinto y lo devuelve en forma de matriz de caracteres
char **ubicarCaracteres(int dimension, int **obstaculosF, int cantObstAleatorios, int cantObstFijos, int posIni[],int obj[]){
    int i, **listaCoordenadasLibres;
    long long *contadorCoordenadasLibres,cont=0;
    char **matriz;
    contadorCoordenadasLibres=&cont;

    //Seteo de la matriz a '0's
    matriz=malloc(sizeof(char*)*dimension);
    for(i=0;i<dimension;i++){
        matriz[i]=malloc(sizeof(char)*dimension+1);
        memset(matriz[i],'0',dimension*sizeof(char));
        matriz[i][dimension]='\0';
        }

    // Poscicion inicial y objetivo
    if(esUbicable(matriz,posIni,dimension))
        matriz[posIni[1]-1][posIni[0]-1]='I';
    if(esUbicable(matriz,obj,dimension))
        matriz[obj[1]-1][obj[0]-1]='X';

    // Obstaculos Fijos
    for(i=0; i<cantObstFijos; i++){
       if(esUbicable(matriz,obstaculosF[i],dimension)){
            matriz[obstaculosF[i][1]-1][obstaculosF[i][0]-1]='1';
        }
    }
    //Obstaculos aleatorios
    listaCoordenadasLibres=generarListaCoordenadasLibres(matriz, dimension, contadorCoordenadasLibres);
    generarParedesRandom(matriz, dimension, cantObstAleatorios, listaCoordenadasLibres,*contadorCoordenadasLibres);

    liberarMemoriaInt(listaCoordenadasLibres,*contadorCoordenadasLibres);
    return matriz;

}
//Recibe un puntero al archivo de entrada y un puntero a la direccion de la matriz, parsea los datos del archivo y
// devuelve el laberinto en forma de matriz de caracteres
char **generarLaberinto(FILE *fp){
//void   generarLaberinto(FILE *fp){
    int dimension, i, **obstaculosF,cantObstFijos=0, cantObstAleatorios,posIni[2],obj[2],maxObstaculos;
    char buf[10010],**matriz;
    fscanf(fp,"%*s %d %*s %*s",&dimension);
    maxObstaculos=(dimension*dimension)-2;
    obstaculosF=malloc(sizeof(int*)*maxObstaculos);
    fscanf(fp, "%s", buf);
    for(i=0;strcmp(buf,"obstaculos")!=0;i++){ 
        cantObstFijos++;
        obstaculosF[i]=malloc(sizeof(int)*2);
        sscanf(buf, "(%d,%d)",&obstaculosF[i][0],&obstaculosF[i][1]);
        fscanf(fp, "%s", buf);  
    }
    fscanf(fp, "%*s %s", buf); 
    sscanf(buf,"%d",&cantObstAleatorios);

    fscanf(fp, "%*s %*s %s" ,buf);
    sscanf(buf, "(%d,%d)",&posIni[0],&posIni[1]);

    fscanf(fp, "%*s %s", buf);
    sscanf(buf, "(%d,%d)", &obj[0], &obj[1]);
    matriz=ubicarCaracteres(dimension, obstaculosF, cantObstAleatorios, cantObstFijos, posIni, obj);
    liberarMemoriaInt(obstaculosF,cantObstFijos);
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

    fp = fopen("EntradaLaberinto.txt", "w+");
    escribirOutput(fp, matriz, dimensionMatriz);
    fclose(fp);

    liberarMemoriaChar(matriz, dimensionMatriz);
}

