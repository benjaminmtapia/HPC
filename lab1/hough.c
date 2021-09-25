#include "functions.h"
#include <math.h>
#include <immintrin.h>

/*
Entradas: matriz con la imagen, matriz de hough a llenar, dimensiones M Y N de la imagen
          dimensiones T y R de la matriz de hough, arreglo flotante con ángulos, delta R
Procesamiento: Se crean los distintos registros necesarios. Al momento de aplicar el algoritmo se utilizan registros con
                las posiciones, con los cosenos y senos, y también para resultados parciales, lo cual se almacena finalmente
                en un registro y con ese indice se accede a la matriz de hough
Salida: Matriz de Hough con los valores resultantes.

*/
int** parallelHough(int** image, int**houghMatrix, int M,int N,int T, int R, float* angles, float deltaR){
    __m128 posX, posY, mcos, msin, icos, jsin, rDelta, rdistance, rindex, rCenter;
    float center = R/2;
    rCenter = _mm_set1_ps(center);

    float* angulos = (float*)malloc(sizeof(float)*T);
    for(int i = 0; i<T; i++){
        angulos[i] = angles[i];
    }
    rDelta = _mm_set1_ps((float) deltaR);
    float* positions = (float*)malloc(sizeof(float)*4);
    int* auxIndex = (int*) malloc(sizeof(int)*4);

    for(int x = 0; x < M ; x++){
        for(int y = 0; y < N; y++){
            //if is edge
            if(image[x][y]!=0){
                float u = (float) x;
                float v = (float) y;
                posX = _mm_set1_ps(u);
                posY = _mm_set1_ps(v);
                //cargar los registros de posicion

                for(int i = 0; i < T; i+=4){
                    mcos = _mm_set_ps(cos(angles[i]),cos(angles[i+1]),cos(angles[i+2]),cos(angles[i+3]));
                    msin = _mm_set_ps(sin(angles[i]),sin(angles[i+1]),sin(angles[i+2]),sin(angles[i+3]));
                    icos = _mm_mul_ps(mcos,posX);
                    jsin = _mm_mul_ps(msin,posY);
                    rdistance = _mm_add_ps(icos,jsin);
                    rindex = _mm_div_ps(rdistance,rDelta);
                    rCenter = _mm_add_ps(rCenter,rindex);

                    _mm_store_ps(positions,rindex);

                   
                    auxIndex[0] = (int)positions[0];
                    auxIndex[1] = (int)positions[1];
                    auxIndex[2] = (int)positions[2];
                    auxIndex[3] = (int)positions[3];
                    // printf("%d %d %d %d", auxIndex[0],auxIndex[1],auxIndex[2], auxIndex[3]);
                    
                    houghMatrix[i][auxIndex[0]]+=1;
                    houghMatrix[i+1][auxIndex[1]]+=1;
                    houghMatrix[i+2][auxIndex[2]]+=1;
                    houghMatrix[i+3][auxIndex[3]]+=1;
                    
                }
            }
        }

    }

    return houghMatrix;
}


/*
Entradas: matriz con la imagen, matriz de hough a llenar, dimensiones M Y N de la imagen
          dimensiones T y R de la matriz de hough, arreglo flotante con ángulos, delta R
Procesamiento: Aplica el algoritmo de hough de manera secuencial
Salida: Matriz de Hough con los valores resultantes.

*/
int** sequentialHough (int** image, int**houghMatrix, int M, int N,int T,int R, float* angles, float deltaR){
    for(int x = 0; x < M ; x++){
        for(int y = 0; y < N; y++){
            //if is edge
            if(image[x][y]!=0){
                //for each theta
                for(int i = 0; i < T; i++){
                    float theta = angles[i];
                    float r = x* cos(theta) + y* sin(theta);
                    int j = (r/deltaR);
                    //printf("%d ", j);
                    houghMatrix[i][j]+=1;
                }
                //printf("\n");
            }
        }
    }
    return houghMatrix;

}


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
    
    float deltaTheta = M_PI/(T);
    float deltaR = sqrt(N*N + M*M)/(R);
    float* angles = getAngles(deltaTheta,T);

    int** seqHough;
    int** parHough;
    
    seqHough =  houghMatrix(T,R);
    parHough =  houghMatrix(T,R);
    clock_t startSeq = clock();
    seqHough = sequentialHough(imageMatrix,parHough,M,N,T,R,angles,deltaR);
    clock_t endSeq = clock();
    double time_usedSeq = (double)(endSeq - startSeq) / CLOCKS_PER_SEC;
    clock_t start = clock();
    parHough = parallelHough(imageMatrix,parHough,M,N,T,R,angles,deltaR);
    clock_t end = clock();
    double time_used = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tiempo Paralelo= %f segundos\n Tiempo Secuencial = %f segundos\n", time_used, time_usedSeq);
    parHough = umbralization(parHough,T,R,U);
    seqHough = umbralization(seqHough,T,R,U);
    writeOut(parHough,T,R,Outfile);
    return 0;

}

