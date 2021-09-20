#include "functions.h"
#include <math.h>
# define MPI 3.14159265358979323846


int* getDistances(float deltaR,int R){
    int* distances = (int*)malloc(sizeof(int)*R);
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

int** sequentialHough (int** image, int N, int M, int R, int U){

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


    for(int i = 0 ; i < N; i++){
        for(int j = 0; j < N; j++){
            //if is edge
            if(image[i][j]!=0){
                //for each theta
                for(int k = 0; k < M; k++){
                    int theta = (angles[k]*MPI)/180;
                    int r = i*cos(theta) + j * sin(theta);
                    //int index = getRindex(distances, R, r);
                       houghMatrix[k][r] = houghMatrix[k][r]+1;
                    
                }
            }
        }
    }

    houghMatrix = umbralization(houghMatrix,M,R,U);
    return houghMatrix;

}
int main(int argc, char** argv){

    int N;
    char* Infile;
    char* Outfile;
    int T;
    int R;
    int U;
    int c;

    while (( (c = getopt(argc, argv, "i:o:N:T:R:U:")) != -1)){
        switch (c)
        {
        case 'i':
            Infile = optarg;
            break;

        case 'o':
            Outfile = optarg;
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
    image =readFile(Infile, N);
    int ** imageMatrix;
    imageMatrix = imageToMatrix(image, N);
    
   int** seqHough;
   seqHough = sequentialHough(imageMatrix,N,T,R,U);
   writeOut(seqHough,T,R,Outfile);

    return 0;

}