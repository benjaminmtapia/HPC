# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <time.h>
#include <omp.h>
#include <math.h>

extern void niceprint(int N, float *Energy);

typedef struct parameters{
    int particleNumber;
    int* particleIds;
    int* particleImpacts;

} parameters;

void readFile(char* fileName, struct parameters* parameters);
void writeFile(char* fileName, int N, float* Energy);
float searchMax(int N, float *energy);