#include "functions.h"
#include <math.h>
#include <immintrin.h>

__m256d _ZGVdN4v_cos(__m256d x);                    
__m256d _ZGVdN4v_exp(__m256d x);                    
__m256d _ZGVdN4v_log(__m256d x);                    
__m256d _ZGVdN4v_sin(__m256d x);                    
__m256d _ZGVdN4vv_pow(__m256d x, __m256d y); 
void    _ZGVdN4vvv_sincos(__m256d x, __m256i ptrs, __m256i ptrc);

__m256  _ZGVdN8v_cosf(__m256 x);            
__m256  _ZGVdN8v_expf(__m256 x);            
__m256  _ZGVdN8v_logf(__m256 x);            
__m256  _ZGVdN8v_sinf(__m256 x);            
__m256  _ZGVdN8vv_powf(__m256 x, __m256 y); 
void    _ZGVdN8vvv_sincosf(__m256 x, __m256i ptrs_lo, __m256i ptrs_hi, __m256i ptrc_lo, __m256i ptrc_hi);

int mm256_print_pd(__m256d x);
int mm256_print_ps(__m256 x);

__attribute__ ((noinline)) int mm256_print_pd(__m256d x){
    double vec_x[4];
    _mm256_storeu_pd(vec_x,x);
    printf("%12.8f %12.8f %12.8f %12.8f  \n", vec_x[3], vec_x[2], vec_x[1], vec_x[0]);
    return 0;
}

void loadRegisterPositions(int X, int Y, __m128* posX, __m128* posY){

}

int** parallelHough(int** image, int**houghMatrix, int M,int N,int T, int R, float* angles, float deltaR){
    __m256d posX, posY, theta, mcos, msin, icos, jsin, rDelta, rdistance, rindex;
    //posY = _mm256_set_pd(M_PI/2, 1.5, 1.0, 0.5);
    
    double* angulos = (double*)malloc(sizeof(double)*T);
    //z =_ZGVdN4v_cos(x);        printf("cos(x)        "); mm256_print_pd(z);
    for(int i = 0; i<T; i++){
        angulos[i] = angles[i];
    }
    rDelta = _mm256_set1_pd((double) deltaR);
    double* positions = (double*)malloc(sizeof(double)*4);
    int* auxIndex = (int*) malloc(sizeof(int)*4);
    for(int x = 0; x < M ; x++){
        for(int y = 0; y < N; y++){
            //if is edge
            if(image[x][y]!=0){
                double u = (double) x;
                double v = (double) y;
                posX = _mm256_set_pd(u,u,u,u);
                posY = _mm256_set_pd(v,v,v,v);
                //cargar los registros de posicion

                for(int i = 0; i < T; i+=4){
                    theta = _mm256_set_pd(angulos[i],angulos[i+1],angulos[i+2],angles[i+3]);
                    //mm256_print_pd(theta);
                    mcos = _ZGVdN4v_cos(theta);
                    
                    msin = _ZGVdN4v_sin(theta);
                    icos = _mm256_mul_pd(mcos,posX);
                    jsin = _mm256_mul_pd(msin,posY);
                    rdistance = _mm256_add_pd(icos,jsin);
                    rindex = _mm256_div_pd(rdistance,rDelta);
                    _mm256_store_pd(positions,rindex);
                    auxIndex[0] = (int)positions[0];
                    auxIndex[1] = (int)positions[1];
                    auxIndex[2] = (int)positions[2];
                    auxIndex[3] = (int)positions[3];
                    //printf("%lf %lf %lf %lf \n",positions[0],positions[1],positions[2],positions[3]);
                    houghMatrix[i][auxIndex[0]]+=1;
                    houghMatrix[i+1][auxIndex[1]]+=1;
                    houghMatrix[i+2][auxIndex[2]]+=1;
                    houghMatrix[i+3][auxIndex[3]]+=1;
                    
                }
                //printf("cos(x): %lf    ", mcos); 
            }
        }

    }

    return houghMatrix;
}

int** sequentialHough (int** image, int**houghMatrix, int M, int N,int T,int R, float* angles, float deltaR){

    for(int x = 0; x < M ; x++){
        for(int y = 0; y < N; y++){
            //if is edge
            if(image[x][y]!=0){
                //for each theta
                for(int i = 0; i < T; i++){
                    float theta = angles[i];
                    float r = x* cos(theta) + y* sin(theta);
                    int j = r/deltaR;
                    //if(j<R)
                    houghMatrix[i][j]+=1;
                }
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
    float deltaR = N*sqrt(2)/(R);
    float* angles = getAngles(deltaTheta,T);

    int** seqHough;
    seqHough =  houghMatrix(T,R);
    seqHough = parallelHough(imageMatrix,seqHough,M,N,T,R,angles,deltaR);
    seqHough = umbralization(seqHough,T,R,U);
    writeOut(seqHough,T,R,Outfile);

    return 0;

}

