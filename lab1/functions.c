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

float* getDistances(float deltaR,int R){
    float* distances = (float*)malloc(sizeof(float)*R);
    for(int i = 0; i < R; i++){
        distances[i] = i* deltaR;
    }
    return distances;
}
float* getAngles(float deltaTheta,int M){
    float* angles = (float*)malloc(sizeof(float)*M);
    for(int i = 0; i < M; i++){
        angles[i] = i* deltaTheta;
    }
    return angles;
}

int** umbralization(int** houghMatrix, int M, int R, int U){

    for(int i = 0; i < M; i++){
        for(int j = 0; j < R; j++){
            if(houghMatrix[i][j]>U){
                houghMatrix[i][j] = 255;
            }
            else{
                houghMatrix[i][j] = 0;
            }
        }
    }
    return houghMatrix;
}

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
