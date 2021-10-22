#include "functions.h"

__host__ void histogramming(int* A, int M, int N, int*H){
    int imageSize = M*N;
    for(int i = 0; i<imageSize;i++){
       H[A[i]]++;
       //printf("%d ",A[i]);
    }
    
}

__host__ int *readFile(char* filename, int M, int N){
    int imageSize = M*N;
    int *image = (int*)malloc(sizeof(int)*imageSize);
    FILE *f = fopen(filename,"rb");
    if(f==NULL){
        printf("Error opening file");
        exit(1);
    }
    fread(image,sizeof(int),imageSize,f);
    fclose(f);
   return image; 
}

__host__ void writeFile(char* filename, int* hist, int N){
    FILE* out = fopen(filename,"w");
    for(int i = 0; i<N; i++){
        fprintf(out,"%d\n",hist[i]);
    }
    fclose(out);
}