# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
#include <fcntl.h>

__host__ void histogramming(unsigned short int * A, int M, int N, int*H);
__host__ unsigned short int *readFile(char* filename, int M, int N);
__host__ void writeFile(char* filename, int* hist, int N);