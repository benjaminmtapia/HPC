# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <ctype.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>

// Device code
__global__ void VecAdd(float* A, float* B, float* C, int N)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < N)
        C[i] = A[i] + B[i];
}
            
// Host code
int main()
{
    int N = 512;
    size_t size = N * sizeof(float);

    // Allocate input vectors h_A and h_B in host memory
    float* h_A = (float*)malloc(sizeof(float)*N);
    float* h_B = (float*)malloc(sizeof(float)*N);
    float* h_C = (float*)malloc(sizeof(float)*N);

    // Initialize input vectors
    for(int i = 0; i < N; i++){
        h_A[i]=cos(i);
        h_B[i]=sin(i);
        h_C[i]=0;
    }

    // Allocate vectors in device memory
    float* d_A;
    cudaMalloc(&d_A, size);
    float* d_B;
    cudaMalloc(&d_B, size);
    float* d_C;
    cudaMalloc(&d_C, size);

    // Copy vectors from host memory to device memory
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Invoke kernel
    int threadsPerBlock = 256;
    int blocksPerGrid =
            (N + threadsPerBlock - 1) / threadsPerBlock;
    VecAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);

    // Copy result from device memory to host memory
    // h_C contains the result in host memory
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    for(int i = 0; i < N; i++){
        printf("%f ",h_C[i]);
    }
    printf("\n");
            
    // Free host memory
return 0;
}