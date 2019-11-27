#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

void liberarMemoriaChar(char **matriz, int *dimensionMatriz){
    int i;
    for(i=0;i<(*dimensionMatriz);i++){
        free(matriz[i]);
    }
    free(matriz);
}

void liberarMemoriaInt(int **arrayInt, int dimensionMatriz){
    int i;
    for(i=0;i<dimensionMatriz;i++){
        free(arrayInt[i]);
    }
    free(arrayInt);
}

int esUbicable(char **matriz, int pos[],int dimension){
    if( (0 <= pos[0]-1) && (pos[0]-1 < dimension) && (0 <= pos[1]-1) && (pos[1]-1 < dimension) && matriz[pos[0]-1][pos[1]-1]=='0'){
        return 1;
    }else{
        printf("Se intento acceder a un lugar que estaba ocuapdo o fuera de rango, por lo que el programa va a cerrarse.");
        exit (-1);
    }
}

void mostrarMatriz(char **matriz,int dimension){
    int i, j;
    for(i=0;i<dimension;i++){
        for(j=0;j<dimension;j++){
            printf("%c",matriz[i][j]);
        }
        printf("\n");
    }
}

int **generarListaCoordenadasLibres(char **matriz, int dimension, int *contadorCoordenadasLibres){
    int i,j,**listaCoordenadasLibres;
    listaCoordenadasLibres=malloc(sizeof(int*)*dimension*dimension);
    for(i=0;i<dimension;i++){
        for(j=0;j<dimension;j++){
            if(matriz[i][j]=='0'){
                listaCoordenadasLibres[(*contadorCoordenadasLibres)]=malloc(sizeof(int)*3); 
                listaCoordenadasLibres[(*contadorCoordenadasLibres)][0]=i;
                listaCoordenadasLibres[(*contadorCoordenadasLibres)][1]=j;
                (*contadorCoordenadasLibres)++;
            }
        }
    }
    return listaCoordenadasLibres;
}

void generarParedesRandom(char **matriz, int dimension, int cantObstAleatorios, int **listaCoordenadasLibres, int *contadorCoordenadasLibres){
    int i,random;
    srand(time(NULL));
    for(i=0;i<cantObstAleatorios;i++){
        random=(rand()%(*contadorCoordenadasLibres));
        matriz[listaCoordenadasLibres[random][1]][listaCoordenadasLibres[random][0]]='1';
        listaCoordenadasLibres[random][0]=listaCoordenadasLibres[(*contadorCoordenadasLibres)-1][0];
        listaCoordenadasLibres[random][1]=listaCoordenadasLibres[(*contadorCoordenadasLibres)-1][1];
        (*contadorCoordenadasLibres)--;
    }
}

char **ubicarCaracteres(int dimension, int **obstaculosF, int cantObstAleatorios, int cantObstFijos, int posIni[],int obj[]){
    int i, *contadorCoordenadasLibres, cont=0, **listaCoordenadasLibres;
    char **matriz;
    contadorCoordenadasLibres=&cont;
    //Seteo de la matriz a '0's
    matriz=malloc((sizeof(char*))*dimension);
    for(i=0;i<dimension;i++){
        matriz[i]=malloc(sizeof(char)*dimension);
        memset(matriz[i],'0',dimension);
        matriz[i][dimension]='\0';}

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
    generarParedesRandom(matriz, dimension, cantObstAleatorios, listaCoordenadasLibres,contadorCoordenadasLibres);

    liberarMemoriaInt(listaCoordenadasLibres,*contadorCoordenadasLibres);
    return matriz;

}

char **generarLaberinto(FILE *fp, int *dimensionMatriz){
    int dimension, largo=0, i, **obstaculosF,cantObstFijos=0, cantObstAleatorios,posIni[2],obj[2],maxObstaculos;
    char buf[1010],**matriz;
    fscanf(fp,"%*s %d %*s %*s",&dimension);
    *dimensionMatriz=dimension;
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

void escribirOutput(FILE *fp, char **matriz, int dimensionMatriz){
    int i=0;
    for(i=0;i<dimensionMatriz;i++){
        fprintf(fp, "%s\n", matriz[i]);
    }
}

void ingresoDeArchivos(int argc, char *argv[]){
    if (argc!=2){
        printf("Por favor ingrese un archivo de entrada valido");
        exit(-1);
    }
}

int main(int argc, char *argv[]){
    int *dimensionMatriz;
    char **matriz;
    FILE *fp;

    ingresoDeArchivos(argc,argv);

    fp = fopen( "example.txt", "r");
    matriz=generarLaberinto(fp,dimensionMatriz);
    fclose(fp);

    fp = fopen("EntradaLaberinto.txt", "w+");
    escribirOutput(fp, matriz, *dimensionMatriz);
    fclose(fp);

    liberarMemoriaChar(matriz, dimensionMatriz);
}

