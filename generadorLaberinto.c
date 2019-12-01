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

//Recibe un array de punteros a char, una posicion, la dimension de la matriz, una bandera que depende 
// de la cantidad de randoms y la dimension, y devuelve 1 si la posicion es ubicable en la matriz,
// o un mensaje de error en caso de no ser ubicable
int esUbicable(char **matriz, int pos[],int dimension, int matrizDeUnos){
    char obstaculosFijos;
    if(matrizDeUnos){
        obstaculosFijos='2';
    }else{
        obstaculosFijos='1';
    }
    if( (0 <= pos[0]-1) && (pos[0]-1 < dimension) && (0 <= pos[1]-1) && (pos[1]-1 < dimension) && matriz[pos[1]-1][pos[0]-1]!='I' && matriz[pos[1]-1][pos[0]-1]!=obstaculosFijos){
        return 1;
    }else{
        printf("Se intento acceder a un lugar que estaba ocuapdo o fuera de rango, por lo que el programa va a cerrarse.");
        liberarMemoriaChar(matriz,dimension);
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

// Recibe la dimension, y la cantidad de obstaculos aleatorios y devuelve un array de punteros a char seteado en 0 o 1
char **crearMatriz(int dimension,int cantObstAleatorios,int matrizDeUnos){
    int i;
    char fill;
    if(!matrizDeUnos){
        fill='0';
    }else{
        fill='1';
    }
    char **matriz=malloc(sizeof(char*)*dimension);
    for(i=0;i<dimension;i++){
        matriz[i]=malloc(sizeof(char)*dimension+1);
        memset(matriz[i],fill,(dimension*sizeof(char))+1);
        matriz[i][dimension]='\0';
        }
    return matriz;
}
//Recibe un array de punteros a char, cantidad de obstaculos aleatorios, una bandera, y la cantidad de lugares ocupados
// del array de punteros a char, y ubica en el array la cantidad de obstaculos aleatorios indicada
void ubicarRandoms(char **matriz, int dimension,int cantObstAleatorios,int matrizDeUnos,int lugaresOcupados){
    int rand1,rand2,i;
    char lugarVacio,rellenar;
    srand(time(NULL));
    if(matrizDeUnos){
        lugarVacio='1';
        rellenar='0';
        cantObstAleatorios=(dimension*dimension)-(cantObstAleatorios+lugaresOcupados);
    }else{
        lugarVacio='0';
        rellenar='1';
    }
    for(i=0;i<cantObstAleatorios;i++){
        rand1=(rand()%dimension);
        rand2=(rand()%dimension);
        if(matriz[rand1][rand2]==lugarVacio){
            matriz[rand1][rand2]=rellenar;
        }else{
            i--;
        }
    }
}

//Recibe un puntero al archivo de entrada y un puntero a la direccion del array de caracteres,
// parsea los datos del archivo y devuelve el laberinto en forma de matriz de caracteres
char **generarLaberinto(FILE *fp, int *matrizDeUnos){
    int dimension, i, cantObstAleatorios,pos[2],lugaresOcupados=2;
    char buf[1010],**matriz,obstaculosFijos;


    // Ingreso de dimension y cantAleatorios
    fscanf(fp,"dimension%*c %d %*s %*s",&dimension);
    fscanf(fp, "%s", buf);
    for(i=0;strcmp(buf,"obstaculos")!=0;i++){
        fscanf(fp, "%s", buf);  
    }
    fscanf(fp, "%*s %d", &cantObstAleatorios);
    rewind(fp);
    if(cantObstAleatorios <= ((dimension*dimension)/2)){
        (*matrizDeUnos)=0;
        obstaculosFijos='1';
    }else{
        (*matrizDeUnos)=1;
        obstaculosFijos='2';
    }
    matriz=crearMatriz(dimension,cantObstAleatorios,*matrizDeUnos);

    // Ingreso de obst fijos, inicio y fin
    fscanf(fp,"%*s %*d %*s %*s");
    fscanf(fp, "%s", buf);
    for(i=0;strcmp(buf,"obstaculos")!=0;i++){
        sscanf(buf, "(%d,%d)",&pos[0],&pos[1]);
        if(esUbicable(matriz,pos,dimension,*matrizDeUnos)){
            matriz[pos[1]-1][pos[0]-1]=obstaculosFijos;
            lugaresOcupados++;
        }
        fscanf(fp, "%s", buf);  
    }
    fscanf(fp, "%*s %*d posicion inicial (%d,%d)" ,&pos[0],&pos[1]);
    if(esUbicable(matriz,pos,dimension,*matrizDeUnos))
        matriz[pos[1]-1][pos[0]-1]='I';
    fscanf(fp, "%*s (%d,%d)", &pos[0], &pos[1]);
    if(esUbicable(matriz,pos,dimension,*matrizDeUnos))
        matriz[pos[1]-1][pos[0]-1]='X';

    ubicarRandoms(matriz,dimension,cantObstAleatorios,*matrizDeUnos,lugaresOcupados);
    return matriz;
}
//Recibe un puntero al archivo a escribir, un array de punteros a char y su dimension, y escribe en este archivo la el contenido del array
void escribirOutput(FILE *fp, char **matriz, int dimensionMatriz, int matrizDeUnos){
    int i,j;
    if(matrizDeUnos){
        for(i=0;i<dimensionMatriz;i++){
            for(j=0;j<dimensionMatriz;j++){
                if(matriz[i][j]=='2'){
                    matriz[i][j]='1';
                }
            }
        fprintf(fp, "%s\n", matriz[i]);
        }
    }
    else{
        for(i=0;i<dimensionMatriz;i++){
            fprintf(fp, "%s\n", matriz[i]);
        }
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
    int dimensionMatriz,*matrizDeUnos,flag;
    char **matriz;
    matrizDeUnos=&flag;
    FILE *fp;
    ingresoDeArchivos(argc,argv);

    fp = fopen( "example.txt", "r");
    matriz=generarLaberinto(fp,matrizDeUnos);
    dimensionMatriz=strlen(matriz[0]);
    fclose(fp);
    
    fp = fopen("laberinto.txt", "w+");
    escribirOutput(fp, matriz, dimensionMatriz,flag);
    fclose(fp);

    liberarMemoriaChar(matriz, dimensionMatriz);
}
