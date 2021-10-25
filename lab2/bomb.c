#include "functions.h"


/*
Laboratorio 2 High Performance Computing
Benjamín Muñoz Tapia, 19818205-2
Vicente Vega Toro, 19501147-8
*/

int D;


void sequential(struct parameters* parameters, int threads, int N, float* material){
    float Ei = 0;
    int impactPosition = 0;
    float Ep = 0;
    float MIN_ENERGY = pow(10,-3)/N;
    for(int j = 0; j < parameters->particleNumber; j++){
        //for each particle
        Ep = parameters->particleImpacts[j];
        impactPosition = parameters->particleIds[j];
        for(int i = 0; i < N; i++){
            Ei = material[i];
            Ei = Ei + ( (pow(10,3) * Ep) / (N* sqrt(abs(impactPosition-i)+1)) );
            if(Ei >= MIN_ENERGY){
                material[i] = Ei;
            }
        }

    }

}

void simulate(struct parameters* parameters, int threads, int N, float* material){

    
    float MIN_ENERGY = pow(10,-3)/N;
    
    float impact = 0;
    float Ei = 0;
    int position = 0;

    #pragma omp parallel num_threads(threads) firstprivate(Ei,position, impact)
    {
     
        float* localEnergy = (float*)malloc(sizeof(float)*N);
        for(int i = 0; i < N ; i++){
            localEnergy[i] = 0;
        }
        #pragma omp for
        for(int j = 0; j<parameters->particleNumber;j++){
            impact = parameters->particleImpacts[j];
            position = parameters->particleIds[j];
            for(int i = 0; i < N ; i++){
                Ei = localEnergy[i];
                Ei = Ei + ( (pow(10,3)*impact) / (N*sqrt(abs(position-i)+1)));
                if( Ei>= MIN_ENERGY){
                    localEnergy[i] = Ei;
                    }

                }
        }
        #pragma omp critical
        {
            for(int i = 0; i <N; i++){
                material[i] += localEnergy[i];
            }
        }
    }    

}

int main(int argc, char** argv){

    int threads = 0;
    int N = 0;
    char* i = (char*)malloc(sizeof(char));
    char* o = (char*)malloc(sizeof(char));
    int c;
    if(argc<10){
        printf("faltan parámetros\n");
        exit(1);
    }

    while (( (c = getopt(argc, argv, "t:N:i:o:D:")) != -1)){
        switch (c)
        {
        case 't':
            threads = atof(optarg);
            break;

        case 'N':
            N = atof(optarg);
            break;

        case 'i':
            i = optarg;
            break;

        case 'o':
            o = optarg;
            break;

        case 'D':
            D = atof(optarg);
            if(D!=0 && D!=1){
                printf("parametros incorrectos para D, solo puede ser 0 o 1\n");
                exit(1);
            }
            break;
        }
    }
    //printf("t: %d, N: %d, i: %s, o: %s, D: %d\n",t,N,i,o,D);

    float* material = (float*) malloc(sizeof(float)*N);
    for (int i = 0; i < N; i++)
    {
        material[i] = 0;
    }
    
    struct parameters* parameters = (struct parameters*)malloc(sizeof(struct parameters));
    readFile(i,parameters);
    clock_t start = clock();
    simulate(parameters,threads,N,material);
    clock_t end = clock();
    double time = (double)(end-start)/CLOCKS_PER_SEC;
    float* energy = (float*) malloc(sizeof(float)*N);
    for(int i = 0; i < N; i++){
        energy[i] = material[i];
    }
    int maxIndex = 0;
    printf("hola\n");
    float max = searchMax(N,material,&maxIndex);
    if(D==1)
    niceprint(N,energy,max);
    printf("execution time: %f secs\n",time);
    writeFile(o,N,energy);
    return 0;

}


