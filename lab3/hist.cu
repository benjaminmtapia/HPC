#include "functions.h"

int D =0;


__host__ int main(int argc, char** argv){
    char* fileIn;
    int M = 0,N,numberOfThreads;
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
            sscanf(optarg,"%d",&numberOfThreads);
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
    //printf("i: %s, m: %d, n: %d, o: %s, t: %d, d: %d\n",fileIn,M,N,fileOut,numberOfThreads,D);
    int *I = readFile(fileIn,M,N);
    int *H = (int*) malloc(sizeof(int)*256);
    
    for(int i = 0; i<=255;i++){
        H[i]=0;
    }
    
    //histogramming(I,M,N,H);
    /*
    for(int i = 0; i <= 255;i++){
        printf("%d %d\n",i,H[i]);
    }
    */
   writeFile(fileOut,H,256);
}