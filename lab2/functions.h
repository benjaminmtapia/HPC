# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <pmmintrin.h>
# include <immintrin.h>
# include <time.h>
#include <fcntl.h>

#include <math.h>

typedef struct parameters{
    int particleNumber;
    int* particleIds;
    int* particleImpacts;

} parameters;

void readFile(char* fileName, struct parameters* parameters);