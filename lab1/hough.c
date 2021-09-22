#include "functions.h"
#include <math.h>
#include <immintrin.h>

# define MPI 3.14159265358979323846


float* getDistances(float deltaR,int R){
    float* distances = (float*)malloc(sizeof(float)*R);
    for(int i = 0; i < R; i++){
        distances[i] = i* deltaR;
    }
    return distances;
}
int* getAngles(float deltaTheta,int M){
    int* angles = (int*)malloc(sizeof(int)*M);
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

int** sequentialHough (int** image,int M, int N, int T, int R, int U){

    int** houghMatrix = (int**)malloc(sizeof(int*)*T);
    for(int k = 0; k < T; k++){
        houghMatrix[k] = (int*)malloc(sizeof(int)*R);
        for(int l =0; l<R;l++){
            houghMatrix[k][l] = 0;
        }
    }
    float deltaTheta = 180/M;
    float deltaR = (N*sqrt(2))/(2*R);
    float* distances = getDistances(deltaR,R);
    int* angles = getAngles(deltaTheta,M);

    int index = 0;
    float theta =0;
    float r = 0;
    for(int i = 0 ; i < M; i++){
        for(int j = 0; j < N; j++){
            //if is edge
            if(image[i][j]!=0){
                //for each theta

                for(int k = 0; k < T; k++){

                    theta = (angles[k]*MPI)/180;
                    r = i*cos(theta) + j * sin(theta);
                    if(r>=0){
                    index = getRindex(distances, R, r);
                    }
                    else{
                    index = getRindex(distances, R, -1*r); 
                    }
                       houghMatrix[k][index] = houghMatrix[k][index]+1;
                    
                }
            }
        }
    }

    //houghMatrix = umbralization(houghMatrix,T,R,U);
    return houghMatrix;

}

void loadRegisters(int i, int j, __m128* posX, __m128* posY ){

    float a[4] __attribute__((aligned(16))) = {i, i, i, i };
    float b[4] __attribute__((aligned(16))) = {j, j, j, j};

    *posX = _mm_load_ps(a);
    *posY = _mm_load_ps(b);
}

/*

int** parallelHough(int** image, int N, int M, int R, int U){
    
    __m128 R; registros de 4 flotantes para los angulos
    usar _mm_load_ps


    usar _mm_set_ps

    _mm_add_ps(__m128 a, __m128 b); para sumar
    _mm_cos_ps(__m128 x);
    _mm_sin_ps(__mm128x);

    registros necesarios: para los i, para los j, para los r, y posiblemente theta (constante)

    
    int** houghMatrix = (int**)malloc(sizeof(int*)*M);
    for(int k = 0; k < M; k++){
        houghMatrix[k] = (int*)malloc(sizeof(int)*R);
        for(int l =0; l<R;l++){
            houghMatrix[k][l] = 0;
        }
    }
    float deltaTheta = 180/M;
    //float deltaR = N*sqrt(2)/(2*R);
    //int* distances = getDistances(deltaR,R);
    int* angles = getAngles(deltaTheta,M);
    
    __m128 posX, posY;//thetas;//,mcos,msin ;//,icos,jsin,r;
    __m512 thetas,mcos,msin;

    for(int i = 0 ; i < N; i++){
        for(int j = 0; j < N; j++){
 

            //if is edge
            if(image[i][j]!=0){
                //for each theta
                for(int k = 0; k < M; k+=4){
                    loadRegisters(i,j,&posX,&posY);
                    thetas = _mm512_load_ps(angles[k]);
                    //mcos = _mask (thetas);

                    
                    posX = _mm_load1_ps(i);
                    posY = _mm_load1_ps(j);
                    theta = _mm_load_ps(angles[k]);
                    _cos = _mm_cos_ps(theta)
                    _sin = _mm_sin_ps(theta)
                    icos = _mm_mul_ps(posX,_cos)
                    jsin = _mm_mul_ps(posY,_sin)
                    r = _mm_sum_ps(icos,jsin)


                       //houghMatrix[k][r] = houghMatrix[k][r]+1;
                    
                }
            }
        }
    }

    houghMatrix = umbralization(houghMatrix,M,R,U);
    return houghMatrix;
}

*/
int main(int argc, char** argv){

    int N;
    char* Infile;
    char* Outfile;
    int M;
    int T;
    int R;
    int U;
    int c;

    while (( (c = getopt(argc, argv, "i:o:M:N:T:R:U:")) != -1)){
        switch (c)
        {
        case 'i':
            Infile = optarg;
            break;

        case 'o':
            Outfile = optarg;
            break;

        case 'M':
            M = atof(optarg);
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
    image =readFile(Infile, M ,N);
    int ** imageMatrix;
    imageMatrix = imageToMatrix(image,M, N);
    
   int** seqHough;
   seqHough = sequentialHough(imageMatrix,M,N,T,R,U);
   writeOut(seqHough,T,R,Outfile);

    return 0;

}