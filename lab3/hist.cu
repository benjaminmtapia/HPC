#include "functions.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <ctype.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>

int D =0;

__global__ void histgmem(unsigned short int* I, int M, int N, int* H){
    int i = blockDim.x * blockIdx.x + threadIdx.x;    
    int npixels = M*N;
    int nthreads = gridDim.x * blockDim.x;

    
    if(nthreads<npixels){
        //some threads will add twice to the global histogram
        atomicAdd(&(H[I[i]]),1);
        //we calculate the left threads
        int newIndex = nthreads+i+1;
        if(newIndex<npixels){
            atomicAdd(&(H[I[newIndex]]),1);
        }
    }
    else if(nthreads>=npixels){
        if(i<npixels){
            atomicAdd(&(H[I[i]]),1);
        }
    }
    __syncthreads();


}

__global__ void histsmem(unsigned short int* I, int M, int N, int* H, int T){
    int i = threadIdx.x; //local id
    int j = blockDim.x * blockIdx.x + threadIdx.x; //global id
    int npixels = M*N;
    int nthreads = gridDim.x * blockDim.x;
    __shared__ int hist[256];
    for(int i = 0; i<256;i++){
        hist[i]=0;
    }

    __syncthreads();

    if(nthreads>=npixels)
    atomicAdd(&(hist[I[j]]),1);

    else if(nthreads<npixels){
        atomicAdd(&(hist[I[j]]),1);
        int newIndex = nthreads+1+j;
        if(newIndex<npixels){
            atomicAdd(&(hist[I[newIndex]]),1);
        }
    }
    __syncthreads();
    //add each local hist to global histogram, but only thread 0 will do this per block
    if(i==0){
        for(int i = 0;i<256;i++)
        atomicAdd(&(H[i]),hist[i]);
    }
    __syncthreads();

}

__host__ int main(int argc, char** argv){
    char* fileIn;
    int M = 0,N,blockSize;
    char* fileOut;
    int c;
    if(argc<10){
        printf("faltan parámetros\n");
        exit(1);
    }

    while (( (c = getopt(argc, argv, "i:m:n:o:t:d:")) != -1)){
        switch (c)
        {
        case 'i':
            fileIn = optarg;
            break;

        case 'm':
            sscanf(optarg,"%d",&M);
            break;

        case 'n':
            sscanf(optarg,"%d",&N);
            break;

        case 'o':
            fileOut = optarg;
            break;
        
        case 't':
            sscanf(optarg,"%d",&blockSize);
            break;

        case 'd':
            sscanf(optarg,"%d",&D);
            if(D!=0 && D!=1){
                printf("parametros incorrectos para D, solo puede ser 0 o 1\n");
                exit(1);
            }
            break;
        }
    }
   
    unsigned short int *image;
    int imageSize = M*N;
    image = readFile(fileIn,M,N);
    int *H = (int*) malloc(sizeof(int)*256);
    int *H2 = (int*) malloc(sizeof(int)*256);
    
    for(int i = 0; i<=255;i++){
        H[i]=0;
        H2[i]=0;
    }

    //device variables
    unsigned short int* image_d;
    int *hist_d;
    int *hist_d2;

    //time variables
    cudaEvent_t startGmem, startSmem, stopGmem, stopSmem;
    cudaEventCreate(&startGmem);
    cudaEventCreate(&stopGmem);
    cudaEventCreate(&startSmem);
    cudaEventCreate(&stopSmem);
    

    //memory allocation and copy into device memory
    cudaMalloc(&image_d, imageSize*sizeof(unsigned short int));
    cudaMalloc(&hist_d, 256*sizeof(int));
    cudaMalloc(&hist_d2, 256*sizeof(int));

    cudaMemcpy(image_d,image,imageSize*sizeof(unsigned short int), cudaMemcpyHostToDevice);
    cudaMemcpy(hist_d,H,256*sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(hist_d2,H2,256*sizeof(int), cudaMemcpyHostToDevice);

    int gridSize = (int)ceil(imageSize/blockSize);

    
    cudaEventRecord(startGmem);
    histgmem<<<gridSize, blockSize>>>(image_d, M, N, hist_d);
    cudaEventRecord(stopGmem);
    cudaEventSynchronize(stopGmem);


    float timeinmillisecondsGlobal = 0;
    cudaEventElapsedTime(&timeinmillisecondsGlobal, startGmem, stopGmem);


    cudaEventRecord(startSmem);
    histsmem<<<gridSize, blockSize>>>(image_d, M, N, hist_d2,blockSize);
    cudaEventRecord(stopSmem);
    cudaEventSynchronize(stopSmem);
    cudaDeviceSynchronize();

    float timeinmillisecondsShared = 0;
    cudaEventElapsedTime(&timeinmillisecondsShared, startSmem, stopSmem);

    

    cudaMemcpy(H,hist_d,256*sizeof(int),cudaMemcpyDeviceToHost);
    cudaMemcpy(H2,hist_d2,256*sizeof(int),cudaMemcpyDeviceToHost);


    if(D==1)
    printHist(H,H2);

    printf("Global memory time: %f ms\n",timeinmillisecondsGlobal);
    printf("Shared memory time: %f ms\n",timeinmillisecondsShared);

    writeFile(fileOut,H,H2);
}