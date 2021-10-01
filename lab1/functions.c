#include "functions.h"

/*
Entradas: Nombre de la imagen, dimensiones que reciben por consola
Procesamiento: Se lee la imagen con fread y se almacena la data en un arreglo
Salida: Arreglo con información de la imagen en enteros
*/
int* readFile(char* fileName, int M, int N){

   int f1 = open(fileName,O_RDONLY);
   
   if(f1==-1){
       printf("Error al abrir archivo\n");
       exit(-1);
   }
   int imageDim = M*N;
   int* image = (int*)malloc(sizeof(int)*imageDim);
   read(f1,image,sizeof(int)*imageDim);
   close(f1);


    return image;
}

/*
Entradas: arreglo con la imagen, dimensiones M Y N de la imagen
Procesamiento: se toma el arreglo de la imagen y se convierte a una matriz
Salida: Matriz con los pixeles de la imagen

*/
int** imageToMatrix(int* image, int M, int N){
    int** imageOut= (int**)malloc(sizeof(int*)*M);
    int index = 0;
    for(int i = 0; i < M; i++ ){
        imageOut[i] = (int*)malloc(sizeof(int)*N);
        for (int j = 0; j < N; j++)
        {
            imageOut[i][j] = image[index];
            index++;
        }
        
    }
    return imageOut;
}

/*
Entradas: matriz imagen, dimensiones T y R de Hough, nombre de archivo de salida
Procesamiento: escribe la matriz de hough 
Salida: No hay salidas
*/
void writeOut(int** image, int T, int R, char* fileName){

   int f2 = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
   if(f2==-1){
       printf("Error con el archivo de salida\n");
       exit(-1);
   } 
   int imageDim = T*R;
    int* outImage = (int*)malloc(sizeof(int)*imageDim);
    int index = 0;
    for(int i = 0; i < T ; i++){
        for(int j = 0 ; j < R; j++){
            outImage[index] = image[i][j];
            index++;
        }
    }

    write(f2,outImage,sizeof(int)*imageDim);
    close(f2);

    free(outImage);
}

/*
Entradas: delta Theta, cantidad de angulos M
Procesamiento: crea los distintos angulos con la discretización del espacio
Salida: arreglo flotante con los angulos a evaluar
*/
float* getAngles(float deltaTheta,int M){
    float* angles = (float*)malloc(sizeof(float)*M);
    for(int i = 0; i < M; i++){
        angles[i] = i* deltaTheta;
    }
    return angles;
}

/*
Entradas: matriz hough, dimensiones T y R de Hough, Umbral U
Procesamiento: Aplica la umbralización para la matriz de hough dejando solo pixeles blancos y negros
Salida: Matriz de hough con umbralización
*/
int** umbralization(int** houghMatrix, int T, int R, int U,float deltaTheta, float deltaR){

    for(int i = 0; i < T; i++){
        for(int j = 0; j < R; j++){
            if(houghMatrix[i][j]>U){
                houghMatrix[i][j] = 255;
                float angle =  (i * deltaTheta) * 180 / M_PI;
                float distance = j * deltaR;
                printf("Distancia = %f; Angulo = %f\n", distance, angle);
            }
            
            else{
                houghMatrix[i][j] = 0;
            }
        }
    }
    return houghMatrix;
}

/*
Entradas: dimensiones M y N
Procesamiento: crea una matriz con las dimensiones especificadas
Salida: matriz de hough
*/
int** houghMatrix(int M, int N){
    int** houghMatrix = (int**)malloc(sizeof(int*)*M);
    for(int i = 0; i < M; i++){
        houghMatrix[i] = (int*)malloc(sizeof(int)*N);
        for(int j =0; j<N;j++){
            houghMatrix[i][j] = 0;
        }
    }
    return houghMatrix;
}