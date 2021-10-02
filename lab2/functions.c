#include "functions.h"

/*
Entradas: Nombre de la imagen, dimensiones que reciben por consola
Procesamiento: Se lee la imagen con fread y se almacena la data en un arreglo
Salida: Arreglo con información de la imagen en enteros
*/

float searchMax(int N, float *energy, int* index){
    float max = 0;
    for(int i =0 ; i<N; i++){
        if(energy[i]>max){
            max = energy[i];
            *index = i;
        }
    }
    return max;
}

void readFile(char* fileName, struct parameters* parameters){

    FILE* f = fopen(fileName,"r");
    if(f == NULL){
        printf("File doesn't exist\n");
        exit(1);
    }
    char* particleN = (char*)malloc(sizeof(char));
    char buffer[255];

    fgets(buffer,255,f);
    particleN =  buffer;
    //printf("numero de particulas: %s\n",particleN);
    parameters->particleNumber = atof(particleN);
    parameters->particleIds = (int*)malloc(sizeof(int)*(atoi(particleN)));
    parameters->particleImpacts = (int*)malloc(sizeof(int)*(atoi(particleN)));

    int i = 0;
    int idAux=0;
    float impactAux = 0;

    while(fgets(buffer,255,f)!=NULL && i!=parameters->particleNumber){
        sscanf(buffer,"%d %f",&idAux,&impactAux);
        parameters->particleIds[i] = idAux;
        parameters->particleImpacts[i] = impactAux;
        i++;
    }
    /*
    for(int i = 0; i < parameters->particleNumber; i++){
        printf("%d %d\n", parameters->particleIds[i], parameters->particleImpacts[i]);
    }
    */
  
}

void writeFile(char* fileName, int N, float* Energy){
    int maxIndex = 0;
    float max = searchMax(N,Energy,&maxIndex);
    FILE* out = fopen(fileName,"w");
    fprintf(out,"%d %f\n",maxIndex,max);
    for(int i = 0; i < N; i++){
        fprintf(out,"%d %f\n",i,Energy[i]);
    }
}