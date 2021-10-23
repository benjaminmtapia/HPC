#include "functions.h"
int D =0;

__global__ void histgmem(unsigned short int* I, int M, int N, int* H){
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    
    
    int npixels = M*N;
    int nthreads = gridDim.x * blockDim.x;
    if(nthreads<npixels){
        //some threads will add twice to the global histogram
        atomicAdd(&(H[I[i]]),1);
        //we calculate the left threads
        int left = npixels - nthreads;
        int newIndex = left - i;
        if(newIndex>=npixels){
            atomicAdd(&(H[I[i]]),1);
        }
    }
    else if(nthreads>npixels){
        if(i<npixels){
            atomicAdd(&(H[I[i]]),1);
        }
    }
    else{
        atomicAdd(&(H[I[i]]),1);
    }
    __syncthreads();


}
__global__ void histogram(unsigned short int* I, int M, int N, int* H){
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    atomicAdd(&(H[I[i]]),1);
    __syncthreads();


}
__global__ void histsmem(unsigned short int* I, int M, int N, int* H){
    int T = gridDim.x * blockDim.x; //blocksize
    int i = threadIdx.x; //local id
    int j = blockIdx.x*T + i; //global id
    __shared__ int hist[256];

    __syncthreads();
    if(j<M*N)
    atomicAdd(&(hist[I[j]]),1);
    __syncthreads();
    //add each local hist to global histogram, but only thread 0 will do this per block
    if(threadIdx.x == 0){
        for(int i = 0; i <=255;i++){
            atomicAdd(&(H[i]),hist[i]);
        }
    }
    

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
    
    for(int i = 0; i<=255;i++){
        H[i]=0;
    }
    //device variables
    unsigned short int* image_d;
    int *hist_d;
    //memory allocation and copy into device memory
    cudaMalloc(&image_d, imageSize*sizeof(unsigned short int));
    cudaMalloc(&hist_d, 256*sizeof(int));
    cudaMemcpy(image_d,image,imageSize*sizeof(unsigned short int), cudaMemcpyHostToDevice);
    cudaMemcpy(hist_d,H,256*sizeof(int), cudaMemcpyHostToDevice);
    int gridSize = (int)ceil(imageSize/blockSize);
    histogram<<<gridSize, blockSize>>>(image_d, M, N, hist_d);
    cudaDeviceSynchronize();
    int * histo = (int*)malloc(sizeof(int)*256);
    cudaMemcpy(H,hist_d,256*sizeof(int),cudaMemcpyDeviceToHost);

    //histogramming(image,M,N,H);

   writeFile(fileOut,H,256);
}