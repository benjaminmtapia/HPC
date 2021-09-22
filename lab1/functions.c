#include "functions.h"

int* readFile(char* fileName, int M, int N){
    FILE* f = fopen(fileName,"r");
    if(f==NULL){
        printf("error al abrir archivo\n");
        exit(1);
    }
    int imageDim = M*N;
    

    int* image = (int*)malloc(sizeof(int)*imageDim);
    fread(image,sizeof(int),imageDim,f);

    fclose(f);
    return image;
}
void printmatrix(int** matrix, int M, int R){
    for(int i = 0; i < M; i++){
        for(int j = 0 ; j < R; j++){
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}
int** imageToMatrix(int* image, int M, int N){
    int** imageOut= (int**)malloc(sizeof(int)*M);
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

void writeOut(int** image, int M, int R, char* fileName){
    FILE* fileOut = fopen(fileName,"wb");
    //printmatrix(image,M,R);
    for(int i = 0; i < M; i++ ){
        for(int j = 0; j < R; j++){
            int aux = image[i][j] ? 255:0;
            fwrite(&aux,1,sizeof(int),fileOut);    
        }
    }

    //fwrite(out,sizeof(int),M*R,fileOut);
}

int getRindex(float*distances, int R, int r){
    int index = -1;
    for(int i = 0 ; i < R; i++){
        if(r == distances[i]){
            index = i;
            break;
        }
    }
    return index;

}
