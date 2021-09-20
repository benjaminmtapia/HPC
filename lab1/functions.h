# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <pmmintrin.h>
# include <immintrin.h>

int* readFile(char* fileName, int N);
void printmatrix(int** matrix, int M, int R);
void writeOut(int** image, int M, int R, char* fileName);
int getRindex(int*distances, int R, int r);
int** imageToMatrix(int* image, int N);