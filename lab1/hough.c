# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <pmmintrin.h>
# include <immintrin.h>
# include <math.h>
# define MPI 3.14159265358979323846

int* readFile(char* fileName, int N){
    FILE* f = fopen(fileName,"r");
    if(f==NULL){
        printf("error al abrir archivo\n");
        exit(1);
    }
    int imageDim = N*N;
    int index = 0;

    int* image = (int*)malloc(sizeof(int)*imageDim);
    fread(image,sizeof(float),imageDim,f);

    fclose(f);
    return image;
}

int** imageToMatrix(int* image, int N){
    int** imageOut= (int**)malloc(sizeof(int)*N);
    int index = 0;
    for(int i = 0; i < N; i++ ){
        imageOut[i] = (int*)malloc(sizeof(int)*N);
        for (int j = 0; j < N; j++)
        {
            imageOut[i][j] = image[index];
            index++;
        }
        
    }
    return imageOut;
}

void writeOut(int** image, int N, char* fileName){
    FILE* fileOut = fopen(fileName,"wb");
    for(int i = 0; i < N; i++ ){
        
        for (int j = 0; j < N; j++)
        {
            int aux = image[i][j];
            fwrite(&aux,sizeof(int),1,fileOut);
        }
        
    }
}

int** sequentialHough (int** image, int N, int M, int R){

    int** houghMatrix = (int**)malloc(sizeof(int*)*M);
    for(int k = 0; k < M; k++){
        houghMatrix[k] = (int*)malloc(sizeof(int)*R);
    }
    float deltaTheta = MPI/M;
    float deltaR = N*(sqrt(2)) / (2*R);


    for(int i = 0 ; i < N; i++){
        for(int j = 0; j < N; j++){
            if(image[i][j]!=0){

            }
        }
    }

}
int main(int argc, char** argv){

    int N;
    char* fileIn;
    char* fileOut;
    int T;
    int R;
    int U;
    int c;

    while (( (c = getopt(argc, argv, "i:o:N:T:R:U:")) != -1)){
        switch (c)
        {
        case 'i':
            fileIn = optarg;
            break;

        case 'o':
            fileOut = optarg;
            break;

        case 'N':
            N = atof(optarg);
            break;

        case 'T':
            T = atof(optarg);
            break;

        case 'R':
            R = atof(optarg);
            break;
        
        case 'U':
            U = atof(optarg);
            break;
        }
        
    }


    int* image;
    image =readFile(fileIn, N);
    int ** imageMatrix;
    imageMatrix = imageToMatrix(image, N);
    writeOut(imageMatrix,N,fileOut);
    /*
    prueba para ver si la imagen fue leida correctamente
    FILE* out = fopen(fileOut,"wb");
    fwrite(image,sizeof(float),N*N,out);

    */
   int imgSize = N*N;
    

   int** seqHough;
   seqHough = sequentialHough(imageMatrix,N,T,R);


    return 0;

}