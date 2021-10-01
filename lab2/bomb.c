#include "functions.h"
#include <omp.h>

/*
Laboratorio 2 High Performance Computing
Benjamín Muñoz Tapia, 19818205-2
Vicente Vega Toro, 19501147-8
*/






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
    for(int i = 0 ; i<N;i++){
        printf("%d %f\n",i,material[i]);
    }

}

void simulate(struct parameters* parameters, int threads, int N, float* material){

    
    float MIN_ENERGY = pow(10,-3)/N;
    float* privateEnergy = (float*)malloc(sizeof(float)*N);
    for(int i = 0; i < N ; i++){
        privateEnergy[i] = 0;
    }
    float impact = 0;
    float Ei = 0;
    int i = 0;
    #pragma omp parallel num_threads(threads) shared(material) firstprivate(privateEnergy, impact,Ei,i)
    {
        #pragma omp for
        for(int j=0 ; j < parameters->particleNumber;j++){

            impact = parameters->particleImpacts[j];


            for(i = 0; i < N; i++){
                //printf("Hebra %d, i = %d, j = %d\n",omp_get_thread_num(),i,j);
                Ei =  (pow(10,3)*impact) / (N*sqrt(abs(j-i)+1));
                if(Ei>= MIN_ENERGY){
                    privateEnergy[i] += Ei;
                }
                
            }
        

        }

            #pragma omp critical
            {
                for(int k = 0; k<N;k++){
                    material[k] += privateEnergy[k];
                }
            }
               
    }
    for(int i = 0 ; i<N;i++){
        printf("%d %f\n",i,material[i]);
    }

}

int main(int argc, char** argv){

    int threads = 0;
    int N = 0;
    char* i = (char*)malloc(sizeof(char));
    char* o = (char*)malloc(sizeof(char));
    int D = 0;
    int c;
    if(argc<10){
        printf("faltan parámetros\n");
        exit(1);
    }

    while (( (c = getopt(argc, argv, "t:N:i:o:D")) != -1)){
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
            D = 1;
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
    sequential(parameters,threads,N,material);
    //simulate(parameters,threads,N,material);
    return 0;

}


