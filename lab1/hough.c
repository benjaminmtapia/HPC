#include "functions.h"
#include <math.h>
#include <immintrin.h>

# define MPI 3.14159265358979323846


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
    seqHough = sequentialHough(imageMatrix,seqHough,M,N,T,R,angles,deltaR);
    seqHough = umbralization(seqHough,T,R,U);
    writeOut(seqHough,T,R,Outfile);

    return 0;

}